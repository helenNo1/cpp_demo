#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
    pid_t pid1 = fork();
    if (!pid1)
    { // 子1
        printf("child1  %d start running!\n", getpid());
        sleep(1);
        printf("child1 exit!\n");
        exit(10);
    }

    pid_t pid2 = fork();
    if (!pid2)
    { // 子2
        printf("child2  %d start running!\n", getpid());
        while (1)
            ;
        printf("child2 exit!\n");
        exit(30);
    }

    pid_t pid = 0;
    int status = 0;
    printf("等待p1 结束%d\n", getpid());
    pid = waitpid(pid1, &status, 0);
    printf("%d %d", pid1, pid);
    if (pid == 0)
    {
        printf("没等到p1\n");
    }
    printf("p1 结束\n");
    if (WIFEXITED(status))
    {
        printf("%d正常退出 退出码%d\n", pid1, WEXITSTATUS(status));
    }
    if (WIFSIGNALED(status))
    {
        printf("%d信号中断 退出码%d\n", pid1, WTERMSIG(status));
    }
}