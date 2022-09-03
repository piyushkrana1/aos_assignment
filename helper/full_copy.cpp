#include<stdlib.h>
#include<bits/stdc++.h>
#include<dirent.h>
#include<string.h>
#include<algorithm>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<termio.h>
#include<pwd.h>
#include <grp.h>
using namespace std;
int x,y,fsize;

char *curr_dir=NULL;
char *home=NULL;
// vector<string>files;
vector<pair<string,string>>files;
pair<string,string>p;

stack<string>backwards;
stack<string>forwards;

void cursor_print(int x,int y){

    cout<<"\033["<<y<<";"<<x<<"H";
    // fflush(stdout);
}


int printdata(const char *curr_dir){
    // vector<string>files;
    DIR *dp=NULL;
    struct dirent *dptr=NULL;
    struct stat fileStat;
    struct passwd *pw;
    struct group *gp;
    char *ct;
    if(curr_dir==NULL){
        printf("\n error");
        return -1;
    }
    dp=opendir((const char*)curr_dir);
    if(dp==NULL){return -1;}
    for(auto count=0;(dptr=readdir(dp))!=NULL;count++){
        p.first=dptr->d_name;
        p.second="";
        files.push_back(p);
    }
    // cout<<endl;
    sort(files.begin(),files.end());
    dp=opendir((const char*)curr_dir);
    string s;
    for(auto c=0;(dptr=readdir(dp))!=NULL;c++){
        s=(string)curr_dir+"/"+files[c].first;
        stat(s.c_str(),&fileStat);
        printf((S_ISDIR(fileStat.st_mode))?"d" :"-");
        if((S_ISDIR(fileStat.st_mode))){
            files[c].second="d";
        }
        printf( (fileStat.st_mode&S_IRUSR)?"r" :"-");
        printf( (fileStat.st_mode&S_IWUSR)?"w" :"-");
        printf( (fileStat.st_mode&S_IXUSR)?"x" :"-");
        printf( (fileStat.st_mode&S_IRGRP)?"r" :"-");
        printf( (fileStat.st_mode&S_IWGRP)?"w" :"-");
        printf( (fileStat.st_mode&S_IXGRP)?"x" :"-");
        printf( (fileStat.st_mode&S_IROTH)?"r" :"-");
        printf( (fileStat.st_mode&S_IWOTH)?"w" :"-");
        printf( (fileStat.st_mode&S_IXOTH)?"x" :"-");
        pw=getpwuid(fileStat.st_uid);
        printf("\t%s",pw->pw_name);
        gp=getgrgid(fileStat.st_gid);
        printf("\t%s",gp->gr_name);
        // // stat(files[c].c_str(),&fileStat);
        printf("\t %.2fK\t",(float)fileStat.st_size/1024);
        ct=ctime(&fileStat.st_mtime);
        for(int i=4;i<=15;i++){
            printf("%c",ct[i]);
        }

        printf("\t %s\n",files[c].first.c_str());

        // printf("\n");
        fsize=files.size();
}
return 0;
}    

struct termios original_param; 
void disable_normal_mode(){
    tcsetattr(STDIN_FILENO,TCSAFLUSH,&original_param);
}

void enable_normal_mode(){
    tcgetattr(STDIN_FILENO,&original_param);
    atexit(disable_normal_mode);
    struct termios normal_param=original_param;
    normal_param.c_lflag&=~(ICANON|ECHO);
    // normal_param.c_lflag&=~(ICANON|ECHO|ISIG|IEXTEN);
    // normal_param.c_lflag&=~(BRKINT|ICRNL|INPCK|ISTRIP|IXON);
    // normal_param.c_lflag&=~(OPOST);
    tcsetattr(STDIN_FILENO,TCSAFLUSH,&normal_param);
}

void down_cursor(){
    if(y<fsize){
    cursor_print(x,++y);
    }
}

void up_cursor(){
    if(y>0){
    cursor_print(x,--y);
    }
}

void clean(){
    printf("\033[H\033[2J\033[3J");
}

void go_home()
{
    if(curr_dir!=home){
        backwards.push(curr_dir);
        files.clear();
        clean();
        strcpy(curr_dir,home);
        printdata(home);
        x=1;y=1;
        cursor_print(x,y);
    }
}

void up_one(){



}

void next_dir(){
    if(!forwards.empty()){
        auto var=forwards.top();
        clean();
        files.clear();
        forwards.pop();
        backwards.push(curr_dir);
        strcpy(curr_dir,var.c_str());
        printdata(curr_dir);
        x=1;y=1;
        cursor_print(x,y);
    }

}


void prev_dir(){
    if(!backwards.empty()){
        auto var=backwards.top();
        clean();
        files.clear();
        forwards.push(curr_dir);
        backwards.pop();
        strcpy(curr_dir,var.c_str());
        printdata(curr_dir);
        x=1;y=1;
        cursor_print(x,y);
    }

}

void enter_key(){

            if(files[y-1].first!="."&&files[y-1].second!="d"){
                string s=(string)curr_dir+"/"+files[y-1].first;
                pid_t baby=fork();
                if(baby==0){
                execl("/bin/sh", "sh", "-c",("xdg-open "+s).c_str(), (char *) NULL);
                }
            }
            
            if(files[y-1].first!="."&&files[y-1].second=="d"){
            backwards.push(string(curr_dir));
            string pcon=string(curr_dir)+"/"+files[y-1].first;
            // strcat(pcon,string(curr_dir));
            // strcat(pcon,"/");
            // strcat(pcon,files[y-1].first);
            char *stoc=new char[pcon.length()+1];
            strcpy(stoc,pcon.c_str());

            strcpy(curr_dir,stoc);
            clean();
            // cout<<backwards.top();
            files.clear();
            printdata(curr_dir);
            x=1;y=1;
            cursor_print(x,y);
            }
}
int main()
{   x=1,y=1;
    curr_dir=get_current_dir_name();
    home=get_current_dir_name();

    clean();
    enable_normal_mode();
    printdata(curr_dir);
    // files.clear();
    cursor_print(x,y);
    char ch;
    while(ch=getchar()){
        if(ch==10){
            enter_key();
        }
        if(ch==127){
            up_one();
        }
        if(ch=='q'){
            exit(1);
        }
        else if(ch='\x1b'){
            ch=getchar();
            if(ch=='['){
                ch=getchar();
                if(ch=='B'){
                    down_cursor();
                }
                if(ch=='A'){
                    up_cursor();
                }
                if(ch=='D'){
                    prev_dir();
                }
                if(ch=='C'){
                    next_dir();
                }
                if(ch=='H'){
                    go_home();
                }
            }
        }
    }
    // clean();
    return 0;
}