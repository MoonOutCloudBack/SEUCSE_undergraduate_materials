#include<iostream>
#include<iomanip>
#include<algorithm>
#include<cstring>
using namespace std;
typedef long long ll;
const int inf=0x3f3f3f3f;

int T,N,M,K,a[25];ll dp[25][25][25];ll t;

ll solve(int l,int r,int k){
	if(dp[l][r][k]!=-1)return dp[l][r][k];
	if(k>r-l)return dp[l][r][k]=0;
	ll ans=0;if(k==0){
		for(int i=l;i<=r;++i)ans+=a[i];
		return dp[l][r][k]=ans;
	}
	if(l==r)return dp[l][r][k]=a[l];
	if(l+1==r)return dp[l][r][k]=a[l]*a[r];
	else for(int i=l;i<r;++i)
		for(int q=0;q<k;++q){
			t=solve(l,i,q)*solve(i+1,r,k-1-q);
			if(ans<t)ans=t;
		}
	return dp[l][r][k]=ans;
}

int main(){
	ios::sync_with_stdio(false);
	cin>>T;while(T--){
		cin>>N>>K;memset(dp,-1,sizeof(dp));
		for(int i=0;i<N;++i)cin>>a[i];
		cout<<solve(0,N-1,K)<<endl;
	}
	return 0;
}

/*
1
3 1
1 3 2
*/
