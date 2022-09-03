#include<bits/stdc++.h>
#include<fcntl.h>
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
    pathvector.push_back("a");
    pathvector.push_back("./temp/ls.cpp");
    pathvector.push_back("./temp/rename.cpp");
    pathvector.push_back("./temp/np_dir.cpp");
    pathvector.push_back("./temp/vector.cpp");
    // pathvector.push_back("cmode_enter.cpp");
    // pathvector.push_back("./temp/ls.cpp");
    pathvector.push_back(".");
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
        // cout<<pathold<<endl;
        // cout<<pathold<<endl;
        // string old_path=pathvar+"/"+secondf;
        string new_path=pathnew+"/"+pathvar;
        // cout<<new_path<<endl;
        rename(pathold.c_str(),new_path.c_str());
    }
//create dir
return 0;
}