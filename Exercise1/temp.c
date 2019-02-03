#include <stdio.h>

main(){
	FILE *f = fopen("hello.txt","a");
	fprintf(f,"%d\n",getpid());
}
