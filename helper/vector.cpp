#include<bits/stdc++.h>
using namespace std;

int main(){



    vector<pair<string,string>>v;
    pair<string,string>p;
    p.first="1";
    p.second="a";
    v.push_back(p);
        p.first="3";
    p.second="c";
    v.push_back(p);
        p.first="5";
    p.second="e";
    v.push_back(p);
    p.first="2";
    p.second="b";
    v.push_back(p);
    p.first="4";
    p.second="d";
    v.push_back(p);

    for(auto i=0;i<v.size();i++){
        cout<<v[i].first<<" ";
    }

    sort(v.begin(),v.end());
    cout<<endl;
    for(auto i=0;i<v.size();i++){
        cout<<v[i].first<<" ";
    }
    cout<<endl;
        for(auto i=0;i<v.size();i++){
        cout<<v[i].second<<" ";
    }


return 0;
}