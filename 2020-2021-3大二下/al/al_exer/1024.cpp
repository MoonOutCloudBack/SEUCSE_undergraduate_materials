#include<iostream>
#include<iomanip>
#include<algorithm>
#include<cstring>
using namespace std;
typedef long long ll;
const int inf=0x3f3f3f3f;

int T,N,M,k;double a[510],b[510],dp[510][510],w[510][510],t;

double solve(int l,int r){
	if(dp[l][r]!=0)return dp[l][r];
	if(r==l-1)return dp[l][r]=0;
	if(l==r)return dp[l][r]=w[l][l];
	double ans=inf;
	for(int i=l;i<=r;++i){
		t=solve(l,i-1)+solve(i+1,r)+w[l][r];
		if(ans>t)ans=t;
	}
	return dp[l][r]=ans;
}

int main(){
	ios::sync_with_stdio(false);
	cin>>T;while(T--){
		cin>>N;memset(dp,0,sizeof(dp));
		memset(w,0,sizeof(w));
		for(int i=1;i<=N;++i)cin>>a[i];
		for(int i=1;i<=N;++i)cin>>a[i];
		for(int i=0;i<=N;++i)cin>>b[i];
		for(int i=1;i<=N;++i){
			w[i][i]=b[i-1]+a[i]+b[i];
			for(int j=i+1;j<=N;++j)
				w[i][j]=w[i][j-1]+a[j]+b[j];
		}
		cout<<setiosflags(ios::fixed)<<setprecision(6)<<solve(1,N)<<endl;
	}
	return 0;
}
