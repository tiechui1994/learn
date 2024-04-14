#include <sys/param.h>

#include "unp.h"

char* time_str() {
    struct timeval tv;
    static char str[30];

    if (gettimeofday(&tv, NULL) < 0) {
        err_sys("gettimeofday error");
    }

    ssize_t n = strftime(str, sizeof(str), "%H:%M:%S", localtime(&tv.tv_sec));
    snprintf(str + n, sizeof(str) - n, ".%06ld", tv.tv_usec);
    return str;
}

void str_cli_select(FILE* fd, int sockfd) {
    int maxfd, stdineof;
    fd_set rset;
    char buf[MAXLINE];
    size_t n;


    stdineof = 0;
    FD_ZERO(&rset);

    for (;;) {
        if (stdineof == 0) {
            FD_SET(fileno(fd), &rset);
        }

        FD_SET(sockfd, &rset);
        maxfd = MAX(fileno(fd), sockfd) + 1;
        Select(maxfd, &rset, NULL, NULL, NULL);

        if (FD_ISSET(sockfd, &rset)) {
            if ((n = (size_t) Read(sockfd, buf, MAXLINE)) == 0) {
                if (stdineof == 1) {
                    return;
                } else {
                    err_quit("str_cli: server terminated pre");
                }
            }
            Write(fileno(stdout), buf, n);
        }

        if (FD_ISSET(fileno(fd), &rset)) {
            if ((n = (size_t) Read(fileno(fd), buf, MAXLINE)) == 0) {
                stdineof = 1;
                Shutdown(sockfd, SHUT_WR);
                FD_CLR(fileno(fd), &rset);
                continue;
            }

            Write(sockfd, buf, n);
        }
    }
}

/*
 * to: 标准输入 -> 服务器
 *
 * |<- has sent ->|<- will send ->|<- free space ->|
 *              tooptr          toiptr
 *
 *
 * fr: 服务器 -> 标准输出
 *
 * |<- has writen ->|<- will write ->|<- free space ->|
 *                 froptr          friptr
 *
 * stdin read -> toiptr++  -> socket write
 * socket read -> friptr++ -> stdout write
 *
 * socket write -> tooptr++
 * stdout write -> froptr++
 *
 * when stdin read EOF -> stdineof=1 and maybe shutdown
 * when socket read EOF -> maybe shutdown
 */
// select, nonblock
void str_cli_nonblock(FILE* fd, int sockfd) {
    int maxfdp, val, stdineof;
    ssize_t n, nw;
    fd_set rset, wset;
    char to[MAXLINE], fr[MAXLINE], cp[MAXLINE];
    char* toiptr, * tooptr, * friptr, * froptr;

    // nonblock
    val = Fcntl(sockfd, F_GETFL, 0);
    Fcntl(sockfd, F_SETFL, val | O_NONBLOCK);

    val = Fcntl(STDIN_FILENO, F_GETFL, 0);
    Fcntl(STDIN_FILENO, F_SETFL, val | O_NONBLOCK);

    val = Fcntl(STDOUT_FILENO, F_GETFL, 0);
    Fcntl(STDOUT_FILENO, F_SETFL, val | O_NONBLOCK);

    toiptr = tooptr = to;
    friptr = froptr = fr;
    stdineof = 0;

    maxfdp = MAX(MAX(STDIN_FILENO, STDOUT_FILENO), sockfd) + 1;

    for (;;) {
        FD_ZERO(&rset);
        FD_ZERO(&wset);

        if (stdineof == 0 && toiptr < &to[MAXLINE]) {
            FD_SET(STDIN_FILENO, &rset); // read from stdin
        }
        if (friptr < &fr[MAXLINE]) {
            FD_SET(sockfd, &rset); // read from stdout
        }
        if (toiptr != tooptr) {
            FD_SET(sockfd, &wset); // data write to socket
        }
        if (friptr != froptr) {
            FD_SET(STDOUT_FILENO, &wset); // data write to stdout
        }

        Select(maxfdp, &rset, &wset, NULL, NULL);

        // stdin read
        if (FD_ISSET(STDIN_FILENO, &rset)) {
            if ((n = read(STDIN_FILENO, toiptr, &to[MAXLINE] - toiptr)) < 0) {
                if (errno != EWOULDBLOCK) {
                    err_sys("read error on stdin: %s", strerror(errno));
                }
            } else if (n == 0) {
                fprintf(stderr, "%s: EOF on stdin\n", "");
                stdineof = 1;
                if (toiptr == tooptr) {
                    shutdown(sockfd, SHUT_WR); // send FIN
                }
            } else {
                fprintf(stderr, "%s: read %ld bytes from stdin\n", time_str(), n);
                toiptr += n;
                FD_SET(sockfd, &wset); // try and write to socket
            }
        }

        // socket read
        if (FD_ISSET(sockfd, &rset)) {
            if ((n = read(sockfd, friptr, &fr[MAXLINE] - friptr)) < 0) {
                if (errno != EWOULDBLOCK) {
                    err_sys("read error on socket");
                }
            } else if (n == 0) {
                fprintf(stderr, "%s: EOF on socket\n", time_str());
                if (stdineof) {
                    return;
                } else {
                    err_quit("str_cli: server terminated prem");
                }
            } else {
                fprintf(stderr, "%s: read %ld bytes from socket\n", time_str(), n);
                friptr += n;
                FD_SET(STDOUT_FILENO, &wset); // try and write
            }
        }

        // stdout write
        if (FD_ISSET(STDOUT_FILENO, &wset) && ((n = friptr - froptr) > 0)) {
            if ((nw = write(STDOUT_FILENO, froptr, (size_t) n)) < 0) {
                if (errno != EWOULDBLOCK) {
                    err_sys("write error on stdout");
                }
            } else {
                fprintf(stderr, "%s: write %ld bytes to stdout\n", time_str(), nw);
                froptr += nw;
                if (friptr == froptr) {
                    froptr = friptr = fr; // back to begin
                }
            }
        }

        // socket write
        if (FD_ISSET(sockfd, &wset) && ((n = toiptr - tooptr) > 0)) {
            if ((nw = write(sockfd, tooptr, (size_t) n)) < 0) {
                if (errno != EWOULDBLOCK) {
                    err_sys("write error on sockfd");
                }
            } else {
                fprintf(stderr, "%s: write %ld bytes to socket\n", time_str(), nw);
                tooptr += nw;
                if (toiptr == tooptr) {
                    toiptr = tooptr = to; // back to begin
                    if (stdineof) {
                        shutdown(sockfd, SHUT_WR);
                    }
                }
            }
        }
    }
}

// noneblock connect with timeout
// timeout used by select.
int connect_nonb(int sockfd, const struct sockaddr* addr, socklen_t addrlen, int timeo) {
    int flags, n, error;
    fd_set rset, wset;
    struct timeval tv;

    flags = Fcntl(sockfd, F_GETFL, 0);
    Fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

    error = 0;
    if ((n = connect(sockfd, addr, addrlen)) < 0) {
        if (errno != EWOULDBLOCK) {
            return -1;
        }
    }

    // special: local link
    if (n == 0) {
        goto done;
    }

    // select with timeout
    FD_ZERO(&rset);
    FD_SET(sockfd, &rset);
    wset = rset;
    tv.tv_sec = timeo;
    tv.tv_usec = 0;

    if ((Select(sockfd + 1, &rset, &wset, NULL, timeo ? &tv : NULL)) == 0) {
        close(sockfd);
        errno = ETIMEDOUT;
        return -1;
    }

    if (FD_ISSET(sockfd, &rset) || FD_ISSET(sockfd, &wset)) {
        socklen_t len = sizeof(error);
        if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len) < 0) {
            return -1;
        }
    } else {
        err_quit("select error: sockfd not set");
    }


    done:
    Fcntl(sockfd, F_SETFL, flags);

    if (error) {
        close(sockfd);
        errno = error;
        return -1;
    }

    return 0;
}
