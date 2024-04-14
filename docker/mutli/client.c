#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFLEN 255

#if 0
#define MCAST_IP   "239.0.0.11"
#define MCAST_PORT 15550
#else
#define MCAST_IP   "225.1.1.1"
#define MCAST_PORT 8080
#endif

static int socket_set_nonblock(int s) {
    int flags, res;

    flags = fcntl(s, F_GETFL, 0);
    if (flags < 0) {
        flags = 0;
    }

    res = fcntl(s, F_SETFL, flags | O_NONBLOCK);
    if (res < 0) {
        printf("fcntl return err:%d!\n", res);
        return -1;
    }

    return 0;
}

int main(int argc, char** argv) {
    int fd;
    int ret, n;

    /* 创建socket */
    fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (fd == -1) {
        printf("create udp socket error %d", -errno);
        return -1;
    }

    /* 设置socket参数 */
    socket_set_nonblock(fd);

    /* 允许多个应用绑定同一个本地端口接收数据包 */
    int yes = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
        printf("setsockopt: SO_REUSEADDR error\n");
        goto failed;
    }

    /* 禁止组播数据回环 */
    int loop = 0;
    if (setsockopt(fd, IPPROTO_IP, IP_MULTICAST_LOOP, (char*) &loop, sizeof(loop)) < 0) {
        printf("setsockopt: IP_MULTICAST_LOOP error\n");
        goto failed;
    }

    /** --------- 设置目标地址  ------------ **/
    struct sockaddr_in addr;
    memset(addr.sin_zero, 0, sizeof(addr.sin_zero));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(MCAST_PORT);

    /* 绑定网卡 */
    ret = bind(fd, (struct sockaddr*) &addr, sizeof(addr));
    if (ret < 0) {
        printf("Bind socket error, ret=%d\n", ret);
        goto failed;
    }

    /** --------- 加入组播 ----------- **/
    struct ip_mreq group;
    group.imr_multiaddr.s_addr = inet_addr(MCAST_IP);
    group.imr_interface.s_addr = htonl(INADDR_ANY);
    ret = setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &group, sizeof(group));
    if (ret < 0) {
        printf("setsockopt: IP_ADD_MEMBERSHIP error, ret=%d\n", ret);
        goto failed;
    }

    printf("create rtp udp socket %d ok\n", fd);

    /* 循环接收网络上来的组播消息 */
    struct timeval tv;
    char recmsg[BUFLEN + 1];
    fd_set rfds;
    for (;;) {
        tv.tv_sec = 3;
        tv.tv_usec = 0;

        FD_ZERO(&rfds);
        FD_SET(fd, &rfds);

        ret = select(fd + 1, &rfds, NULL, NULL, &tv);
        if (-1 == ret) {
            printf("===> func: %s, line: %d, Socket select error\n", __func__, __LINE__);
            return -1;
        } else if (0 == ret) {
            printf("===> func: %s, line: %d, select timeout\n", __func__, __LINE__);
            continue;
        }

        eagain:
        n = (int) recv(fd, recmsg, BUFLEN, 0);
        if (n < 0) {
            printf("recvfrom err in udptalk!, n: %d, errno: %d\n", n, -errno);
            if (EAGAIN == errno)
                goto eagain;
            else
                return -1;
        } else if (n == 0) {
            printf("recv data siez: %d\n", n);
        } else {
            printf("s: %d, peer: %s \n", n, recmsg);
        }

    }

    return 0;

    failed:
    if (fd > 0)
        close(fd);
    return -1;
}
