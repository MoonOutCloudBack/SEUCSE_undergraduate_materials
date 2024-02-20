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
struct work{int d;int p;};
bool operator < (work w1,work w2){return w1.p>w2.p;}

int T,N;work w[50010];bool busy[100010];

int main(){
    ios::sync_with_stdio(false);
    cin>>T;while(T--){
        cin>>N;for(int i=0;i<N;++i)cin>>w[i].d>>w[i].p;
        sort(w,w+N);ll ans=0;
        memset(busy,0,sizeof(busy));
        for(int i=0;i<N;++i){
            int t=w[i].d;
            while(busy[t])--t;
            if(t!=0)ans+=w[i].p,busy[t]=1;
        }
        cout<<ans<<endl;
    }
    return 0;
}
