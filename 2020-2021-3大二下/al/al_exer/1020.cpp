#include<iostream>
#include<cstring>
using namespace std;
typedef long long ll;
struct mat{int r;int c;};

int M,N;int t;mat a[510];int dp[510][510];

int main(){
	ios::sync_with_stdio(false);
	cin>>M;while(M--){
		cin>>N;for(int i=0;i<N;++i)cin>>a[i].r>>a[i].c;
		memset(dp,0,sizeof(dp));
		for(int j=1;j<=N-1;++j)
			for(int i=0;i+j<=N-1;++i){
				dp[i][i+j]=2e9;
				for(int k=i;k<i+j;++k)
					if(dp[i][i+j]>dp[i][k]+dp[k+1][i+j]+a[i].r*a[k].c*a[i+j].c)
						dp[i][i+j]=dp[i][k]+dp[k+1][i+j]+a[i].r*a[k].c*a[i+j].c;
			}
		cout<<dp[0][N-1]<<endl;
	}
	return 0;
}
