#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>
#include <sys/socket.h>

ssize_t send(int sockfd, const void *buf, size_t len, int flags) {
    // 检查发送请求是否符合内容过滤规则
    if (contains_sensitive_data(buf, len)) {
        printf("Send request denied: Content filtering rule\n");
        return -1;
    }

    // 调用原始的send函数
    ssize_t (*original_send)(int, const void *, size_t, int);
    original_send = dlsym(RTLD_NEXT, "send");
    return original_send(sockfd, buf, len, flags);
}

