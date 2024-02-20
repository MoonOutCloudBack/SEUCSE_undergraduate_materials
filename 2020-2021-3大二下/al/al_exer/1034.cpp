#include<iostream>
#include<cstring>
#include<algorithm>
#include<cmath>
#include<set>
#include<map>
#include<vector>
#include<stack>
#include<queue>
#include<string>
using namespace std;
typedef long long ll;
const int inf=0x3f3f3f3f;
const ll mod=1e9+7;const double eps=1e-10;

int T,N,u,v;int isb[50010];bool checked[50010];

int solve(int cur,vector<int>* e,bool b,int f){  // b is cur's color
    if(b&&isb[cur]!=-1)return isb[cur];
    int ans=0;if(b)ans=1;  // solve(cur) counts cur
    for(auto i:e[cur]){
        if(i==f)continue;
        int bcase=solve(i,e,0,cur),wcase=solve(i,e,1,cur);
        if(b)ans+=bcase;else ans+=max(bcase,wcase);
    }
    if(b)isb[cur]=ans;
    return ans;
}

int main(){
    ios::sync_with_stdio(false);
    cin>>T;while(T--){
        cin>>N;vector<int> e[50010];
        for(int i=1;i<N;++i)
            cin>>u>>v,e[v].push_back(u),e[u].push_back(v);
        memset(isb,-1,sizeof(isb));memset(checked,0,sizeof(checked));
        cout<<max(solve(1,e,0,-1),solve(1,e,1,-1))<<endl;
    }
    return 0;
}
