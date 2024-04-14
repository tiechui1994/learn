#include "unp.h"

void Getsockname(int fd, struct sockaddr* sa, socklen_t* salenp) {
    if (getsockname(fd, sa, salenp) < 0) {
        err_sys("getsockname error");
    }
}

void Getsockopt(int fd, int level, int optname, void* optval, socklen_t* optlenp) {
    if (getsockopt(fd, level, optname, optval, optlenp) < 0) {
        err_sys("getsockopt error");
    }
}

void Setsockopt(int fd, int level, int optname, const void* optval, socklen_t optlen) {
    if (setsockopt(fd, level, optname, optval, optlen) < 0) {
        err_sys("setsockopt error");
    }
}

int Socket(int family, int type, int protocal) {
    int n;

    if ((n = socket(family, type, protocal)) < 0) {
        err_sys("socket error");
    }

    return n;
}

void Listen(int fd, int backlog) {
    char* ptr;

    if ((ptr = getenv("LISTENQ")) != NULL) {
        backlog = atoi(ptr);
    }

    if (listen(fd, backlog) < 0) {
        err_sys("listen error");
    }
}

void Bind(int fd, const struct sockaddr* sa, socklen_t salen) {
    if (bind(fd, sa, salen) < 0) {
        err_sys("bind error");
    }
}

int Accept(int fd, struct sockaddr* sa, socklen_t* salenp) {
    int n;

    again:
    if ((n = accept(fd, sa, salenp)) < 0) {
        if (errno == EPROTO || errno == ECONNABORTED)
            goto again;
        else
            err_sys("accept error");
    }

    return n;
}

void Connect(int fd, const struct sockaddr* sa, socklen_t salen) {
    if (connect(fd, sa, salen) < 0) {
        err_sys("connect error");
    }
}

int Shutdown(int fd, int howto) {
    int n;
    if ((n = shutdown(fd, howto)) < 0) {
        err_sys("shutdown error");
    }

    return n;
}

void Send(int fd, const void* ptr, size_t nb, int flags) {
    if (send(fd, ptr, nb, flags) != (ssize_t) nb) {
        err_sys("send error");
    }
}

void Sendmsg(int fd, const struct msghdr* msg, int flags) {
    uint i;
    ssize_t nb = 0;

    for (i = 0; i < msg->msg_iovlen; i++) {
        nb += msg->msg_iov[i].iov_len;
    }

    if (sendmsg(fd, msg, flags) != nb) {
        err_sys("sendmsg error");
    }
}

void Sendto(int fd, const void* ptr, size_t nb, int flags, const struct sockaddr* sa, socklen_t salen) {
    if (sendto(fd, ptr, nb, flags, sa, salen) != (ssize_t) nb) {
        err_sys("sendto error");
    }
}

ssize_t Recv(int fd, void* ptr, size_t nb, int flags) {
    ssize_t n;
    if ((n = recv(fd, ptr, nb, flags)) != (ssize_t) nb) {
        err_sys("recv error");
    }

    return n;
}

ssize_t Recvmsg(int fd, struct msghdr* msg, int flags) {
    ssize_t n;

    if ((n = recvmsg(fd, msg, flags)) < 0) {
        err_sys("recvmsg error");
    }

    return n;
}

ssize_t Recvfrom(int fd, void* ptr, size_t nb, int flags, struct sockaddr* sa, socklen_t* salen) {
    ssize_t n;

    if ((n = recvfrom(fd, ptr, nb, flags, sa, salen)) < 0) {
        err_sys("recvfrom error");
    }

    return n;
}

