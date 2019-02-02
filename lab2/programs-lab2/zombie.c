#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int
main()
{
pid_t pid;

pid=fork();

if(pid<0)
        perror("fork:");
else if(pid==0){

        printf("In Child: pid = %d, parent pid= %d", getpid(), getppid());
        printf("child finishing...\n");
        exit(0);
}
else if (pid>0){

	for(int i=0;i<10;i++){
	pid=fork();

	if(pid<0)
        	perror("fork:");
	else if(pid==0){
		printf("In Child: pid = %d, parent pid= %d", getpid(), getppid());
        	printf("child finishing...\n");
       		exit(0);
	}	
}

while(1) wait(NULL);
}

}
