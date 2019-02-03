#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

void sigusr(int signo){
	
}
int main ()
{
  int i = 0, j = 0; 
  pid_t ret;
  int status;

signal(SIGUSR1, sigusr);

sigset_t mask;
sigset_t omask;

sigemptyset (&mask);
sigaddset (&mask, SIGUSR1);

sigprocmask(SIG_BLOCK,&mask,&omask);

  ret = fork ();
  if (ret == 0)
    {
      for (i = 0; i < 5000; i++){
    	
        printf ("Child: %d\n", i);
		kill(getppid(),SIGUSR1);
		//pause();	
		sigsuspend(&omask);
//		sigprocmask(SIG_UNBLOCK,&mask,&omask);    
	}
      printf ("Child ends\n");
		//kill(getppid(),SIGUSR1);
    }
  else
    {
      //pause();
      //printf ("Parent resumes.\n");
      for (j = 0; j < 5000; j++){
    	//sigprocmask(SIG_BLOCK,&mask,&omask);    
		sigsuspend(&omask);
		printf ("Parent: %d\n", j);
		kill(ret,SIGUSR1);
		//pause();
		
//		sigprocmask(SIG_UNBLOCK,&mask,&omask);    
		}
	printf ("Parent ends.\n");
    }
}
