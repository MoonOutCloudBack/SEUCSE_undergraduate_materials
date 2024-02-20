#include<iostream>
#include<set>
#include<map>
#include<algorithm>
#include<cstring>
using namespace std;
typedef long long ll;
const int inf=0x3f3f3f3f;
struct ac{int s;int f;int v;};
bool operator <(ac a1,ac a2){return a1.f<a2.f;}

int T,N;ac a[10010];int dp[30000];

int main(){
	ios::sync_with_stdio(false);
	cin>>T;while(T--){
		cin>>N;set<int> se;map<int,int> mp;
		for(int i=0;i<N;++i)
			cin>>a[i].s>>a[i].f>>a[i].v,se.insert(a[i].s),se.insert(a[i].f);
		int count=0;
		for(auto i:se)mp[i]=count,++count;
		for(int i=0;i<N;++i)
			a[i].s=mp[a[i].s],a[i].f=mp[a[i].f];
		sort(a,a+N);int ddl=a[N-1].f;
		memset(dp,0,sizeof(dp));
		for(int n=0;n<N;++n)
			for(int c=ddl;c>=a[n].f;--c)
				if(dp[c]<dp[a[n].s]+a[n].v)
					dp[c]=dp[a[n].s]+a[n].v;
		cout<<dp[ddl]<<endl;
	}
	return 0;
}
