#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/wait.h>

int main()
{
    pid_t pid, childpid;
    int i, status, exitcode;
    for (i = 0; i < 10; i++)
    {
        pid = fork();
        if (pid == 0 || pid == -1)
        {
            break;
        }
        else
        {

            childpid = waitpid(pid, &status, 0);
            exitcode = WEXITSTATUS(status);
            printf("pid %d exit code %d\n", childpid, exitcode);
        }
    }
    if (pid == -1)
    {
    }
    else if (pid == 0)
    {
        printf("child %d\n", getpid());
        exit(100);
    }
    else
    {
        printf("parent %d\n", getpid());
    }
}