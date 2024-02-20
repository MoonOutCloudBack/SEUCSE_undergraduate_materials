#include<iostream>
#include<cstring>
using namespace std;

int n,C;int w[100000],c[100000],dp[100000];
int main(){
	ios::sync_with_stdio(false);
	memset(dp,0,sizeof(dp));
	cin>>n>>C;
	for(int i=1;i<=n;++i)
		cin>>c[i]>>w[i];  // 从1开始计数

	for(int i=1;i<=n;++i)
		for(int s=C;s>0;--s)
			if(s>=c[i]&&dp[s]<dp[s-c[i]]+w[i])
				dp[s]=dp[s-c[i]]+w[i];

	cout<<dp[C];
	return 0;
}
