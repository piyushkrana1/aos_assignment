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
#include<sys/ioctl.h>
#include<fcntl.h>
using namespace std;
int x,y,fsize;
int mode_flag=0;
char *curr_dir=NULL;
char *home=NULL;

vector<pair<string,string>>files;
pair<string,string>p;

stack<string>backwards;
stack<string>forwards;

string vcommand;
vector<string>pathvector;

struct winsize w;



void cursor_print(int x,int y){
    // printf("\033[",y,x);
    cout<<"\033["<<y<<";"<<x<<"H";
    // fflush(stdout);
}

void lineclear(){
    cursor_print(1,29);
    cout<<"\033[0K";
}

void print_type(){
    cursor_print(1,28);
    if(mode_flag==0){
        cout<<"NORMAL MODE";
        
    }
    if(mode_flag==1){
        cout<<"COMMAND MODE";
    }
    cursor_print(1,1);
}

int printdata(const char *curr_dir){
    print_type();
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
    clean();
    files.clear();
    backwards.push(curr_dir);
    // forwards.push(curr_dir);
    string s=string(curr_dir);
    int var;
    for(int i=0;i<s.length();i++){
        if(s[i]=='/'){
            var=i;
        }
    }
    string a="";
    for(int i=0;i<var;i++){
        a=a+s[i];
    }
    strcpy(curr_dir,a.c_str());
    printdata(curr_dir);
    x=1;y=1;
    cursor_print(x,y);
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

            if(files[y-1].first=="..")
            {
                up_one();
            }

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


//coomand mode function

//function to remove path till last slash
string path_removed(string pathvar){
    int var=0;
    for(int i=0;i<pathvar.length();i++){
        if(pathvar[i]=='/'){
            var=i;
        }
    }
    string s="";
    for(int i=0;i<var;i++){
        s=s+pathvar[i];
    }
    return s;
}

string get_file_name(string s){
    string var="";
    for(auto &i:s){
        if(i=='/'){
            var="";
        }
        else{
            var=var+i;
        }
    }
    return var;
}

void deletef(){
    string firstf=pathvector[1];
    char buf[255];
    int value;
    char *res=realpath(firstf.c_str(),buf);
    string pathvar=string(res);
    remove(pathvar.c_str());
    clean();
    files.clear();
    printdata(curr_dir);
    x=1;y=29;
    cursor_print(x,y);
}


void copy(){

    mode_t mode=S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IROTH;
    // pathvector.push_back("a");
    // pathvector.push_back("random.txt");
    // pathvector.push_back("./temp");
    int n=pathvector.size();
    for(int i=1;i<n-1;i++){
        string firstf=pathvector[i];
        string secondf=pathvector[n-1];
        char bufo[255];
        char bufn[255];
        int value;
        char *resold=realpath(firstf.c_str(),bufo);
        char *resnew=realpath(secondf.c_str(),bufn);
        string pathold=string(resold);
        string pathnew=string(resnew);
        string pathvar=get_file_name(pathold);
        string full_path=pathnew+"/"+pathvar;
        creat(full_path.c_str(),mode);
        int fd_one,fd_two;
        fd_one=open(pathold.c_str(),O_RDONLY);
        if(fd_one==-1){cout<<"error"<<endl;}
        fd_two=open(full_path.c_str(),O_WRONLY);
        // if(fd_two==-1){cout<<"error"<<endl;}
        char buf[1024];
        while(read(fd_one,&buf,1024)){
            write(fd_two,&buf,1024);
        }
    }

    clean();
    files.clear();
    printdata(curr_dir);
    x=1;y=29;
    cursor_print(x,y);
}

void move(){
    int n=pathvector.size();
    for(int i=1;i<n-1;i++){
        string firstf=pathvector[i];
        string secondf=pathvector[n-1];
        char bufo[255];
        char bufn[255];
        int value;
        char *resold=realpath(firstf.c_str(),bufo);
        char *resnew=realpath(secondf.c_str(),bufn);
        string pathvar=get_file_name(string(resold));
        string pathold=string(resold);
        string pathnew=string(resnew);
        string new_path=pathnew+"/"+pathvar;
        rename(pathold.c_str(),new_path.c_str());

        clean();
        files.clear();
        printdata(curr_dir);
        x=1;y=29;
        cursor_print(x,y);
    }

}



void create_dir(){
    mode_t mode=S_IRUSR | S_IWUSR | S_IXUSR|S_IRGRP|S_IROTH;
    string firstf=pathvector[1];//filename
    string secondf=pathvector[2];//destination

    struct passwd *pw=getpwuid(getuid());
    const char *tilda_dir=pw->pw_dir;
    if(firstf[0]=='~'){
        firstf=string(tilda_dir)+firstf.substr(1,firstf.length()-1);
    }
    if(secondf[0]=='~'){
        secondf=string(tilda_dir)+secondf.substr(1,secondf.length()-1);
    }

    char buf[255];
    int value;
    char *res=realpath(secondf.c_str(),buf);
    string pathvar=string(res);
    string full_path=pathvar+"/"+firstf;
    mkdir(full_path.c_str(),mode);
    clean();
    files.clear();
    printdata(curr_dir);
    x=1;y=29;
    cursor_print(x,y);

}

void create_file(){
    mode_t mode=S_IRUSR|S_IWUSR | S_IXUSR|S_IRGRP|S_IROTH;

    string firstf=pathvector[1];//filename
    string secondf=pathvector[2];//destination

    struct passwd *pw=getpwuid(getuid());
    const char *tilda_dir=pw->pw_dir;
    if(firstf[0]=='~'){
        firstf=string(tilda_dir)+firstf.substr(1,firstf.length()-1);
    }
    if(secondf[0]=='~'){
        secondf=string(tilda_dir)+secondf.substr(1,secondf.length()-1);
    }
    char buf[255];
    int value;
    char *res=realpath(secondf.c_str(),buf);
    string pathvar=string(res);
    string full_path=pathvar+"/"+firstf;
    creat(full_path.c_str(),mode);
    clean();
    files.clear();
    printdata(curr_dir);
    x=1;y=29;
    cursor_print(x,y);
}


void rename_file(){

    string firstf=pathvector[1];
    string secondf=pathvector[2];

    struct passwd *pw=getpwuid(getuid());
    const char *tilda_dir=pw->pw_dir;
    if(pathvector[1][0]=='~'){
        pathvector[1]=string(tilda_dir)+pathvector[1].substr(1,pathvector[1].length()-1);
    }
    firstf=pathvector[1];
    char buf[255];
    int value;
    char *res=realpath(firstf.c_str(),buf);
    string pathvar=path_removed(string(res));
    string new_path=pathvar+"/"+secondf;
    rename(firstf.c_str(),new_path.c_str());
    clean();
    files.clear();
    printdata(curr_dir);
    x=1;y=29;
    cursor_print(x,y);

}

void pathget(){
    string temp="";
    for(auto &i:vcommand){
        if(i==' '){
            pathvector.push_back(temp);
            temp="";
        }
        else{
            temp=temp+i;
        }
    }
    pathvector.push_back(temp);
}


void fgoto(){
    char buf[256];
    struct passwd *pw=getpwuid(getuid());
    const char *tilda_dir=pw->pw_dir;
    if(pathvector[1][0]=='~'){
        pathvector[1]=string(tilda_dir)+pathvector[1].substr(1,pathvector[1].length()-1);
    }
    if(pathvector[1][0]=='.'&&pathvector[1][1]!='.'){
        pathvector[1]=string(curr_dir)+pathvector[1].substr(1,pathvector[1].length()-1);
    }
    if(pathvector[1][0]=='.'&&pathvector[1][1]=='.'){
        pathvector[1]=path_removed(curr_dir);
    }

    char *res=realpath(pathvector[1].c_str(),buf);
    if(res){
        backwards.push(string(res));
        strcpy(curr_dir,res);
        clean();
        files.clear();
        printdata(curr_dir);
        x=1;y=29;
        cursor_print(x,y);
    }
    // if(!res){
    //     cout<<"nahi chalunga";
    // }
}

void com_enter()
{   
    pathget();
    if(vcommand=="quit"){
        cout<<endl;
        exit(1);
    }
    if(pathvector[0]=="goto"){
        fgoto();
    }
    if(pathvector[0]=="rename"){
        rename_file();
    }
    if(pathvector[0]=="create_file"){
        create_file();
    }
    if(pathvector[0]=="create_dir"){
        create_dir();
    }
    if(pathvector[0]=="move"){
        move();
    }
    if(pathvector[0]=="copy"){
        copy();
    }
    if(pathvector[0]=="delete"){
        deletef();
    }
    
}
// ENTER 10
// ESCAPE 27
// BACKSPACE 127

void command(){
    mode_flag=1;
    print_type();
    x=1;y=29;
    cursor_print(x,y);
    char c;
    while(c=getchar()){
        // char c;
        // c=getchar();
        // if(vcommand=="quit"){
        //     exit(0);
        // }
        if(c==27){
            mode_flag=0;
            print_type();
            clean();
            files.clear();
            printdata(curr_dir);
            x=1;y=1;
            cursor_print(x,y);
            vcommand.clear();
            break;
        }
        else if(c!=27&&c==127){
                vcommand=vcommand.substr(0,vcommand.length()-1);
                lineclear();
                cout<<vcommand;
        }
        else if(c==10){
            com_enter();
            vcommand.clear();
            pathvector.clear();
            lineclear();
        }
        else if(c!=127&&c!=27){
            vcommand=vcommand+c;
            lineclear();
            cout<<vcommand;
        }
    }
}




int main()
{  
    x=1,y=1;
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
        if(ch==':'){
            mode_flag=1;
            command();
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