#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/wait.h>
int main(int argc, char *argv[], char *env[])
{
    printf("%d\n", getpid());
    pid_t pid = vfork();
    if (!pid)
    {
        printf("child pid %d\n", getpid());
        int res = execl("/usr/bin/ls", "/usr/bin/ls", NULL);
        if (res == -1)
        {
            perror("execl");
        }
        exit(0);
    }
    else
    {
        wait(NULL);
        printf("%d  end \n", getpid());
    }

    // sleep(1);
    return 0;
}