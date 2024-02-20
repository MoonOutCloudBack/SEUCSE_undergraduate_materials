#include<iostream>
#include<string>
#include<cstring>
using namespace std;

int M,N;string s;int a[105],dp[105];


int main(){
	ios::sync_with_stdio(false);
	cin>>M;while(M--){
		cin>>N;for(int i=0;i<N;++i)cin>>a[i];
		for(int i=0;i<N;++i)dp[i]=1;
		for(int i=1;i<N;++i){
			for(int j=0;j<i;++j)
				if(a[j]>=a[i]&&dp[j]+1>dp[i])dp[i]=dp[j]+1;
		}
		int ans=1;for(int i=0;i<N;++i)if(dp[i]>ans)ans=dp[i];
		cout<<ans<<" ";
		for(int i=0;i<N;++i)dp[i]=1;
		for(int i=1;i<N;++i){
			for(int j=0;j<i;++j)
				if(a[j]<a[i]&&dp[j]+1>dp[i])dp[i]=dp[j]+1;
		}
		ans=1;for(int i=0;i<N;++i)if(dp[i]>ans)ans=dp[i];
		cout<<ans<<endl;
	}
	return 0;
}