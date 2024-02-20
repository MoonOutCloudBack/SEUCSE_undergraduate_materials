#include<iostream>
#include<algorithm>
#include<cstring>
using namespace std;
const int inf=0x3f3f3f3f;

int T,n,E,s,t,M;int u,v,w,ans;int B[510];bool isin[510];
int edge[510][510];int dp[510][110];

int reach(int cur,int m){
	if(m<0)return inf;
	if(dp[cur][m]!=-1)return dp[cur][m];
	int rest,mindist=inf;
	for(int i=1;i<=n;++i){
		if(isin[i])continue;
		if(edge[cur][i]!=inf&&m>=B[i]){
			isin[i]=true;
			rest=reach(i,m-B[i]);
			if(rest!=inf&&mindist>rest+edge[cur][i])
				mindist=rest+edge[cur][i];
			isin[i]=false;
		}
	}
	return (dp[cur][m]=mindist);
}

int main(){
	ios::sync_with_stdio(false);
	cin>>T;while(T--){
		memset(edge,inf,sizeof(edge));
		memset(isin,0,sizeof(isin));
		memset(dp,-1,sizeof(dp));
		cin>>n>>E>>s>>t>>M;
		for(int i=1;i<=n;++i)cin>>B[i];B[s]=0;
		for(int i=0;i<E;++i){
			cin>>u>>v>>w;
			if(edge[u][v]>w)edge[u][v]=w;
			edge[v][u]=edge[u][v];
		}
		for(int i=0;i<=M;++i)dp[t][i]=0;
		isin[s]=true;ans=reach(s,M);
		cout<<(ans==inf?-1:ans)<<endl;
	}
	return 0;
}
