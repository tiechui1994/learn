#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define MCAST_PORT 8080
#define MCAST_GROUP "225.1.1.1"

int main(int argc, char* argv[]) {
    struct sockaddr_in addr;
    int fd;
    char* message = "Hello, World!";

    /* 创建socket */
    if ((fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        perror("socket");
        exit(1);
    }

    /* 设置socket参数 */
    unsigned char ttl = 32;
    if (setsockopt(fd, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl)) < 0) {
        perror("Setsock ttl failed \n");
        exit(1);
    }

    struct in_addr local;
    local.s_addr = htonl(INADDR_ANY);
    if (setsockopt(fd, IPPROTO_IP, IP_MULTICAST_IF, (char*) &local, sizeof(local)) < 0) {
        printf("setsockopt: IP_MULTICAST_IF error\n");
        exit(1);
    }

    /* 设置目标地址 */
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(MCAST_GROUP);
    addr.sin_port = htons(MCAST_PORT);

    printf("message: %s, (HEX: 0x%08x)\n", message, *(unsigned int*) message);
    /* 发送消息 */
    while (1) {
        if (sendto(fd, message, strlen(message), 0, (struct sockaddr*) &addr, sizeof(addr)) < 0) {
            perror("sendto");
            exit(1);
        }
        sleep(2); //会影响接受端select的超时时间，延时越大，select的超时越大
    }
}