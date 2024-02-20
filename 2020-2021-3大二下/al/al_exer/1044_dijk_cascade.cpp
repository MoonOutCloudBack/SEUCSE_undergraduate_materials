#pragma GCC optimize(3,"Ofast","inline")
#include<iostream>
#include<cstring>
#include<algorithm>
#include<ctime>
#include<vector>
using namespace std;

int TIMES=12;

struct navi{
    int num;int s;int d;  // source destiny
};
bool operator < (navi q1,navi q2){return q1.s<q2.s;}
struct edge{
    double t0;double alpha;int num;
    edge(double t0,double alp,int n):t0(t0),alpha(alp),num(n){}
    edge(){}
};

int T,m,E,n;navi nav[8010];
int u,v;double temp0,temp1,temp2;
edge e[410][410];bool hasedge[410][410];
double dist[410];int path[410];bool checked[410];bool canreach[410];
int order[410];
vector<int> out[8010];

inline void findpath(int i,int j,int num,int times){
    out[num].clear();
    int k=path[j],pre=j;out[num].push_back(j);
    while(k!=-1){
        out[num].push_back(e[k][pre].num),out[num].push_back(k);
        e[k][pre].t0+=e[k][pre].alpha;
        pre=k,k=path[k];
    }
}

int main(){
    ios::sync_with_stdio(false);
    srand(time(NULL));
    cin>>T;while(T--){
        cin>>m>>E>>n;
        for(int i=0;i<n;++i)cin>>nav[i].d>>nav[i].s,nav[i].num=i;
        // navi's source & destiny is reversed! find path from d to s
        sort(nav,nav+n);
        memset(hasedge,0,sizeof(hasedge));
        for(int i=0;i<E;++i){
            cin>>u>>v>>temp0>>temp1>>temp2;
            e[u][v].t0=e[v][u].t0=temp0;
            e[u][v].alpha=temp2;e[v][u].alpha=temp1;
            e[u][v].num=e[v][u].num=2*i;++e[u][v].num;
            // not ++e[v][u].num, for we go from destiny to source
            hasedge[u][v]=hasedge[v][u]=1;
        }
        
        for(int times=0;times<TIMES;++times){
            for(int i=0;i<m;++i)order[i]=i;
            random_shuffle(order,order+m);
            // dijk
            for(int des=0;des<m;++des){
                int destiny=order[des];
                navi cit;cit.s=destiny;
                int base=lower_bound(nav,nav+n,cit)-nav;
                int limit=upper_bound(nav,nav+n,cit)-nav;
                if(base==n)continue;

                memset(dist,0,sizeof(dist));
                memset(canreach,0,sizeof(canreach));
                memset(checked,0,sizeof(checked));
                memset(path,-1,sizeof(path));
                int cur=destiny;canreach[cur]=1;  // actually from destiny
                for(int j=1;j<m;++j){  // main dijk
                    checked[cur]=1;
                    for(int k=0;k<m;++k)  // update dist
                        if(hasedge[cur][k]&&(!canreach[k]||
                            dist[k]>dist[cur]+e[cur][k].t0)){
                            dist[k]=dist[cur]+e[cur][k].t0;
                            canreach[k]=1,path[k]=cur;
                        }
                    cur=-1;
                    for(int k=0;k<m;++k)
                        if(!checked[k]&&canreach[k]&&(cur==-1||dist[cur]>dist[k]))
                            cur=k;
                }
                for(int i=base;i<limit;++i)
                    findpath(nav[i].s,nav[i].d,nav[i].num,times);
                // source & destiny are reversed!
            }
        }

        for(int i=0;i<n;++i){
            cout<<out[i].size()<<endl;
            for(auto j:out[i])cout<<j<<" ";
            cout<<endl;
        }
    }
    return 0;
}
