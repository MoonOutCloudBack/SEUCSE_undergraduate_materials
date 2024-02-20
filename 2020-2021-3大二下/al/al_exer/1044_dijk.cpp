#pragma GCC optimize(2)
#include<iostream>
#include<cstring>
#include<algorithm>
#include<ctime>
#include<vector>
#include<stack>
using namespace std;
typedef long long ll;
const int inf=0x3f3f3f3f;const double eps=1e-10;

struct navi{
    int num;int s;int d;  // source destiny
};
struct edge{
    double t0;double alpha;int num;int f;
    edge(double t0,double alp,int n):t0(t0),alpha(alp),num(n){f=0;}
    edge(){f=0;}
};

int T,m,E,n;navi nav[8010];
int u,v;double temp0,temp1,temp2;
edge e[410][410];bool hasedge[410][410];
double dist[410];int path[410];bool checked[410];bool canreach[410];
stack<int> out[8010];

void findpath(int i,int j,int num){
    int k=path[j],pre=j;out[num].push(j);
    while(k!=-1){
        out[num].push(e[k][pre].num),out[num].push(k);
        pre=k,k=path[k];
    }
}

int main(){
    ios::sync_with_stdio(false);
    srand(time(NULL));
    cin>>T;while(T--){
        cin>>m>>E>>n;
        memset(hasedge,0,sizeof(hasedge));
        for(int i=0;i<n;++i)cin>>nav[i].s>>nav[i].d,nav[i].num=i;
        random_shuffle(nav,nav+n);
        for(int i=0;i<E;++i){
            cin>>u>>v>>temp0>>temp1>>temp2;
            e[u][v]=edge(temp0,temp1,2*i);
            e[v][u]=edge(temp0,temp2,2*i+1);
            hasedge[u][v]=hasedge[v][u]=1;
        }
        for(int i=0;i<n;++i){  // for each navi
            memset(dist,0,sizeof(dist));
            memset(canreach,0,sizeof(canreach));
            memset(checked,0,sizeof(checked));
            memset(path,-1,sizeof(path));
            int cur=nav[i].s;canreach[cur]=1;
            for(int j=1;j<m;++j){
                checked[cur]=1;
                for(int k=0;k<m;++k)  // update dist
                    if(hasedge[cur][k]&&(!canreach[k]||
                        dist[k]>dist[cur]+e[cur][k].t0)){
                        dist[k]=dist[cur]+e[cur][k].t0;
                        canreach[k]=1,path[k]=cur;
                        e[cur][k].t0+=e[cur][k].alpha;  // update edge
                    }
                cur=-1;
                for(int k=0;k<m;++k)
                    if(!checked[k]&&canreach[k]&&(cur==-1||dist[cur]>dist[k]))
                        cur=k;
                // now get cur
            }
            findpath(nav[i].s,nav[i].d,nav[i].num);
        }
        for(int i=0;i<n;++i){
            cout<<out[i].size()<<endl;
            while(!out[i].empty())
                cout<<out[i].top()<<" ",out[i].pop();
            cout<<endl;
        }

    }
    return 0;
}
