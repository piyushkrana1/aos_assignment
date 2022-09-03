#include<bits/stdc++.h>
using namespace std;
int main(){

    string s="command firstpath secondpath thirdpath";
    vector<string>vofpath;
    // s=s.substr(0,s.length()-1);
    // s=s.substr(0,s.length()-1);
    cout<<s<<endl;
    string temp="";
    for(auto &i:s){
        if(i==' '){
            vofpath.push_back(temp);
            temp="";
        }
        else{
            temp=temp+i;
        }

    }
    vofpath.push_back(temp);
    for(auto &i:vofpath){
        cout<<i<<endl;
    }

    return 0;
}