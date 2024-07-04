#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

// 声明原始的socket函数
static int (*original_socket)(int domain, int type, int protocol);

// 重写socket函数
int socket(int domain, int type, int protocol) {
    // 调用原始的socket函数
    original_socket = dlsym(RTLD_NEXT, "socket");
    int sockfd = original_socket(domain, type, protocol);

    // 检查是否为DNS请求
    if (domain == AF_INET && type == SOCK_DGRAM && protocol == IPPROTO_UDP) {
        // 获取进程ID和可执行文件信息
        pid_t pid = getpid();
        char exe_path[1024];
        ssize_t len = readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1);
        if (len != -1) {
            exe_path[len] = '\0';
        } else {
            perror("readlink");
            exit(EXIT_FAILURE);
        }

        // 打印进程ID和可执行文件信息
        printf("Process ID: %d\n", pid);
        printf("Executable file: %s\n", exe_path);

        // 解析DNS请求的域名
        struct sockaddr_in addr;
        socklen_t addr_len = sizeof(addr);
        if (getsockname(sockfd, (struct sockaddr *)&addr, &addr_len) == 0) {
            char ip_str[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &addr.sin_addr, ip_str, INET_ADDRSTRLEN);
            printf("DNS request for domain: %s\n", ip_str);
        }
    }

    return sockfd;
}

