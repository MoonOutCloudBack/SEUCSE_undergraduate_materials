#include<iostream>
#include<cstring>
using namespace std;

int n,C,N;int w[100000],c[100000],dp[100000][100];
int main(){
	ios::sync_with_stdio(false);
	memset(dp,0,sizeof(dp));
	cin>>n>>C>>N;
	for(int i=1;i<=n;++i)
		cin>>c[i]>>w[i];  // 从1开始计数

	for(int i=1;i<=n;++i)
		for(int s=C;s>0;--s)
			for(int j=N;j>0;--j)
				if(s>=c[i]&&dp[s][j]<dp[s-c[i]][j-1]+w[i])
					dp[s][j]=dp[s-c[i]][j-1]+w[i];

	cout<<dp[C][N];
	return 0;
}
