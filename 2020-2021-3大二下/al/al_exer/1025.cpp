#include<iostream>
#include<algorithm>
#include<cstring>
using namespace std;
typedef long long ll;
const int inf=0x3f3f3f3f;

int T,N,M,k,t;int a[10010],dp[10010];

int main(){
	ios::sync_with_stdio(false);
	cin>>T;while(T--){
		cin>>N;for(int i=0;i<N;++i)cin>>a[i];
		memset(dp,-1,sizeof(dp));
		for(int i=1;i<N;++i)
			for(int j=0;j<i;++j)
				if(a[j]<=a[i])dp[i]=min(dp[i],dp[j]-1);
		int ans=0;
		for(int i=0;i<N;++i)if(ans>dp[i])ans=dp[i];
		cout<<-ans<<endl;
	}
	return 0;
}
