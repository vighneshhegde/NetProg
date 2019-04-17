#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "key.h"

#define MSGLEN 200
/*
1. list
2. join group number is specified


*/

struct my_msgbuf
{
  long mtype;
  char mtext[MSGLEN];
  int groupno;
  int sendernumber;
  int command;
};


int main ()
{
	struct my_msgbuf buf; 
	int msqid;
	key_t key;

	if ((key = ftok (MSGQ_PATH, 'B')) == -1)
	{
		perror ("ftok");
		exit (1);
	}

	if ((msqid = msgget (key, 0644 | IPC_CREAT)) == -1)
	{
		perror ("msgget");
		exit (1);
	}
	int cmd, state = 1;
	printf ("Server Connected. Enter command.\n"); 
	if(state == 1){
		printf("1 to list\n2 to join\n3 to create\n^D to exit:\n");
		scanf("%d",&cmd);
		if(cmd==1){
			state = 1;
			//send 1 to server
			//receive list
		}
		else if(cmd==2){
			//send 2 to server
			//receive list
			//send group number
			//receive group number back confirmation
			//state = 2
		}
		else if(cmd==3){
			printf("Enter name of new group");
			scanf("%s",&buf.mtext);
			//send buf to server
			//recieve group number back as confirmation
			//state = 2
		}
	}
	else if(state == 2){
		int pid = fork();
		if(pid==0){
			//keep receiving and printing
			while (1){
				if (msgrcv (msqid, &buf, sizeof (buf), getpid(), 0) == -1)
				{
					perror ("msgrcv");
					exit (1);
				}
				printf("%s",buf.mtext);
			}
		}
		else{
			while (fgets (buf.mtext,MESGLEN,stdin), !feof (stdin)){
			strtok(buf.mtext,"\n")
			buf.sendernumber = getppid();
			//buf.groupnumber = groupno; //insert that here
			if (msgsnd (msqid, &buf, sizeof (buf), 0) == -1)
				perror ("msgsnd");
			}
		}
	}
/*
  if (msgctl (msqid, IPC_RMID, NULL) == -1)
    {
      perror ("msgctl");
      exit (1);
    }
*/  
return 0;
}
