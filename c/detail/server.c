#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>
#include <ctype.h>

#define PORT 8000
#define SERVER "127.0.0.1"
#define BACKLOG 100

typedef struct client {
    int sock;
    struct sockaddr_in addr;
} client;

void* handle(void* arg) {
    client* client = (struct client*) arg;
    int client_sock = client->sock;
    struct sockaddr_in client_addr = client->addr;
    char buf[1024];
    ssize_t n;

    printf("client(%s:%d) connect.... \n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    while (1) {
        n = recv(client_sock, buf, sizeof(buf) - 1, 0);
        buf[n] = '\0';
        if (strstr(buf, "exit")) {
            close(client_sock);
            break;
        }

        printf("client(%s:%d)\tmsg:%s \n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buf);
        for (int i = 0; i < n; i++) {
            buf[i] = (char) toupper(buf[i]);
        }
        send(client_sock, buf, (size_t) n, 0);
    }

    printf("client(%s:%d) stop.... \n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    close(client_sock);
    return NULL;
}

int server() {
    struct sockaddr_in server_addr;

    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER);
    server_addr.sin_port = htons(PORT);

    // bind sock and addr
    bind(server_sock, (struct sockaddr*) &server_addr, sizeof(server_addr));

    // listen
    listen(server_sock, BACKLOG);

    int client_sock;
    struct sockaddr_in client_addr;
    while (1) {
        socklen_t client_len = sizeof(client_addr);
        if ((client_sock = accept(server_sock, (struct sockaddr*) &client_addr, &client_len)) < 0) {
            perror("accept error");
            continue;
        }
        pthread_t tid;
        client arg = {
                sock:client_sock,
                addr:client_addr,
        };
        errno = pthread_create(&tid, NULL, &handle, (void*) &arg);
        if errno {
            perror("pthread_create\n");
            close(client_sock);
            break;
        }

        pthread_join(tid, NULL);
    }

    close(server_sock);
    return 0;
}

/**
 * struct sockaddr {
 *   unsigned short sa_family;
 *   char           sa_data[14];
 * }
 *
 * struct sockaddr_in {
 *   short          sin_family;
 *   unsigned short sin_port;   // 2bytes, 端口号
 *   struct in_addr sin_addr;   // 4bytes, 32位的ip地址
 *   char           sin_zero[8];// 8bytes
 * }
 *
 * struct in_addr {
 *   unsigned long s_addr;  // 4 bytes
 * }
 *
 *
 * // ip转换: arpa/inet.h
 * inet_aton()  十进制ip地址转换成网络字节顺序的32位二进制数
 * inet_addr()  与inet_aton()功能一样.
 * inet_ntoa()  将网络字节的32位二进制数转换成十进制ip地址
 *
 * inet_pton(int family, const char *strptr, void *addrptr)  与inet_aton()功能类似, 通用. addrptr可以是struct in_addr* 或者 int
 * inet_ntop(int family, const void *addrptr, char *strptr, socklen_t len)  与inet_ntoa()功能类似, 通用
 *
 *
 * // 高低字节: netinet/in.h
 * htons()  短整型主机转网络字节序
 * htonl()  长整型主机转网络字节序
 * ntohs()  短整型网络转主机字节序
 * ntohl()  长整型网络转主机字节序
 *
 * // 域名转换
 *
 * struct hostent {
 *  char    *h_name;            // official name of host
 *  char    **h_aliases;        // alias list
 *  int     h_addrtype;         // host address type. AF_INET, AF_INET6
 *  int     h_length;           // ip地址长度
 *  char    **h_addr_list;      // list of addresses
 *  h_addr	h_addr_list[0];     // addrr
 * }
 * gethostbyname()   主机名转换成ip地址
 * gethostbyaddr(const char* addrptr, int len, int family)   ip地址转换成主机名
 */
void ip_convert() {
    struct in_addr addr;
    inet_aton("127.0.0.1", &addr);
    printf("inet_aton:%d\n", addr.s_addr);
    printf("inet_addr:%d\n", inet_addr("127.0.0.1"));

    printf("inet_ntoa:%s\n", inet_ntoa(addr));


    struct in_addr inaddr;
    inet_pton(AF_INET, "127.0.0.1", &inaddr);
    printf("inet_pton:%d\n", inaddr.s_addr);

    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &inaddr, ip, sizeof(ip));
    printf("inet_ntop:%s\n", ip);
}

void num_convert() {
    printf("htons:%u\n", htons(1000));
    printf("htonl:%u\n", htonl(1000));

    printf("ntohs:%u\n", ntohs(59395));
    printf("ntohl:%u\n", ntohl(3892510720));
}

void domain_convert() {
    // gethostbyname
    struct hostent* host = gethostbyname("www.google.com");
    printf("h_name: %s\n", host->h_name);
    printf("h_length: %d\n", host->h_length);
    printf("h_addrtype: %d, AF_INET: %d\n", host->h_addrtype, AF_INET);
    for (; *host->h_aliases != NULL; host->h_aliases++) {
        printf("h_alias:%s\n", host->h_aliases[0]);
    }

    for (; *host->h_addr_list != NULL; host->h_addr_list++) {
        char addr[INET_ADDRSTRLEN];
        inet_ntop(host->h_addrtype, host->h_addr, addr, sizeof(addr));
        printf("h_addr: %s\n", addr);
    }

    // gethostbyaddr
    char ip[] = "205.186.152.122";
    struct hostent* hostnode = gethostbyaddr(ip, sizeof(ip), AF_INET);
    printf("h_name:%s\n", hostnode->h_name);

}

void print(void* ptr) {
    char* msg = (char*) ptr;
    struct tm time;
    timelocal(&time);
    ssize_t now = time.tm_gmtoff;
    while (1) {
        timegm(&time);
        if (time.tm_gmtoff - now > 400) {
            break;
        }
        printf("%d-%d-%d %d:%d:%d message is:%s\n", time.tm_year + 1900, time.tm_mon, time.tm_mday, time.tm_hour,
               time.tm_min,
               time.tm_sec, msg);
        sleep(1);
    }
}

void thread_use() {
    pthread_t thread;
    void* retval;
    char* message = "Hello world";
    int ret_thread = pthread_create(&thread, NULL, (void*) &print, (void*) message);
    if (ret_thread == 0) {
        printf("create thread success. pid:%ld\n", thread);
    } else {
        printf("create thread failed\n");
    }

    pthread_join(thread, &retval);
}


/**
 * time:
 *
 * time_t // long类型
 *
 * struct tm {
 *  int tm_sec;
 *  int tm_min;
 *  int tm_hour;
 *  int tm_mday;  // Day [1-31]
 *  int tm_mon;
 *  int tm_year;  // Year - 1900
 *  int tm_wday;  // Day of week. [0-6]
 *  int tm_yday;  // Days in year.[0-365]
 *  int tm_isdst; // DST. [-1/0/1]
 *  long int tm_gmtoff;  // Seconds east of UTC
 *  const char *tm_zone; // Timezone abbreviation
 * }
 *
 * 获取时间:
 *
 * time_t time (time_t *timer) // 当timer为NULL, 获取时间戳, 距1970-01-01 08:00:00的时间戳
 *
 * struct tm* localtime(const time_t *timer) // 本地时间
 * struct tm* gmtime(const time_t *timer)    // GMT时间
 *
 *
 * 时间格式化: %a %b %c %H:%M:%S %Y
 * char* ctime (const time_t *timer)
 * char* asctime (const struct tm *tp)
 *
 * size_t strftime (char *str, size_t maxsize, const char *format, const struct tm *tp)
 *
 * 构建时间:
 * time_t time_t mktime (struct tm *tp)
 *
 */
void time_use() {
    struct tm* local;
    time_t t = time(NULL);

    local = localtime(&t);
    printf("local: %d:%d:%d \n", local->tm_hour, local->tm_min, local->tm_sec);
    printf("date: %d-%d-%d \n", local->tm_year + 1900, local->tm_mon, local->tm_mday);

    local = gmtime(&t);
    printf("utc: %d:%d:%d \n", local->tm_hour, local->tm_min, local->tm_sec);
    printf("date: %d-%d-%d \n", local->tm_year + 1900, local->tm_mon, local->tm_mday);

    printf("ctime time: %s\n", ctime(&t));
    printf("asctime time: %s\n", asctime(local));

    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", (const struct tm*) local);
    printf("formattime time: %s\n", time_str);
}

int main() {
    server();
}