/* NETWORK PROGRAMMING - EXERCISE 1
Write a program signal.c taking N, K, L, and M as CLA for the following requirement.
(a) Parent process creates N processes and each of the N processes creates K child processes.
When a process is created, its pid is stored in a file. When the last process i.e. (N*K)th is
created, it will send SIGUSR1 signal to all processes indicating that they all can use the file
now.
(b) Each process selects a random number between 1 and 31 (except SIGKILL and SIGSTOp)
and sends that signal to one of the randomly selected process. Process should print signal
number and the process id to the console.
(c) Each process follows step (b) M number of times. After that it checks if the number of
signals received is less than L, then it will terminate. Else it repeats step(b) M number of
times. When it terminates, it should print a message "Process <pid> received x number of
signals. So terminating.
(d) Before beginning the loop in (c), a process which has a child will check if all of its children
exited. If yes, it will also exit printing "Process <pid> exiting becuase all its children
exited".
*/

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

int count=0;

void sigusr1_handler(int signal){}
void handler(int signal){
	count++;
	printf("Process %d received signal %d\n",getpid(),signal);
}

void randsig(int pid){
	int r = rand()%31+1;
	while(r==9||r==19){//except kill and stop signals.
		r = rand()%31+1;
	}
	kill(pid,r);
	//printf("Process %d received signal %d\n",pid,r);
}

int randpid(int total){//returns a random pid
	FILE *f = fopen("pid.txt","r");
	//int *array = malloc(total*sizeof(int));

	int p, i, random = rand()%total+1;
	for(i=0;i<random;i++){
		fscanf(f,"%d\n",&p);
	}
	fclose(f);
	return p;
}

void stepc(int m,int total){
	int i;
	for(i=0;i<m;i++){
		randsig(randpid(total));
	}

}

int main (int argc, char **argv){				//n,k,l,m
/* argv[1] = n = number of child processes of the parent
** argv[2] = k = number of child processes of each child
** argv[3] = l = threshold of termination
** argv[4] = m = number of reptitions of step (b)
*/
	
	srand(time(NULL));
	int n = 3, k = 4, l = 5, m = 2,status;
	if (argc <= 4){
		printf("Wrong number of arguments, assuming defaults: n=3,k=4,l=5,m=2\n\n");
	}
	else{
		n = atoi(argv[1]);
		k = atoi(argv[2]);
		l = atoi(argv[3]);
		m = atoi(argv[4]);
	}
	int total = 1+n+n*k;
	
	int fd = open("pid.txt",O_APPEND|O_CREAT|O_WRONLY|O_TRUNC,0666);
	close(fd);	
	FILE *f = fopen("pid.txt","a");
	
	signal(12,sigusr1_handler);
	signal(1,handler);
	signal(2,handler);
	signal(3,handler);
	signal(4,handler);
	signal(5,handler);
	signal(6,handler);
	signal(7,handler);
	signal(8,handler);
	signal(11,handler);
	signal(12,handler);
	signal(13,handler);
	signal(14,handler);
	signal(15,handler);
	signal(16,handler);
	signal(17,handler);
	signal(18,handler);
	signal(20,handler);
	signal(21,handler);
	signal(22,handler);
	signal(23,handler);
	signal(24,handler);
	signal(25,handler);
	signal(26,handler);
	signal(27,handler);
	signal(28,handler);
	signal(29,handler);
	signal(30,handler);
	signal(31,handler);

	int i, j;
	for (i = 0; i < n; i++){//CREATING N CHILDREN
		int pid = fork ();
		if (pid == 0){			//child
			for (j = 0; j < k; j++){	//CREATING K GRANDCHILDREN
				int pid2 = fork();
				if(pid2==0){
					fprintf(f,"%d\n",getpid());
					fclose(f);
					pause();
					signal(12, handler);//changing the handler because the job of SIGUSR1 is over.
					while(1){
						if(count<l)	stepc(m,total);
						else{
							printf("%d received %d signals. Now exiting.\n",getpid(),count);
							exit(0);
						}
					}
				}
				//else wait(NULL);
			}
			fprintf(f,"%d\n",getpid());
			fclose(f);
			pause();
			signal(12, handler);//changing the handler because the job of SIGUSR1 is over.
			while(1){
				if(count<l){
					stepc(m,total);
					if(waitpid(-1,&status,0)==-1){ //all children exited
						printf("Process %d exiting becuase all its children exited\n",getpid());
						break;
					}
				}
				else{
					printf("Process %d received %d signals. So terminating.\n",getpid(),count);
					break;
				}
			}
			exit(0);
		}
	}

	fprintf(f,"%d\n",getpid());
	fclose(f);

	int p[2],e;
	pipe(p);
	while(1){
		if((e=fork())==0){
			dup2(p[1],1);
			execlp("wc","wc","-l","pid.txt",(char*)0);
		}
		else{
			waitpid(e,NULL,0);
			char inbuf[10];
			read(p[0],inbuf,6);
			if(atoi(inbuf)>=total){//send a USR1 signal to all processes.
				break;
			}
		}
	}

	kill(0,12);
	//part(c) begins here
	signal(12, handler);//changing the handler because the job of SIGUSR1 is over.
	while(1){
		if(count<l){
			stepc(m,total);
			if(waitpid(-1,&status,WNOHANG)==-1){ //all children exited
				printf("Parent Process %d exiting becuase all its children exited.\n",getpid());
				break;
			}
		}
		else{
			printf("Parent process %d received %d signals. So exiting. Children will become orphaned.\n",getpid(),count);
			break;
		}
	}
	exit(0);
}
