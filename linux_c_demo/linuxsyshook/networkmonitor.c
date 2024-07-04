#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

// 重定义原始的网络通信函数
int (*original_socket)(int domain, int type, int protocol);
int (*original_connect)(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
ssize_t (*original_send)(int sockfd, const void *buf, size_t len, int flags);


int socket(int domain, int type, int protocol){
 // 打印网络通信信息
    printf("Creating socket: domain=%d, type=%d, protocol=%d\n", domain, type, protocol);
    // 调用原始的socket函数
    original_socket = dlsym(RTLD_NEXT, "socket");
    return original_socket(domain, type, protocol);
	
}


// 重定义connect函数
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    // 获取目标主机和端口
    struct sockaddr_in *addr_in = (struct sockaddr_in *)addr;
    char *ip = inet_ntoa(addr_in->sin_addr);
    int port = ntohs(addr_in->sin_port);

    // 打印网络通信信息
    printf("Connecting to %s:%d\n", ip, port);
    // 调用原始的connect函数
    original_connect = dlsym(RTLD_NEXT, "connect");
    return original_connect(sockfd, addr, addrlen);
}




// 重定义send函数
ssize_t send(int sockfd, const void *buf, size_t len, int flags) {
    // 打印网络通信信息
    printf("Sending data through socket descriptor %d\n", sockfd);
    // 调用原始的send函数
    original_send = dlsym(RTLD_NEXT, "send");
    return original_send(sockfd, buf, len, flags);
}

