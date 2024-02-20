#include<iostream>
#include<cstring>
#include<algorithm>
#include<cmath>
#include<vector>
using namespace std;
typedef long long ll;
const int inf=0x3f3f3f3f;const double eps=1e-10;

struct navi{
    int s;int d;  // source destiny
};
struct edge{
    double t0;double alpha;int num;
    double pre;
    edge(double t0,double alp,int n):t0(t0),alpha(alp),num(n){pre=0;}
    edge(){pre=0;}
};

int T,m,E,n;navi nav[8010];
int u,v;double temp0,temp1,temp2;
edge e[410][410];int d[410];bool hasedge[410][410];
double dist[410][410];int path[410][410];bool canreach[410][410];
vector<int> out;
int beta=26;

void findpath(int i,int k,int j){
    if(k==-1)
        {out.push_back(e[i][j].num),out.push_back(j);return;}
    findpath(i,path[i][k],k);findpath(k,path[k][j],j);
}

int main(){
    ios::sync_with_stdio(false);
    cin>>T;while(T--){
        cin>>m>>E>>n;
        for(int i=0;i<n;++i)cin>>nav[i].s>>nav[i].d;
        memset(hasedge,0,sizeof(hasedge));
        memset(d,0,sizeof(d));
        for(int i=0;i<E;++i){
            cin>>u>>v>>temp0>>temp1>>temp2;
            e[u][v].t0=e[v][u].t0=temp0;
            e[u][v].alpha=temp1;e[v][u].alpha=temp2;
            e[u][v].num=e[v][u].num=2*i;++e[v][u].num;
            hasedge[u][v]=hasedge[v][u]=1;
            ++d[u],++d[v];
        }
        for(int i=0;i<m;++i)
            for(int j=0;j<m;++j)
                if(hasedge[i][j])
                    e[i][j].pre=e[i][j].t0+e[i][j].alpha*(d[i]+d[j])*beta;
        memset(dist,0,sizeof(dist));
        memset(canreach,0,sizeof(canreach));
        memset(path,-1,sizeof(path));
        for(int i=0;i<m;++i){
            dist[i][i]=0,canreach[i][i]=1;
            for(int j=0;j<m;++j){
                if(hasedge[i][j]){
                    dist[i][j]=e[i][j].pre,canreach[i][j]=1;
                }
            }
        }
        for(int k=0;k<m;++k)  // m points
            for(int i=0;i<m;++i)
                for(int j=0;j<m;++j)
                    if(i!=j&&canreach[i][k]&&canreach[k][j]&&(!canreach[i][j]||dist[i][j]>dist[i][k]+dist[k][j])){
                        dist[i][j]=dist[i][k]+dist[k][j],canreach[i][j]=1;
                        path[i][j]=k;
                        // merge the path
                    }
        for(int i=0;i<n;++i){
            out.clear();
            findpath(nav[i].s,path[nav[i].s][nav[i].d],nav[i].d);
            cout<<1+out.size()<<endl<<nav[i].s;
            for(auto j:out)cout<<" "<<j;
            cout<<endl;
        }
    }
    return 0;
}