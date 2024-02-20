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

int T,n,m;string s,p;int nex[100010];

int main(){
    ios::sync_with_stdio(false);
    cin>>T;while(T--){
        cin>>n>>m>>s>>p;
        nex[0]=nex[1]=-1;
        for(int i=2;i<=m;++i){
            int q=nex[i-1];
            while(q!=-1&&p[q+1]!=p[i-1])q=nex[q];
            if(q!=-1||p[i-1]==p[0])nex[i]=q+1;
            else nex[i]=-1;
        }
        int ans=0,i=0,j=0;
        while(i<n){
            while(j<m&&s[i]==p[j])++i,++j;
            if(j==m)++ans;if(j==0)++i;
            j=nex[j]+1;
        }
        cout<<ans<<endl;
    }
    return 0;
}
/*
1
9 3
121222121 212
*/
