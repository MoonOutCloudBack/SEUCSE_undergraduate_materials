#include<iostream>
#include<string>
#include<cstring>
using namespace std;

int M,N,K;string s;long long dp[50][8];long long h[42][42];

long long getv(int start,int stop){
	if(start<0||stop<0)return 0;
	if(h[start][stop]>=0)return h[start][stop];
	long long ans=s[start]-'0';
	for(int i=start+1;i<=stop;++i)
		ans=ans*10+(s[i]-'0');
	return ans;
}

int main(){
	ios::sync_with_stdio(false);
	cin>>M;while(M--){
		memset(h,-1,sizeof(h));
		cin>>N>>K;cin>>s;
		for(int i=0;i<=N;++i)dp[i][0]=getv(0,i-1);

		for(int k=1;k<=K;++k){
			for(int n=k+1;n<=N-K+k;++n){
				long long b=-1;
				for(int i=k;i<n;++i){
					b=dp[i][k-1]*getv(i,n-1);
					if(b>dp[n][k])dp[n][k]=b;
				}
			}
		}
		cout<<dp[N][K]<<endl;
	}
	return 0;
}