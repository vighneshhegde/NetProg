#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

int filecount=0;

char* getFileName(char* url){
    char* save;
    char* fname = strtok(url,"/");
    // printf("%s\n",save);
    while(fname!=NULL){
        save = fname;
        fname = strtok(NULL,"/");
    }
    // fname = strtok(NULL,"");
    fname = malloc(strlen(save)+1);
    strcpy(fname,save);
    return fname;
}

char* getIP(char *ptr,char* url){
    char **pptr, address[100];
    char str[INET_ADDRSTRLEN];
    
    char* ip;
    char* ip2 = malloc(20);
    struct hostent *hptr;
    if ((hptr = gethostbyname(ptr)) == NULL){
        printf("gethostbyname() did not find a hostname\n");
        exit(1);
    }

    strcpy(url, hptr->h_name);
    // printf("official hostname:%s\n", hptr->h_name);
    // for (pptr = hptr->h_aliases; *pptr != NULL; pptr++)
    //     printf("alias:%s\n", *pptr);
    switch (hptr->h_addrtype)
    {
    case AF_INET:
        pptr = hptr->h_addr_list;
        ip = (char*)inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str));
        for (; *pptr != NULL; pptr++)
        {
            strcpy(address, inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
            // printf("address:%s\n", inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
        }
        
        strcpy(ip2,ip);
        return ip2;
    default:
        printf("wrong type of address.\n");
        exit(0);
    }

}

void* download(void* d){
    char* url = malloc(strlen(*(char**)d)+1);
    strcpy(url,*(char**)d);

    char *ptr;
    char str[INET_ADDRSTRLEN];

    char* ip;
    char*restOfUrl,*fname,*extension_temp,*extension;
    int fsize;
    // printf("Enter Hostname:");
    // while ((fgets(url,50,stdin)), !feof(stdin))
    // fgets(url,100,stdin);

    //this line to remove \n if url is obtained through fgets
    // url[strlen(url)-1]='\0';
    
    // printf("url: %s\n",url);
    ptr = strtok(url,"/");
    if(strcmp(ptr,"http:")==0||strcmp(ptr,"https:")==0){
        ptr = strtok(NULL,"/");
        restOfUrl = strtok(NULL,"");
        // ptr = strtok(NULL,&dlim);
    }
    
    ip = getIP(ptr,url);
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
    // printf("%s\n",httpreq);

    if(send(sd,httpreq,strlen(httpreq),0)<0){
        perror("send(): ");
        exit(1);
    }
    // printf("sent %s\n",httpreq);

    bzero(httpreq,5000);
    size_t recvlen;

    
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
        extension_temp = strstr(buf,"Content-Type:");
        if (extension_temp!=NULL) {
            extension = malloc(strlen(extension_temp)+1);
            strcpy(extension,extension_temp);
        }        
        buf = strtok(NULL,"\n");  
    }
    temp+=2;
    temp=recvlen-temp;
    memcpy(httpreq,strtok(NULL,""),temp);//strcpy does not copy in the beginning properly, for some reason.
    // printf("%d %d \n",contentLength,recvlen);
    
    fname = getFileName(restOfUrl);
    if (extension!=NULL) {
        extension_temp = extension;
        extension = strchr(extension,'/');
        extension++;
        // printf("ext: %s\n",extension);
        if(strchr(fname,'.')==NULL){
            extension_temp = realloc(extension_temp,strlen(extension)+strlen(fname)+20);
            sprintf(extension_temp,"%s_%d.%s",fname,++filecount,extension);
            fname = extension_temp;
        }
        else free(extension_temp);
    }
    FILE* fp = fopen(fname,"wb");//wb because we don't know what kind of file it is, it could be text
    fseek(fp,0,SEEK_END);
     
    /*Get the current position of the file pointer.*/
    while(ftell(fp)>0){//means the file already exists
        fclose(fp);
        char* newname = malloc(strlen(fname)+strlen("(copy)")+1);
        sprintf(newname,"(copy)%s",fname);
        free(fname);
        fname = newname;
        FILE* fp = fopen(fname,"wb");//wb because we don't know what kind of file it is, it could be text
        fseek(fp,0,SEEK_END);
    }
    
    
    while(1){
        // printf("%s",httpreq);
        fwrite(httpreq,temp,1,fp);
            contentLength-=temp;
        // printf("written to %s\n",fname);
        if(contentLength>0){
            bzero(httpreq,5000);
            recvlen = recv(sd,httpreq,5000,0);
            temp = recvlen;
        }
        else{ 
        
        // printf("%d\n",contentLength);
           break;
        }
    }
    printf("fetched %s\n",fname);
    pthread_exit(NULL);
    close(sd);
    fclose(fp);
    return NULL;
}


int main(int argc, char** argv)
{
    if(argc<2){
        printf("Please specify the file with url list\n");
        exit(0);
    }
    clock_t    start_time, end_time;
    double total_CPU_time;
    start_time = clock();

    pthread_attr_t attr;
    pthread_t * threads = NULL;
    int i;
    FILE* f = fopen(argv[1],"r");

    char* url = malloc(100);
    for(i=0;;i++){
        if(fgets(url,100,f)==NULL){
            // printf("fgets()\n");
            break;
        }
        // this line to remove \n if url is obtained through fgets
        threads = realloc(threads,(i+1)*sizeof(pthread_t));
        url[strlen(url)-1]='\0';
        // printf("%s\n",url);
        if(pthread_attr_init(&attr)!=0){
            perror("pthread_attr_init: ");
            exit(1);
        }
        // download(url);
        if(pthread_create(&threads[i],&attr,download,&url)!=0){
            perror("create()");
            exit(1);
        }
        
        if(pthread_attr_destroy(&attr)!=0){
            perror("pthread_attr_destroy: ");
            exit(1);
        }
        // printf("%s\n",url);
    }    
    int count = i;
    for(i=0;i<count;i++){
        pthread_join(threads[i],NULL);
    }

    end_time = clock();
    total_CPU_time  =  (double) (end_time - start_time);
    total_CPU_time =  total_CPU_time / CLOCKS_PER_SEC;
    printf("Total CPU Time: %f seconds\n",total_CPU_time);
    fclose(f);
    exit(0);
}