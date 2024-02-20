#include<iostream>
#include<algorithm>
#include<cstring>
using namespace std;

int M,N,C;int s[510],v[510],dp[110];

int main(){
	ios::sync_with_stdio(false);
	cin>>M;while(M--){
		cin>>N>>C;for(int i=1;i<=N;++i)cin>>s[i]>>v[i];
		memset(dp,0,sizeof(dp));
		for(int n=1;n<=N;++n)
			for(int c=C;c>=s[n];--c){
				if((c-s[n]==0||dp[c-s[n]])&&dp[c]<dp[c-s[n]]+v[n])
					dp[c]=dp[c-s[n]]+v[n];
			}
		cout<<dp[C]<<endl;
	}
	return 0;
}
