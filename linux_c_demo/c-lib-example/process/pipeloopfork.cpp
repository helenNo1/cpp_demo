#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

int main()
{
    int fd[2];
    int ret;
    pid_t pid;
    // 创建一个管道
    ret = pipe(fd);
    if (ret < 0)
    {
        perror("pipe error");
        return -1;
    }
    int i = 0;
    int n = 10;
    for (i = 0; i < n; i++)
    {
        // 创建子进程
        pid = fork();
        if (pid < 0)
        {
            perror("fork error");
            return -1;
        }
        else if (pid == 0)
        {
            break;
        }
    }
    if (i == n)
    { // 父进程中关闭管道，只做子进程回收
        close(fd[0]);
        close(fd[1]);
        pid_t wpid;
        int status;
        while (1)
        {
            // 等待回收子进程
            wpid = waitpid(-1, &status, WNOHANG); // 不阻塞地回收所有进程
            if (wpid == 0)                        // 没有子进程退出
            {
                sleep(1); // 可加可不加，避免大量重复循环
                continue;
            }
            else if (wpid == -1) // 已经没有子进程
            {
                printf("no child is living, wpid==[%d]\n", wpid);
                exit(0);
            }
            else if (wpid > 0)
            {
                if (WIFEXITED(status)) // 正常退出
                {
                    printf("child normal exited, status==[%d]\n", WEXITSTATUS(status));
                }
                else if (WIFSIGNALED(status)) // 被信号杀死
                {
                    printf("child killed by signo==[%d]\n", WTERMSIG(status));
                }
            }
        }
    }

    // 第一个子进程
    if (i == 0)
    {
        printf("write child: fpid==[%d], cpid==[%d]\n", getppid(), getpid());
        close(fd[0]);
        // 将标准输出重定向到管道到写端
        // dup2(fd[1], STDOUT_FILENO);
        // execlp("ps", "ps", "aux", NULL);
        // perror("execlp error");
        char buf[256] = {0};
        int num = 0;
        while (1)
        {
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "writen num %d\n", num++);
            write(fd[1], buf, sizeof(buf));
            sleep(1);
        }

        // close(fd[1]);
    }

    // 第2个子进程以后的进程
    if (i > 0 && i < n)
    {
        printf("read child: fpid==[%d], cpid==[%d]\n", getppid(), getpid());
        close(fd[1]);
        // 将标准输入重定向到管道到读端
        // dup2(fd[0], STDIN_FILENO);
        // execlp("grep", "grep", "--color", "bash", NULL);
        // perror("execlp error");
        char buf[256] = {0};
        while (1)
        {
            memset(buf, 0, sizeof(buf));
            int ret = read(fd[0], buf, sizeof(buf));
            if (ret == 0)
            {
                printf("read over \n");
                break;
            }
            else if (ret > 0)
            {
                write(STDOUT_FILENO, buf, ret);
            }
        }

        // close(fd[0]);
    }
    return 0;
}