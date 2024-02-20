#include<iostream>
#include<iomanip>
#include<algorithm>
#include<cstring>
using namespace std;
typedef long long ll;
const int inf=0x3f3f3f3f;

int T,N;int b[1010];double t;

int main(){
	ios::sync_with_stdio(false);
	cout.setf(ios::fixed);
	cin>>T;while(T--){
		cin>>N;double ans=0;
		for(int i=1;i<N;++i)cin>>t,ans+=t/100;
		cout<<fixed<<setprecision(6)<<1+ans<<endl;
	}
	return 0;
}
