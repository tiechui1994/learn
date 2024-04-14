#include "unp.h"

char* Fgets(char* msg, int size, FILE* in) {
    char* p;
    if ((p = fgets(msg, size, in)) == NULL && ferror(in)) {
        err_sys("fgets error");
    }
    return p;
}

int Fputs(const char* msg, FILE* out) {
    int n;
    if ((n = fputs(msg, out)) == EOF) {
        err_sys("fputs error");
    }

    return n;
}

const char* Inet_ntop(int family, const void* addrp, char* bufp, size_t len) {
    const char* ptr;

    if (bufp == NULL) {
        err_quit("NULL 3rd argument to inet_ntop");
    }

    if ((ptr = inet_ntop(family, addrp, bufp, (socklen_t) len)) == NULL) {
        err_sys("inet_ntop error");
    }

    return ptr;
}

void Inet_pton(int family, const char* strp, void* bufp) {
    int n;


    if ((n = inet_pton(family, strp, bufp)) < 0) {
        err_sys("inet_pton error for %s", strp);
    } else if (n == 0) {
        err_quit("inet_pton error for %s", strp);
    }
}