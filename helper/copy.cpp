#include<bits/stdc++.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
using namespace std;
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
int main()
{  vector<string>pathvector;
    mode_t mode=S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IROTH;
    pathvector.push_back("a");
    pathvector.push_back("random.txt");
    pathvector.push_back("./temp");
    string firstf=pathvector[1];
    string secondf=pathvector[2];
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

    cout<<pathold<<endl;
    cout<<pathnew<<endl;

    int fd_one,fd_two;
    fd_one=open(pathold.c_str(),O_RDONLY);
    if(fd_one==-1){cout<<"error"<<endl;}
    fd_two=open(full_path.c_str(),O_WRONLY);
    if(fd_two==-1){cout<<"error"<<endl;}
    char buf[512];
    while(read(fd_one,&buf,512)){
        write(fd_two,&buf,512);
    }
    return 0;


}