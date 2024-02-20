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
    int to;double t0;double alpha;int num;int f;
    edge(int to,double t0,double alp,int n):t0(t0),to(to),alpha(alp),num(n){f=0;}
    edge(){f=0;}
};

int T,m,E,n;navi nav[8010];
int u,v;double temp0,temp1,temp2;
vector<edge> e[410];
double dist[410][410];int path[410][410];bool canreach[410][410];
vector<int> out;

void findpath(int i,int k,int j){
    if(k==-1)
        for(auto ed:e[i])
            if(ed.to==j){out.push_back(ed.num),out.push_back(j);return;}
    findpath(i,path[i][k],k);findpath(k,path[k][j],j);
}

int main(){
    ios::sync_with_stdio(false);
    cin>>T;while(T--){
        cin>>m>>E>>n; for(int i=0;i<410;++i)e[i].clear();
        for(int i=0;i<n;++i)cin>>nav[i].s>>nav[i].d;
        for(int i=0;i<E;++i){
            cin>>u>>v>>temp0>>temp1>>temp2;
            e[u].push_back(edge(v,temp0,temp1,2*i));
            e[v].push_back(edge(u,temp0,temp2,2*i+1));
        }
        memset(dist,0,sizeof(dist));  // because of memset??
        memset(canreach,0,sizeof(canreach));
        memset(path,-1,sizeof(path));
        for(int i=0;i<m;++i){
            dist[i][i]=0,canreach[i][i]=1;
            for(auto j:e[i]){
                if(!canreach[i][j.to]||dist[i][j.to]>j.t0){
                    dist[i][j.to]=j.t0,canreach[i][j.to]=1;
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