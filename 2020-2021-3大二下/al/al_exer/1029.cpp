#include<iostream>
#include<cstring>
#include<algorithm>
#include<set>
#include<map>
#include<vector>
#include<stack>
#include<queue>
#include<string>
using namespace std;
typedef long long ll;
const int inf=0x3f3f3f3f;
const ll mod=1e9+7;

struct edge{
    int from;int to;int w;
    edge(int f,int t,int w):from(f),to(t),w(w){}
    edge(){}
};
bool operator < (edge e1,edge e2){return e1.w<e2.w;}

int ufs[510];
int find(int i){
    if(ufs[i]!=i)return ufs[i]=find(ufs[i]);
    return i;
}
void merge(int i,int j){
    ufs[find(i)]=find(j);
}

int T,N,E,u,v,w;edge e[20010];

int main(){
    ios::sync_with_stdio(false);
    cin>>T;while(T--){
        cin>>N>>E;int c=0;
        for(int i=0;i<E;++i){
            cin>>u>>v>>w;--u,--v;if(u==v)continue;
            e[c].from=u,e[c].to=v,e[c].w=w,++c;
        }
        int ans=0;for(int i=0;i<N;++i)ufs[i]=i;
        sort(e,e+c);int co=0;
        for(int i=0;i<N-1;++i){
            if(co>=c){ans=-1;break;}
            edge ed=e[co++];
            if(find(ed.from)!=find(ed.to))merge(ed.from,ed.to),ans+=ed.w;
            else --i;
        }
        cout<<ans<<endl;
    }
    return 0;
}
