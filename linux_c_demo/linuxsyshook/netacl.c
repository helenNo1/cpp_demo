#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    // 获取目标主机和端口
    struct sockaddr_in *addr_in = (struct sockaddr_in *)addr;
    char *ip = inet_ntoa(addr_in->sin_addr);
    int port = ntohs(addr_in->sin_port);

    // 检查连接请求是否符合访问控制规则
    if (strcmp(ip, "blocked_ip") == 0) {
        printf("Connection to %s:%d denied: Access control rule\n", ip, port);
        return -1;
    }

    // 调用原始的connect函数
    int (*original_connect)(int, const struct sockaddr *, socklen_t);
    original_connect = dlsym(RTLD_NEXT, "connect");
    return original_connect(sockfd, addr, addrlen);
}

