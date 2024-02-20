#include<iostream>
#include<algorithm>
#include<cstring>
using namespace std;

int M,N,X;int a[50010];

bool binarysearch(int l,int r,int X){
	if(l>=r)
		if(a[l]==X)return true;
		else return false;
	int mid=(l+r)/2;
	if(X==a[mid])return true;
	else if(X<a[mid])return binarysearch(l,mid-1,X);
	else return binarysearch(mid+1,r,X);
}

int main(){
	ios::sync_with_stdio(false);
	cin>>M;while(M--){
		cin>>N>>X;for(int i=0;i<N;++i)cin>>a[i];
		sort(a,a+N);bool ans=false;
		for(int i=0;a[i]<=X&&i<N;++i)
			if(binarysearch(0,N-1,X-a[i])){ans=true;break;}
		if(ans)cout<<"yes"<<endl;
		else cout<<"no"<<endl;
	}
	return 0;
}
