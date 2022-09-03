#include<bits/stdc++.h>
using namespace std;

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


int main()
{  
    string s="./temp.txt";
    // int n=s.length();
    // char oldf[]="./temp.txt";
    // char oldf[n]=(s.beg);
    char newf[]="p.txt";
    char buf[255];
    int value;
    char *res=realpath(s.c_str(),buf);
    string pathvar=path_removed(string(res));
    string new_path=pathvar+"/"+newf;
    rename(s.c_str(),new_path.c_str());
    cout<<endl;
    // int var
    // string s=string(res);

    // cout<<string(res)<<endl;
    // value=rename(oldf,newf);
    // cout<<value<<endl;
}