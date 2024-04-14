#include <sys/types.h>	/* basic system data types */
#include <sys/socket.h>	/* basic socket definitions */

#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>  // fcntl()
#include <signal.h> // sigaction(), sigemptyset()
#include <time.h>   // ctime(), clock(), mktime(), strftime()

#include <netinet/in.h> // sockaddr_in
#include <arpa/inet.h>  // inet()
#include <sys/ioctl.h>  // ioctl()
#include <sys/uio.h>    // readv(), writev()
#include <sys/time.h>   // gettimeofday()
#include <sys/select.h> // select(), pselect()
#include <sys/poll.h>   // poll()
#include <sys/epoll.h>  // epoll_create(), epoll_ctl(), epoll_wait()

#define LISTENQ     1024    /* 2nd argument to listen() */
#define MAXLINE     4096    /* max text line length */
#define BUFFSIZE    8192    /* buffer size for reads and writes */

#define SERV_PORT 5555

typedef void Sigfuc(int);


// SOCKET
int Socket(int family, int type, int flags);

void Bind(int fd, const struct sockaddr* addr, socklen_t addrlen);

void Connect(int fd, const struct sockaddr* addr, socklen_t addrlen);

void Listen(int fd, int backlog);

int Accept(int fd, struct sockaddr* addr, socklen_t* addrlen);

int Shutdown(int fd, int how);

void Setsockopt(int fd, int level, int key, const void* optval, socklen_t optlen);

void Getsockopt(int fd, int level, int key, void* optval, socklen_t* optlen);

void Send(int fd, const void* msg, size_t size, int flags);

void Sendmsg(int fd, const struct msghdr* msgh, int flags);

void Sendto(int fd, const void* msg, size_t size, int flags, const struct sockaddr* addr, socklen_t addrlen);

ssize_t Recv(int fd, void* msg, size_t size, int flags);

ssize_t Recvmsg(int fd, struct msghdr* msgh, int flags);

ssize_t Recvfrom(int fd, void* msg, size_t size, int flags, struct sockaddr* addr, socklen_t* addrlen);

// UNIX
void Close(int fd);

int Fcntl(int fd, int cmd, int arg);

int Ioctl(int fd, uint cmd, void* arg);

ssize_t Read(int fd, void* buf, size_t buflen);

ssize_t Write(int fd, void* buf, size_t buflen);

ssize_t Writen(int fd, void* buf, size_t buflen);

ssize_t Readline(int fd, void* buf, size_t buflen);

ssize_t Readv(int fd, const struct iovec* iov, int iovcnt);

ssize_t Writev(int fd, const struct iovec* iov, int iovcnt);

int Select(int maxfd, fd_set* rset, fd_set* wset, fd_set* eset, struct timeval* timeout);

// SIGNAL
Sigfuc* Signal(int, Sigfuc*);

Sigfuc* Signal_intr(int, Sigfuc*);


// TCP
int Tcp_listen(const char*, const char*, socklen_t*);

int Tcp_connect(const char*, const char*);


// LOG
void err_dump(const char*, ...);

void err_msg(const char*, ...);

void err_quit(const char*, ...);

void err_ret(const char*, ...);

void err_sys(const char*, ...);

// UTIL
char* Fgets(char* msg, int size, FILE* in);

int Fputs(const char* msg, FILE* out);

const char* Inet_ntop(int family, const void* addrp, char* bufp, size_t len);

void Inet_pton(int family, const char* strp, void* bufp);

// COMMON
char* time_str();

void str_cli_select(FILE* fp, int sockfd);

void str_cli_nonblock(FILE* fp, int sockfd);

int connect_nonb(int sockfd, const struct sockaddr* addr, socklen_t addrlen, int timeo);