#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>

#define MSGSIZE 20

int main ()
{
  int i;
  char *msg = "How are you?";

  char inbuff[MSGSIZE];
  int p1[2],p2[2],p3[2],p4[2],p5[2],p6[2];
  pid_t ret;
  pipe (p1);
  pipe (p2);
  pipe (p3);
  pipe (p4);
  pipe (p5);
  pipe (p6);

  if (fork ()== 0){//C1
  	read (p1[0], inbuff, MSGSIZE);
  	int i;
  	for(i=0;inbuff[i];i++) inbuff[i]=toupper(inbuff[i]); 
	printf("Output of C1(pid=%d) is %s\n",getpid(),inbuff);
  	write (p2[1], inbuff, MSGSIZE);
	exit(1);	      
  }
  else if (fork ()== 0){//C2
	read (p2[0], inbuff, MSGSIZE);
	for(i=0;inbuff[i];i++) inbuff[i]=(inbuff[i+1]); 
	printf("Output of C2(pid=%d) is %s\n",getpid(),inbuff);
	write (p3[1], inbuff, MSGSIZE);      
  }
  else if (fork ()== 0){//C3
	read (p3[0], inbuff, MSGSIZE);
	inbuff[strlen(inbuff)-1]='\0';
  	write (p4[1], inbuff, MSGSIZE);
	printf("Output of C3(pid=%d) is %s\n",getpid(),inbuff);	      
  }
  else if (fork ()== 0){//C4
	read (p4[0], inbuff, MSGSIZE);
	for(i=0;inbuff[i];i++) inbuff[i]=(inbuff[i+1]); 	
	printf("Output of C4(pid=%d) is %s\n",getpid(),inbuff);
  	write (p5[1], inbuff, MSGSIZE);     
  }
  else if (fork ()== 0){//C5
	read (p5[0], inbuff, MSGSIZE);
  	inbuff[strlen(inbuff)-1]='\0';
	printf("Output of C5(pid=%d) is %s\n",getpid(),inbuff);	      
  	write (p6[1], inbuff, MSGSIZE);

	
  }
  else//parent
  {
    printf("Enter text:\n");
    scanf("%[^\n]s",inbuff);
    write (p1[1], inbuff,MSGSIZE);
    read (p6[0], inbuff, MSGSIZE);
    printf("Output of Parent: %s\n", inbuff);
    wait(NULL);
  }
 
}
