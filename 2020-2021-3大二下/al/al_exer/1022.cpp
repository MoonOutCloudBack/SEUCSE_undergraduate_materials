#include<iostream>
#include<algorithm>
#include<cstring>
#include<string>
using namespace std;
typedef long long ll;
const int inf=0x3f3f3f3f;

int T,N,M,k,t;string s1,s2;int dp[510][510];

int main(){
	ios::sync_with_stdio(false);
	cin>>T;while(T--){
		cin>>s1>>s2;
		memset(dp,0,sizeof(dp));
		for(int i=1;i<=s1.length();++i)
			for(int j=1;j<=s2.length();++j){
				if(s1[i-1]==s2[j-1])dp[i][j]=dp[i-1][j-1]+1;
				t=max(dp[i-1][j],dp[i][j-1]);
				dp[i][j]=max(dp[i][j],t);
			}
		cout<<dp[s1.length()][s2.length()]<<endl;
	}
	return 0;
}
