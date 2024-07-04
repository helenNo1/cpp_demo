#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>

// 重定义原始的进程创建函数
pid_t (*original_fork)(void);
int (*original_execvp)(const char *file, char *const argv[]);

// 重定义fork函数
pid_t fork(void) {
    // 打印进程创建信息
    printf("Forking a new process\n");
    // 调用原始的fork函数
    original_fork = dlsym(RTLD_NEXT, "fork");
    return original_fork();
}

// 重定义execvp函数
int execvp(const char *file, char *const argv[]) {
    // 打印进程创建信息
    printf("Executing new process: %s\n", file);
    // 调用原始的execvp函数
    original_execvp = dlsym(RTLD_NEXT, "execvp");
    return original_execvp(file, argv);
}

