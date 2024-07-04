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
        if (pid == 0)
        {
            printf("child do something %d \n", getpid());
            sleep(10);
            return 0;
        }
        childpid = waitpid(pid, &status, 0);
        exitcode = WEXITSTATUS(status);
        printf("pid %d exitcode %d\n", childpid, exitcode);
        printf("this is parent pid %d\n", getpid());
    }
    return 0;
}