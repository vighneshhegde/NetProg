#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>


char* getFileName(char* url){
    char* save;
    char* fname = strtok(url,"/");
    // printf("%s\n",save);
    while(fname!=NULL){
        save = fname;
        fname = strtok(NULL,"/");
    }
    // fname = strtok(NULL,"");
    return save;
}


int main(int argc, char** argv)
{
    char *ptr, **pptr, url[100], address[100];
    char str[INET_ADDRSTRLEN];
    struct hostent *hptr;
    char* ip, *restOfUrl,*fname;
    printf("Enter Hostname:");
    // while ((fgets(url,50,stdin)), !feof(stdin))
    fgets(url,100,stdin);
    url[strlen(url)-1]='\0';
    {
        ptr = strtok(url,"/");
        if(strcmp(ptr,"http:")==0||strcmp(ptr,"https:")==0){
            ptr = strtok(NULL,"/");
            restOfUrl = strtok(NULL,"");
            // ptr = strtok(NULL,&dlim);
        }
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
	
	sd = socket (AF_INET,SOCK_STREAM,0);

	server.sin_family = AF_INET;
	server.sin_addr.s_addr=inet_addr(ip);
	inet_aton(ip,&server.sin_addr);
	inet_pton(AF_INET, ip, &server.sin_addr);
	server.sin_port = htons(80);

	if(connect(sd, (struct sockaddr*) &server, sizeof(server))!=0){
        perror("connect(): ");
        exit(1);
    }
    // printf("Connected to %d\n",sd);

    char* httpreq = malloc(5000);
    // printf("%s\n",getFileName(restOfUrl));
    sprintf(httpreq, "GET /%s HTTP/1.1\r\nHost: %s\r\n\r\n", restOfUrl, url);

    if(send(sd,httpreq,strlen(httpreq),0)<0){
        perror("send(): ");
        exit(1);
    }
    // printf("sent %s\n",httpreq);

    bzero(httpreq,5000);
    size_t recvlen;

    fname = getFileName(restOfUrl);
    FILE* fp = fopen(fname,"w");
    recvlen = recv(sd,httpreq,5000,0);
    // printf("received %lu, %s\n\n",recvlen,httpreq); 
    char* len, *buf = strtok(httpreq,"\n");
    int contentLength,temp=0;
    while(strcmp(buf,"\r")!=0){
        // printf("%s\n",buf);
        temp+=strlen(buf)+1;
        len = strstr(buf,"Content-Length:");
        if(len!=NULL){
            contentLength = atoi(buf+strlen("Content-Length:"));
            // printf("%d %d\n",contentLength,recvlen);
        }
        buf = strtok(NULL,"\n");  
    }
    temp+=2;
    temp=recvlen-temp;
    strcpy(httpreq,strtok(NULL,""));
    // printf("%d %d\n",strlen(httpreq),recvlen-temp);
    while(1){
        // printf("%s",httpreq);
        fwrite(httpreq,1,temp,fp);
        // printf("written %d\n",recvlen);
        if(recvlen==5000){
            recvlen = recv(sd,httpreq,1000,0);
            temp = recvlen;
        }
        else{ 
            printf("%d\n",recvlen);
            break;
        
        }
    }

    close(sd);
    fclose(fp);
    exit(0);
}