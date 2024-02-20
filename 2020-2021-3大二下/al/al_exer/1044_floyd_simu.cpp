#pragma GCC optimize(3,"Ofast","inline")
#include<iostream>
#include<cstring>
#include<algorithm>
#include<ctime>
#include<vector>
using namespace std;

struct navi{
    int num;int s;int d;  // source destiny
};
struct edge{
    double t0;double alpha;int num;
    edge(double t0,double alp,int n):t0(t0),alpha(alp),num(n){}
    edge(){}
};

int T,m,E,n;navi nav[8010];
int u,v;double temp0,temp1,temp2;
edge e[410][410];bool hasedge[410][410];
double dist[410][410];int path[410][410];bool canreach[410][410];
vector<int> out[8010];

int TIMES=5;double beta=0.68;

inline void clearpath(int num,int &times){
    if(!out[num].empty()){
        int pre=out[num][0],j;
        for(int u=2;u<out[num].size();u+=2){
        j=out[num][u];
        e[pre][j].t0-=e[pre][j].alpha*beta;
        pre=j;
        }
    }
}

inline void findpath(int i,int k,int j,int num,int &times){
    if(k==-1){
        out[num].push_back(e[i][j].num),out[num].push_back(j);
        e[i][j].t0+=e[i][j].alpha;
        return;
    }
    findpath(i,path[i][k],k,num,times);findpath(k,path[k][j],j,num,times);
}

int main(){
    ios::sync_with_stdio(false);
    srand(time(NULL));
    cin>>T;while(T--){
        cin>>m>>E>>n;
        for(int i=0;i<n;++i)cin>>nav[i].s>>nav[i].d,nav[i].num=i;
        memset(hasedge,0,sizeof(hasedge));
        for(int i=0;i<E;++i){
            cin>>u>>v>>temp0>>temp1>>temp2;
            e[u][v].t0=e[v][u].t0=temp0;
            e[u][v].alpha=temp1;e[v][u].alpha=temp2;
            e[u][v].num=e[v][u].num=2*i;++e[v][u].num;
            hasedge[u][v]=hasedge[v][u]=1;
        }
        random_shuffle(nav,nav+n);

        for(int times=0;times<TIMES;++times){
            int gap=max(n/6,1);
            for(int cur=0;cur<n;cur+=gap){
                memset(dist,0,sizeof(dist));
                memset(canreach,0,sizeof(canreach));
                memset(path,-1,sizeof(path));
                // clearpath
                int lim=(n<cur+gap)?n:cur+gap;
                if(times)for(int i=cur;i<lim;++i)
                        clearpath(nav[i].num,times);
                // floyd init
                for(int i=0;i<m;++i){
                    canreach[i][i]=1;
                    for(int j=0;j<m;++j)
                        if(hasedge[i][j])
                            dist[i][j]=e[i][j].t0,canreach[i][j]=1;
                }
                for(int k=0;k<m;++k)  // m points
                    for(int i=0;i<m;++i)
                        for(int j=0;j<m;++j)
                            if(canreach[i][k]&&canreach[k][j]&&
                                (!canreach[i][j]||dist[i][j]>dist[i][k]+dist[k][j])){
                                dist[i][j]=dist[i][k]+dist[k][j],canreach[i][j]=1;
                                path[i][j]=k;
                                // merge the path
                            }
                for(int i=cur;i<lim;++i){
                    out[nav[i].num].clear();out[nav[i].num].push_back(nav[i].s);
                    findpath(nav[i].s,path[nav[i].s][nav[i].d],nav[i].d,nav[i].num,times);
                }
            }
        }
        for(int i=0;i<n;++i){
            cout<<out[i].size()<<endl;
            for(auto j:out[i])cout<<j<<" ";
            cout<<endl;out[i].clear();
        }
    }
    return 0;
}
