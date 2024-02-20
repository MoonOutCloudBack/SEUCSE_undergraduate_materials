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

int T,N;ll c[100010],y;

int main(){
    ios::sync_with_stdio(false);
    cin>>T;while(T--){
        cin>>N;for(int i=0;i<N;++i)cin>>c[i];
        cin>>y;ll ans=y*c[0];ll best=c[0];
        for(int i=1;i<N;++i){
            cin>>y;
            if((++best)>c[i])best=c[i];
            ans+=y*best;
        }
        cout<<ans<<endl;
    }
    return 0;
}