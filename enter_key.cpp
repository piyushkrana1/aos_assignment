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
// vector<string>files;
vector<pair<string,string>>files;
pair<string,string>p;
void cursor_print(int x,int y){
    // printf("\033[",y,x);
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

void enter_key(){
            // cout<<"\t\t\t\t\t\t"<<""
// //             // cout<<"enter";
            // cout<<"\t\t"<<files[y-1];
            curr_dir=getenv("PWD");
            // struct stat fstattemp;
            // string s=(string)curr_dir+"/"+files[y-1];
            // cout<<s;
            // stat(files[y-1].c_str(),&fstattemp);
            // if(!S_ISDIR(fstattemp.st_mode)){
            //     execl("/bin/sh", "sh", "-c",xdg-open, (char *) NULL);
            // }
// //             char cwd[256];
// //             getcwd(cwd,sizeof(cwd));

            //For files
            if(files[y-1].first!="."&&files[y-1].second!="d"){
                string s=(string)curr_dir+"/"+files[y-1].first;
                pid_t baby=fork();
                if(baby==0){
                execl("/bin/sh", "sh", "-c",("xdg-open "+s).c_str(), (char *) NULL);
                }
            }



            //for directories
            if(files[y-1].first!="."&&files[y-1].second=="d"){
            curr_dir=strcat(curr_dir,"/");
            curr_dir=strcat(curr_dir,files[y-1].first.c_str());
//         //    cout<<"\t\t"<<curr_dir;
            clean();
            files.clear();
            printdata(curr_dir);
            x=1,y=1;
            cursor_print(x,y);
            }
}



int main()
{   x=1,y=1;
    curr_dir=getenv("PWD");
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
        if(ch=='q'){exit(1);}
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
            }
        }
    }
    // clean();
    return 0;
}