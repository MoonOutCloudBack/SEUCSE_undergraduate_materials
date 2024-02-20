#include<iostream>
#include<algorithm>
#include<cstring>
#include<vector>
using namespace std;
typedef long long ll;
const int inf=0x3f3f3f3f;

int T,N,M,k,t;
int matching[110];bool check[110];

int solve(int u,vector<int> E[]){
	for(auto i:E[u]){
		int v=i;if(!check[v]){
			check[v]=1;
			if(matching[v]==-1||solve(matching[v],E)){
				matching[v]=u,matching[u]=v;
				return true;
			}
		}
	}
	return false;
}

int main(){
	ios::sync_with_stdio(false);
	cin>>T;while(T--){
		cin>>N>>M;vector<int> E[110];
		for(int i=0;i<N;++i){
			cin>>k;for(int j=0;j<k;++j)
				cin>>t,t+=N,E[i].push_back(t),E[t].push_back(i);
		}
		memset(matching,-1,sizeof(matching));
		int ans=0;for(int u=0;u<N;++u)
			if (matching[u] == -1) {
				memset(check, 0, sizeof(check));
				if (solve(u,E)) ++ans;
			}
		cout<<ans<<endl;
	}
	return 0;
}
