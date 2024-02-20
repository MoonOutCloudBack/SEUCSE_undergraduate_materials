#include<iostream>
#include<algorithm>
#include<cstring>
#include<cmath>
using namespace std;
typedef long long ll;
const int inf=0x3f3f3f3f;

int T,K,N,dp[110][10010];

int solve(int k,int c){
	if(dp[k][c]!=-1)return dp[k][c];
	if(k==1)return dp[k][c]=c;
	if(c==1)return dp[k][c]=1;
	return dp[k][c]=solve(k,c-1)+solve(k-1,c-1)+1;
}

int main(){
	ios::sync_with_stdio(false);
	cin>>T;while(T--){
		cin>>K>>N;int ans=1;
		memset(dp,-1,sizeof(dp));
		while(solve(K,ans)<N)++ans;
		cout<<ans<<endl;
	}
	return 0;
}
