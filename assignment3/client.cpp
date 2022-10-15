#include<bits/stdc++.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>
#include<pthread.h>

// using namespace std;

char peername[50];
int myport;
void * listening_func(void *peer_fd);
void sending_function();
void listening(int listen_socket_fd);

void sending_function(){

    char buffer[1500];
    int port;
    printf("port number to send the message:");
    // port=9000;
    scanf("%d",&port);
    int p_s_fd;
    p_s_fd=socket(AF_INET,SOCK_STREAM,0);
    if(p_s_fd<0){
        perror("cant make a new socket\n");
    }

    struct sockaddr_in send_addr;
    send_addr.sin_family=AF_INET;
    send_addr.sin_addr.s_addr=INADDR_ANY;
    send_addr.sin_port=htons(port);
    
    int connectvar=connect(p_s_fd,(struct sockaddr *)&send_addr,sizeof(send_addr));
    if(connectvar<0){
        perror("cant connect");
        return;
    }
    
    //sending message with data of port and peer;
    char buffer2[1000];
    char message;
    printf("enter the Command\n");
    scanf("%c",&message);
    scanf("%[^\n]s",buffer2);
    sprintf(buffer,"%s",buffer2);
    send(p_s_fd,buffer,sizeof(buffer),0);
    printf("\ndata sent\n");
    close(p_s_fd);

}

void * listening_func(void *peer_fd){

    int s_fd = *((int *)peer_fd);
    while (1)
    {
        sleep(2);
        listening(s_fd);
    }
}

//Receiving messages on our port
void listening(int listen_socket_fd)
{
    struct sockaddr_in addr;
    int addrlen=sizeof(addr);
    char buffer[1024];
    int setopt=1;
    char *message="acknowledgement";
    int acceptvar=accept(listen_socket_fd,(struct sockaddr*)&addr,(socklen_t*)(&addrlen));
    if(acceptvar<0){
        perror("cant accpt");
        exit(EXIT_FAILURE);
    }
    int valread=read(acceptvar,buffer,1024);
    printf("%s\n",buffer);
    send(acceptvar,message,strlen(message),0);
}




int main(int argc,char const *argv)
{
    printf("Enter a name for your peer : ");
    scanf("%s",peername);
    // peername="client1";

    printf("Enter a portnumber you want to dedicate to the peer: ");
    scanf("%d",&myport);
    // myport=8089;

    //buiilding a socket for comm
    int peer_fd=socket(AF_INET,SOCK_STREAM,0);
    if(peer_fd<0){
        perror("error failed");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in address;
    address.sin_family=AF_INET;
    address.sin_addr.s_addr=INADDR_ANY;
    address.sin_port=htons(myport);

    printf("peers ip adress is %s\n",inet_ntoa(address.sin_addr));
    printf("my port adress is :%d\n ",(int)ntohs(address.sin_port));

    int bindvar=bind(peer_fd,(struct sockaddr*)&address,sizeof(address));
    if(bindvar<0){
        perror("cant bind the port :Port not available");
        exit(EXIT_FAILURE);
    }

    //listening
    int backlog=10;
    int listenvar=listen(peer_fd,backlog);
    if(listenvar<0){
        perror("Error in Listening at this port");
        exit(EXIT_FAILURE);
    }

    //now doing main functionality accept and send inside threading

    pthread_t newthread;
    pthread_create(&newthread,NULL,&listening_func,&peer_fd);//always in listening state in back we can say
    
    // int choice;
    // printf("Enter the commands:\n");
    
    while (true)
    {  
        sleep(1);
       sending_function();
    }

    close(peer_fd);
    return 0;

}