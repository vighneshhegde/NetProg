#include<stdio.h>

int main(){
int p[2];
pipe(p);
if(fork()==0){
					close(p[0]);
					dup2(p[1],1);
					execlp("wc","wc","-l","pid.txt",(char*)0);
				}
				else{
					int wc;
					char inbuf[10];
					//close(p[1]);
					read(p[0],inbuf,6);
					//write(0,inbuf,4);
		//sscanf(inbuf,"%d ",&wc);
					printf("%d",atoi(inbuf));
}
}
