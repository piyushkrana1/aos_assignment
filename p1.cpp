#include<stdlib.h>
#include<bits/stdc++.h>

#include<dirent.h>
#include<string.h>
#include<algorithm>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
using namespace std;

int main()
{   
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
        // printf("\n");
    }
    return 0;
}