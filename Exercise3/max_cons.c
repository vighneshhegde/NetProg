#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    char *ptr, **pptr, url[100], address[100];
    char str[INET_ADDRSTRLEN];
    struct hostent *hptr;
    const char* ip;
    printf("Enter Hostname:");
    // while ((fgets(url,50,stdin)), !feof(stdin))
    fgets(url,100,stdin);
    url[strlen(url)-1]='\0';
    {
        ptr = url;
        if ((hptr = gethostbyname(ptr)) == NULL)
        {
            printf("gethostbyname() did not find a hostname\n");
            exit(1);
        }

        strcpy(url, hptr->h_name);
        printf("official hostname:%s\n", hptr->h_name);
        for (pptr = hptr->h_aliases; *pptr != NULL; pptr++)
            printf("alias:%s\n", *pptr);
        switch (hptr->h_addrtype)
        {
        case AF_INET:
            pptr = hptr->h_addr_list;
            ip = inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str));
            for (; *pptr != NULL; pptr++)
            {
                strcpy(address, inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
                printf("address:%s\n", inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
            }
            break;
        default:
            printf("wrong type of address.\n");
            exit(0);
        }
    }
    int	sd;
	struct	sockaddr_in server;

	server.sin_family = AF_INET;
	server.sin_addr.s_addr=inet_addr(ip);
	inet_aton(ip,&server.sin_addr);
	inet_pton(AF_INET, ip, &server.sin_addr);
	server.sin_port = htons(80);

    int i;
    for(i=0;i<1100;i++){
        sd = socket (AF_INET,SOCK_STREAM,0);
        if(connect(sd, (struct sockaddr*) &server, sizeof(server))!=0){
            perror("connect()");
            //exit(1);
        }
        printf("%d\n",i);

    }
    printf("%d\n",i);
    return 0;
}
