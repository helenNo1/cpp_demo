#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

typedef void (*sighandler_t)(int);

// 回调函数 arg是信号的值
void signalHandler(int arg)
{
    // 没有收到信号，所以这个不会执行
    printf("接收到信号!\n");
}

int main(void)
{
    sighandler_t fun; // 函数指针

    // 捕获信号，有对应的信号过来，执行回调函数  ，这个是非阻塞函数
    fun = signal(SIGINT, signalHandler);
    if (fun == SIG_ERR)
    {
        printf("signal failure\n");
    }

    while (1)
    {
        sleep(1);
        printf("hello world\n");
        raise(SIGINT);
    }

    return 0;
}