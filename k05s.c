/*** TCP/IP server program ***/
/* Usage: ./server HostName */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
 
#define PORT 50001
 
char buf[]="Sample message.\n";
 
int main(int argc, char *argv[])
{
    struct sockaddr_in ServerAddr;
    struct sockaddr_in ClientAddr;
    struct hostent *HostInfo;
    int ServerSessionID;
    int ClientSessionID;
    int clientlength;
  
    if (argc!=2){
        fprintf(stderr,"Usage: ./server HostName!\n");
        exit(1);
    }
  
    if ((ServerSessionID=socket(AF_INET,SOCK_STREAM,0))==-1){
        fprintf(stderr, "Failed to make a socket!\n");
        exit(1);
    }
  
    if ((HostInfo=gethostbyname(argv[1]))==NULL){
        fprintf(stderr,"Failed to find host.\n");
        exit(1);
    }
  
    ServerAddr.sin_family=AF_INET;
    ServerAddr.sin_port=htons(PORT);
    memcpy((char *)&ServerAddr.sin_addr,(char *)HostInfo->h_addr_list[0],
                                                       HostInfo->h_length);
    if (bind(ServerSessionID,(struct sockaddr *)&ServerAddr,
                                                  sizeof(ServerAddr))==-1){
        fprintf(stderr,"Failed to assign a socketname.\n");
        exit(1);
    }
  
    if (listen(ServerSessionID,5)==-1){
        fprintf(stderr,"Failed to listen.\n");
        exit(1);
    }
  
    while(1){
        clientlength=sizeof(ClientAddr);
        if ((ClientSessionID=accept(ServerSessionID,
          (struct sockaddr *)&ClientAddr,(socklen_t *)&clientlength))==-1){
            fprintf(stderr,"Failed to accept.\n");
            exit(1);
        }
    
        if (send(ClientSessionID,buf,sizeof(buf),0)==-1){
            fprintf(stderr,"Failed to send a message.\n");
            exit(1);
        }
    
        if (shutdown(ClientSessionID,2)==-1){
            fprintf(stderr,"Failed to shutdown.\n");
            exit(1);
        }
        close(ClientSessionID);
    }
}
