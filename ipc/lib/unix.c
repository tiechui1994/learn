#include "unp.h"

void Close(int fd) {
    if (close(fd) < 0) {
        err_sys("close error: %s", strerror(errno));
    }
}

void Dup2(int fd1, int fd2) {
    if (dup2(fd1, fd2) == -1) {
        err_sys("dup2 error: %s", strerror(errno));
    }
}

int Fcntl(int fd, int cmd, int arg) {
    int n;

    if ((n = fcntl(fd, cmd, arg)) == -1) {
        err_sys("fcntl error: %s", strerror(errno));
    }

    return n;
}

int Ioctl(int fd, uint request, void* arg) {
    int n;

    if ((n = ioctl(fd, request, arg)) == -1) {
        err_sys("ioctl error: %s", strerror(errno));
    }

    return n;
}


ssize_t Read(int fd, void* buf, size_t buflen) {
    ssize_t n;

    if ((n = read(fd, buf, buflen)) == -1) {
        err_sys("read error: %s", strerror(errno));
    }

    return n;
}

static int read_cnt;
static char* read_ptr;
static char read_buf[MAXLINE];

static ssize_t readchar(int fd, char* ch) {
    if (read_cnt <= 0) {
        again:
        if ((read_cnt = (int) read(fd, read_buf, sizeof(read_buf))) < 0) {
            err_msg("read error: %s", strerror(errno));
            if (errno == EINTR) {
                goto again;
            }

            return -1;
        } else if (read_cnt == 0) {
            return 0;
        }

        read_ptr = read_buf;
    }

    read_cnt--;
    *ch = *(read_ptr++);
    return 1;
}

ssize_t Readline(int fd, void* buf, size_t buflen) {
    ssize_t i, rc;
    char ch, * p;

    p = buf;
    for (i = 1; i < buflen; i++) {
        if ((rc = readchar(fd, &ch)) == 1) {
            *(p++) = ch;
            if (ch == '\n') {
                break;
            }
        } else if (rc == 0) {
            *p = 0;
            return i - 1;
        } else {
            return -1;
        }
    }

    *p = 0;
    return i;
}

ssize_t Write(int fd, void* buf, size_t buflen) {
    ssize_t n;
    if ((n = write(fd, buf, buflen)) != buflen) {
        err_sys("write error: %s", strerror(errno));
    }

    return n;
}

ssize_t Writen(int fd, void* buf, size_t buflen) {
    size_t nleft;
    ssize_t nwriten;
    const char* p;

    p = buf;
    nleft = buflen;

    while (nleft > 0) {
        if ((nwriten = write(fd, p, nleft)) <= 0) {
            err_msg("write err: %s", strerror(errno));
            if (nwriten < 0 && errno == EINTR) {
                nwriten = 0;
            } else {
                return -1;
            }
        }

        nleft -= nwriten;
        p += nwriten;
    }

    if (nleft != 0) {
        err_sys("writen error: %s", strerror(errno));
    }

    return buflen;
}


ssize_t Readv(int fd, const struct iovec* iov, int iovcnt) {
    ssize_t n;
    if ((n = readv(fd, iov, iovcnt) < 0)) {
        err_sys("readv error: %s", strerror(errno));
    }

    return n;
}

ssize_t Writev(int fd, const struct iovec* iov, int iovcnt) {
    ssize_t n;
    if ((n = writev(fd, iov, iovcnt) < 0)) {
        err_sys("writev error: %s", strerror(errno));
    }

    return n;
}

int Select(int maxfd, fd_set* rset, fd_set* wset, fd_set* eset, struct timeval* timeout) {
    return select(maxfd, rset, wset, eset, timeout);
}