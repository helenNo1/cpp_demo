#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
int main()
{

    // make two process which run same
    // program after this instruction
    pid_t p = fork();
    if (p < 0)
    {
        perror("fork fail");
        _exit(1);
    }
    else if (p > 0)
    {
        printf("parent %d\n", p);
        _exit(0);
    }
    else
    {
        printf("child pid %d ppid %d\n", getpid(), getppid());
    }
    printf("Hello world!, process_id(pid) = %d \n", getpid());
    return 0;
}
