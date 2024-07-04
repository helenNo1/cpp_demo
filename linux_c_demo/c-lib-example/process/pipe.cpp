#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int fd[2];
    pipe(fd);
    pid_t pid = fork();
    if (!pid)
    {
        close(fd[1]);
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
    }
    else if (pid > 0)
    {
        close(fd[0]);
        char buf[256] = {0};
        int num = 0;
        while (1)
        {
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "data from parent %d\n", num++);
            write(fd[1], buf, sizeof(buf));
            sleep(1);
        }
    }

    return 0;
}