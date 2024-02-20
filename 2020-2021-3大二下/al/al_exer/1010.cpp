#include<iostream>
#include<string>
#include<cstring>
using namespace std;

int M,N,X;string s;int a[20010];

void binarysearch(int l,int r,int X,int f){
	if(l>=r)
		if(a[l]==X){cout<<"success, father is "<<a[f]<<endl;return;}
		else {cout<<"not found, father is "<<a[l]<<endl;return;}
	int mid=(l+r)/2;
	if(X<a[mid])binarysearch(l,mid-1,X,mid);
	else binarysearch(mid,r,X,mid);
}

int main(){
	ios::sync_with_stdio(false);
	cin>>M;while(M--){
		cin>>N>>X;for(int i=0;i<N;++i)cin>>a[i];
		binarysearch(0,N-1,X,-1);
	}
	return 0;
}