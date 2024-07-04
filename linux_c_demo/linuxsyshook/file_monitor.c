#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <stdarg.h>

// 重定义原始的文件操作函数
int (*original_open)(const char *pathname, int flags, ...);
ssize_t (*original_read)(int fd, void *buf, size_t count);
ssize_t (*original_write)(int fd, const void *buf, size_t count);

// 重定义open函数
int open(const char *pathname, int flags, ...) {
    // 打印文件访问信息
    printf("Opening file: %s\n", pathname);
    // 调用原始的open函数
    original_open = dlsym(RTLD_NEXT, "open");
    return original_open(pathname, flags);
}

// 重定义read函数
ssize_t read(int fd, void *buf, size_t count) {
    // 打印文件读取信息
    printf("Reading from file descriptor %d\n", fd);
    // 调用原始的read函数
    original_read = dlsym(RTLD_NEXT, "read");
    return original_read(fd, buf, count);
}

// 重定义write函数
ssize_t write(int fd, const void *buf, size_t count) {
    // 打印文件写入信息
    printf("Writing to file descriptor %d\n", fd);
    // 调用原始的write函数
    original_write = dlsym(RTLD_NEXT, "write");
    return original_write(fd, buf, count);
}

