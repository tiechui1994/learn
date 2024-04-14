#include "unp.h"

int Tcp_listen(const char* host, const char* serv, socklen_t* addrlenp) {
    int listenfd, n;
    const int on = 1;
    struct addrinfo hints;
    struct addrinfo* res;
    struct addrinfo* ressave;

    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((n = getaddrinfo(host, serv, &hints, &res)) != 0) {
        err_quit("tcp_listen error for %s, %s:%s", host, serv, gai_strerror(n));
    }

    ressave = res;

    do {
        listenfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (listenfd < 0) {
            continue;
        }

        Setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

        if (bind(listenfd, res->ai_addr, res->ai_addrlen) == 0) {
            break;
        }

        Close(listenfd);
    } while ((res = res->ai_next));

    if (res == NULL) {
        err_sys("tcp_listen error for %s, %s", host, serv);
    }

    Listen(listenfd, LISTENQ);

    if (addrlenp) {
        *addrlenp = res->ai_addrlen;
    }

    freeaddrinfo(ressave);

    return listenfd;
}

int Tcp_connect(const char* host, const char* serv) {
    int sockfd, n;
    struct addrinfo hints;
    struct addrinfo* res;
    struct addrinfo* ressave;

    if ((n = getaddrinfo(host, serv, &hints, &res)) != 0) {
        err_quit("tcp_connect error for %s, %s:%s", host, serv, gai_strerror(n));
    }

    ressave = res;

    do {
        sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (sockfd < 0) {
            continue;
        }

        if (connect(sockfd, res->ai_addr, res->ai_addrlen) == 0) {
            break;
        }

        Close(sockfd);
    } while ((res = res->ai_next) != NULL);

    if (res == NULL) {
        err_sys("tcp_connect error for %s, %s", host, serv);
    }

    freeaddrinfo(ressave);
    return sockfd;
}