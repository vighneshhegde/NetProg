#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include<stdio.h>

char *env_init[] = { "USER=unknown", "PATH=/tmp", NULL };

int
main (void)
{
  pid_t pid;

  if ((pid = fork ()) < 0)
    {
      perror ("fork error");
    }
  else if (pid == 0)
    {                           /* specify pathname, specify environment */
     printf("First child:\n");
      if (execle ("/home/user/Documents/NetProg/lab2/programs-lab2/echoall", "echoall", "myarg1",
                  "MY ARG2", (char *) 0, env_init) < 0)
        perror ("execle error");
        
    }
   
  if (waitpid (pid, NULL, 0) < 0)
    perror ("wait error");

  if ((pid = fork ()) < 0)
    {
      perror ("fork error");
    }
  else if (pid == 0)
    {                           /* specify filename, inherit environment */
     printf("Second child:\n");
     //char *path[] = {"PATH =/home/user/Documents/NetProg/lab2/programs-lab2",NULL};
      if (execlp ("echoall", "echoall", "only 1 arg", (char *) 0 ) < 0)
        perror ("execlp error");
    }

  exit (0);
}
