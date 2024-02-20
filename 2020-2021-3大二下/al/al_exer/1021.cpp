#include<iostream>
#include<cstring>
using namespace std;
typedef long long ll;

int M,N,C;int t;int c[10010],v[10010];int dp[10010];

int main(){
	ios::sync_with_stdio(false);
	cin>>M;while(M--){
		cin>>C>>N;for(int i=0;i<N;++i)cin>>c[i]>>v[i];
		memset(dp,0,sizeof(dp));
		for(int i=0;i<N;++i)
			for(int j=c[i];j<=C;++j)
				if(dp[j]<dp[j-c[i]]+v[i])
					dp[j]=dp[j-c[i]]+v[i];
		cout<<dp[C]<<endl;
	}
	return 0;
}
