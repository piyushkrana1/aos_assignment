#include<bits/stdc++.h>
#include <netinet/in.h>
#include <stdio.h>
#include<pthread.h>
#include<thread>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include<ctime>


//definations

#define DTTMFMT "%Y-%m-%d %H:%M:%S "
#define DTTMSZ 21
#define tracker1_port 9000
using namespace std;

static char *getDtTm (char *buff) {
    time_t t = time (0);
    strftime (buff, DTTMSZ, DTTMFMT, localtime (&t));
    return buff;
}
void logwriter(const string &s){
    FILE *fp;
    char dt[DTTMSZ];
    string filename="logfile.txt";
    fp=fopen(filename.c_str(),"at");
    getDtTm(dt);
    fprintf(fp,dt);
    fprintf(fp,s.c_str());
    fclose(fp);
}
unordered_map<string,string>user_password;


vector<string> stringtovectorofstring(string str){
    vector<string>vecstr;
    string temp="";

    for(int i=0;i<str.size();i++){
        if(str[i]!=' '){
            temp=temp+str[i];
        }
        else{
            vecstr.push_back(temp);
            temp="";
        }
    }
    vecstr.push_back(temp);
    return vecstr;
}

void create_user_funct(string name,string password){
    user_password[name]=password;
    cout<<"user successfully created\n";
}
int login(string name,string password){
    if(user_password.find(name)!=user_password.end()){
        if(password!=user_password[name]){
            cout<<"wrong password\n";
            return 0;
        }
        else{
            cout<<"Login successfully\n";
            return 1;
        }
    }
    else{
        cout<<"user not found\n";
        return 0;
    }

}

void funct_clients(int acceptvar){
    vector<string>commands;
    if(acceptvar<0){
        perror("cant accpt");
    }
    while(true){
        char buffer[1000];
        int valread=read(acceptvar,buffer,1000);
        printf("%s\n",buffer);
        commands=stringtovectorofstring(string(buffer));
        // for(auto i=0;i<commands.size();i++){
        //     cout<<i<<" "<<commands[i]<<endl;
        // }
        
        if(commands[0]=="create_user"){
            if(commands.size()<3){cout<<"wrong command Again\n";}
            create_user_funct(commands[1],commands[2]);
            // commands.clear();
        }
        if(commands[1]=="login"){
            if(commands.size()<3){cout<<"wrong command Again\n";}
            login(commands[1],commands[2]);
            // commands.clear();
        }
        // commands.clear();
    }
}

int main(){
    // string messages="log file successfully created";
    // logwriter(messages);
   
    struct  sockaddr_in addr;
    int addrlen=sizeof(addr);
    char buffer[1024];
    int setopt=1;
    // char *message="acknowledgement";

    //make socket
   int tracker_fd=socket(AF_INET,SOCK_STREAM,0);
    if(tracker_fd<0){
        perror("socket build failed");
        exit(EXIT_FAILURE);
    }

    //attaching or saving socket; forcefully
    if(setsockopt(tracker_fd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&setopt,sizeof(setopt))){
        perror("socket not set");
        exit(EXIT_FAILURE);
    }
    logwriter("thread created");
    addr.sin_addr.s_addr=INADDR_ANY;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(tracker1_port);

    //bind to address

    int bindvar=bind(tracker_fd,(struct sockaddr*)&addr,sizeof(addr));
    if(bindvar<0){
        perror("binding failed");
    }   

    //listen with backlog =10
    int backlog=10;

    int listenvar=listen(tracker_fd,backlog);
    if(listenvar<0){
        perror("cant listen");
        exit(EXIT_FAILURE);
    }

    //accepting to listen
    vector<thread>threadvec;
    while(true){
        int acceptvar=accept(tracker_fd,(struct sockaddr*)&addr,(socklen_t*)(&addrlen));
        if(acceptvar<0){
            perror("cant accpt");
            exit(EXIT_FAILURE);
        }
        thread th(funct_clients,acceptvar);
        threadvec.push_back(move(th));
        //this work to do in thread function which handles all the clients  
        // int valread=read(acceptvar,buffer,1024);
        // printf("%s\n",buffer);
        // send(acceptvar,message,strlen(message),0);
        // printf("hello sent to client\n");
        for(thread &th:threadvec){
            if(th.joinable())
                th.join();
        }

    }


    // shutdown(tracker_fd,SHUT_RDWR); used to block connection rdwr is like one way or other
    //close fully destroy the connections;
    // close(tracker_fd);  
    return 0;
}   