#include "unp.h"

Sigfuc* Signal(int signo, Sigfuc* func) {
    struct sigaction act, oact;

    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    if (signo == SIGALRM) {
#ifdef SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT;
#endif
    } else {
#ifdef SA_RESTART
        act.sa_flags |= SA_RESTART;
#endif
    }

    if (sigaction(signo, &act, &oact) < 0) {
        err_sys("signal error");
    }

    return oact.sa_handler;
}

Sigfuc* Signal_intr(int signo, Sigfuc* func) {
    struct sigaction act, oact;

    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

#ifdef SA_INTERRUPT
    act.sa_flags |= SA_INTERRUPT;
#endif

    if (sigaction(signo, &act, &oact) < 0) {
        err_sys("signal_intr error");
    }

    return oact.sa_handler;
}