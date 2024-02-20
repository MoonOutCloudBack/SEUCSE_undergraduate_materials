#include<iostream>
#include<algorithm>
#include<cstring>
using namespace std;
typedef long long ll;
const int inf=0x3f3f3f3f;

int T,N,M,k,t;int a[1010],b[1010];

int main(){
	ios::sync_with_stdio(false);
	cin>>T;while(T--){
		cin>>N>>M;
		for(int i=0;i<N;++i)cin>>a[i];
		for(int i=0;i<M;++i)cin>>b[i];
		int pos1=(M+N)/2,pos2=(M+N-1)/2;
		int p=0,q=0,c=0;double ans=0;
		while(1){
			if((p<N&&q<M&&a[p]<=b[q])||(q>=M))t=a[p],++p;
			else t=b[q],++q;
			if(c==pos2)ans+=t;
			if(c==pos1){ans+=t;break;}
			++c;
		}
		cout<<ans/2<<endl;
	}
	return 0;
}
