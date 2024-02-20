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

int T,N,E,u,v,w,s,t;int e[505][505];int dist[505];bool b[505];

int main(){
    ios::sync_with_stdio(false);
    cin>>T;while(T--){
        cin>>N>>E>>s>>t;--s,--t;
        memset(e,-1,sizeof(e));
        for(int i=0;i<E;++i){
            cin>>u>>v>>w;--u,--v;
            if(e[u][v]==-1||e[u][v]>w)e[u][v]=e[v][u]=w;
        }
        memset(dist,-1,sizeof(dist));memset(b,0,sizeof(b));
        for(int i=0;i<N;++i)dist[i]=e[s][i];dist[s]=0,b[s]=1;
        for(int i=1;i<N;++i){
            u=inf,v=-1;
            for(int j=0;j<N;++j)
                if(!b[j]&&dist[j]!=-1&&dist[j]<u)u=dist[j],v=j;
            if(v==-1)break;b[v]=1;
            for(int k=0;k<N;++k)
                if(e[v][k]!=-1&&!b[k]&&(dist[k]==-1||dist[k]>dist[v]+e[v][k]))
                    dist[k]=dist[v]+e[v][k];
        }
        cout<<dist[t]<<endl;
    }
    return 0;
}
