#pragma GCC optimize(3,"Ofast","inline")
#include<iostream>
#include<cstring>
#include<algorithm>
#include<ctime>
#include<vector>
using namespace std;

int DIJK_TIMES=3,ITE_TIMES=3;

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
edge e_dijk[410][410];
double dist_dijk[410];int path_dijk[410];bool checked_dijk[410];bool canreach_dijk[410];
int order[410];

vector<int> out[8010];
edge e[410][410];bool hasedge[410][410];  // hasedge can use twice
double dist[410][410];int path[410][410];bool canreach[410][410];
// double beta=1;

bool cmptime(navi q1,navi q2){
    double t1=0,t2=0;int pre,j;
    if(!out[q1.num].empty()){
        pre=out[q1.num][0];
        for(int u=2;u<out[q1.num].size();u+=2){
            j=out[q1.num][u];t1+=e[pre][j].t0;
            pre=j;
        }
    }
    if(!out[q2.num].empty()){
        pre=out[q2.num][0];
        for(int u=2;u<out[q2.num].size();u+=2){
            j=out[q2.num][u];t2+=e[pre][j].t0;
            pre=j;
        }
    }
    return t1>t2;
}


inline void findpath_dijk(int i,int j,int num,int &times){
    int k,pre;
    out[num].clear();
    k=path_dijk[j],pre=j;out[num].push_back(j);
    while(k!=-1){
        out[num].push_back(e_dijk[k][pre].num),out[num].push_back(k);
        e_dijk[k][pre].t0+=e_dijk[k][pre].alpha*0.8;
        pre=k,k=path_dijk[k];
    }
}

inline void clearpath(int num){
    if(!out[num].empty()){
        int pre=out[num][0],j;
        for(int u=2;u<out[num].size();u+=2){
            j=out[num][u];
            e[pre][j].t0-=e[pre][j].alpha;
            pre=j;
        }
    }
}

inline void findpath(int i,int j,int num){
    int k,pre;
    out[num].clear();
    k=path_dijk[j],pre=j;out[num].push_back(j);
    while(k!=-1){
        out[num].push_back(e_dijk[k][pre].num),out[num].push_back(k);
        e_dijk[k][pre].t0+=e_dijk[k][pre].alpha;
        pre=k,k=path_dijk[k];
    }
}


int main(){
    ios::sync_with_stdio(false);
    srand(time(NULL));
    cin>>T;while(T--){
        // getdata
        cin>>m>>E>>n;
        for(int i=0;i<n;++i)cin>>nav[i].d>>nav[i].s,nav[i].num=i;
            // navi's source & destiny is reversed! find path from d to s
        sort(nav,nav+n);
        memset(hasedge,0,sizeof(hasedge));
        for(int i=0;i<E;++i){
            cin>>u>>v>>temp0>>temp1>>temp2;
            e_dijk[u][v].t0=e_dijk[v][u].t0=temp0;
            e_dijk[u][v].alpha=temp2;e_dijk[v][u].alpha=temp1;  // alpha should be reversed
            e_dijk[u][v].num=e_dijk[v][u].num=2*i;++e_dijk[u][v].num;
            // not ++e_dijk[v][u].num, for we go from destiny to source
            e[u][v]=e_dijk[u][v],e[v][u]=e_dijk[v][u];  // copy
            hasedge[u][v]=hasedge[v][u]=1;
        }

        // dijk preprocess
        for(int times=0;times<DIJK_TIMES;++times){
            for(int i=0;i<m;++i)order[i]=i;
            random_shuffle(order,order+m);
            // dijk
            for(int des=0;des<m;++des){
                int destiny=order[des];
                navi cit;cit.s=destiny;
                int base=lower_bound(nav,nav+n,cit)-nav;
                int limit=upper_bound(nav,nav+n,cit)-nav;
                if(base==n)continue;

                memset(dist_dijk,0,sizeof(dist_dijk));
                memset(canreach_dijk,0,sizeof(canreach_dijk));
                memset(checked_dijk,0,sizeof(checked_dijk));
                memset(path_dijk,-1,sizeof(path_dijk));
                int cur=destiny;canreach_dijk[cur]=1;  // actually from destiny
                for(int j=1;j<m;++j){  // main dijk
                    checked_dijk[cur]=1;
                    for(int k=0;k<m;++k)  // update dist_dijk
                        if(hasedge[cur][k]&&(!checked_dijk[k])&&(!canreach_dijk[k]||
                            dist_dijk[k]>dist_dijk[cur]+e_dijk[cur][k].t0)){
                            dist_dijk[k]=dist_dijk[cur]+e_dijk[cur][k].t0;
                            canreach_dijk[k]=1,path_dijk[k]=cur;
                        }
                    cur=-1;
                    for(int k=0;k<m;++k)
                        if(!checked_dijk[k]&&canreach_dijk[k]&&
                            (cur==-1||dist_dijk[cur]>dist_dijk[k]))
                            cur=k;
                }
                for(int i=base;i<limit;++i)
                    findpath_dijk(nav[i].s,nav[i].d,nav[i].num,times);
                // source & destiny are reversed!
            }
        }
        
        // change format
        for(int i=0;i<n;++i){
            for(int j=2;j<out[i].size();j+=2){
                u=out[i][j-2],v=out[i][j];
                e[v][u].t0+=e[v][u].alpha;  // reversed edge & path
            }
        }
        
        // dijk iterate
        for(int times=0;times<ITE_TIMES;++times){
            sort(nav,nav+n,cmptime);
            int halt=n/3/(times+1);
            for(int i=0;i<halt;++i){
                clearpath(nav[i].num);
                int destiny=nav[i].s;
                memset(dist_dijk,0,sizeof(dist_dijk));
                memset(canreach_dijk,0,sizeof(canreach_dijk));
                memset(checked_dijk,0,sizeof(checked_dijk));
                memset(path_dijk,-1,sizeof(path_dijk));
                int cur=destiny;canreach_dijk[cur]=1;  // actually from destiny
                for(int j=1;j<m;++j){  // main dijk
                    checked_dijk[cur]=1;
                    for(int k=0;k<m;++k)  // update dist_dijk
                        if(hasedge[cur][k]&&(!checked_dijk[k])&&(!canreach_dijk[k]||
                            dist_dijk[k]>dist_dijk[cur]+e_dijk[cur][k].t0)){
                            dist_dijk[k]=dist_dijk[cur]+e_dijk[cur][k].t0;
                            canreach_dijk[k]=1,path_dijk[k]=cur;
                        }
                    cur=-1;
                    for(int k=0;k<m;++k)
                        if(!checked_dijk[k]&&canreach_dijk[k]&&
                            (cur==-1||dist_dijk[cur]>dist_dijk[k]))
                            cur=k;
                }
                findpath(nav[i].s,nav[i].d,nav[i].num);
                // source & destiny are reversed!
            }
        }

        // output
        for(int i=0;i<n;++i){
            cout<<out[i].size()<<endl;
            for(auto j:out[i])cout<<j<<" ";
            cout<<endl;
        }
    }
    return 0;
}
