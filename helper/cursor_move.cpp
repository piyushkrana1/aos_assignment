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
using namespace std;
int x=1,y=0,fsize;
void cursor_print(int x,int y){
    // printf("\033[",y,x);
    cout<<"\033["<<y<<";"<<x<<"H";
    fflush(stdout);
}
int printdata(){
    vector<string>files;
    char *curr_dir=NULL;
    DIR *dp=NULL;
    struct dirent *dptr=NULL;
    struct stat fileStat;

    curr_dir=getenv("PWD");
    if(curr_dir==NULL){
        printf("\n error");
        return -1;
    }
    // cout<<endl;
    // cout<<endl;
    // cout<<endl;

    dp=opendir((const char*)curr_dir);

    if(dp==NULL){return -1;}
    for(auto count=0;(dptr=readdir(dp))!=NULL;count++){
        files.push_back(dptr->d_name);
    }
    // cout<<endl;
    sort(files.begin(),files.end());
    dp=opendir((const char*)curr_dir);
    for(auto c=0;(dptr=readdir(dp))!=NULL;c++){
        stat(files[c].c_str(),&fileStat);
        printf((S_ISDIR(fileStat.st_mode))?"d" :"-");
        printf( (fileStat.st_mode&S_IRUSR)?"r" :"-");
        printf( (fileStat.st_mode&S_IWUSR)?"w" :"-");
        printf( (fileStat.st_mode&S_IXUSR)?"x" :"-");
        printf( (fileStat.st_mode&S_IRGRP)?"r" :"-");
        printf( (fileStat.st_mode&S_IWGRP)?"w" :"-");
        printf( (fileStat.st_mode&S_IXGRP)?"x" :"-");
        printf( (fileStat.st_mode&S_IROTH)?"r" :"-");
        printf( (fileStat.st_mode&S_IWOTH)?"w" :"-");
        printf( (fileStat.st_mode&S_IXUSR)?"x" :"-");
        printf("\t %u",fileStat.st_uid);
        // stat(files[c].c_str(),&fileStat);
        printf("\t %.2fK",(float)fileStat.st_size/1024);
        printf("\t %s\n",files[c].c_str());
        fsize=files.size();
        // printf("\n");
        
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
int main()
{   
    x=1,y=0;
    clean();
    enable_normal_mode();
    printdata();
    cursor_print(x,y);
    char c;
    // while(read(STDIN_FILENO,&c,1)==1&&c!='q'){
    //     if(c=='A'){down_cursor();}
    // }
    char ch;
    while(ch=getchar()){
        // fflush(stdout);
        if(ch=='q'){exit(1);}
        else if(ch='\x1b'){
            ch=getchar();
            // fflush(stdout);
            if(ch=='['){
                ch=getchar();
                // fflush(stdout);
                if(ch=='B'){
                    down_cursor();
                }
                if(ch=='A'){
                    up_cursor();
                }
            }
        }
    }
    // while(read(STDIN_FILENO,&c,1)==1&&c!='q'){
    // // while(read(STDIN_FILENO,&c,1)==1&&c!='q');
    //     // if(c=='\x1b'){
    //         // char seq[3];
    //         // if(read(STDIN_FILENO,&seq[0],1)!=1) return '\x1b';
    //         // if(read(STDIN_FILENO,&seq[1],1)!=1) return '\x1b';
    //         // if(seq[0]=='['){
    //         switch(c){
    //             case 40:
    //                 cout<<"down";
    //                 down_cursor();
    //                 break;
    //             case 'A':
    //                 down_cursor();
    //                 break;
    //         }
    //         // }
    //     // }

    // }
    return 0;
}