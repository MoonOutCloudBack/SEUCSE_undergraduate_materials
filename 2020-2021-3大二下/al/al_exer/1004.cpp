#include<iostream>
using namespace std;

int m,n,t;int *a=NULL,*b=NULL;

void mergesort(int l,int r,int depth){
	if(l>=r)return;
	int mid=(l+r)/2;
	mergesort(l,mid,depth+1),mergesort(mid+1,r,depth+1);
	if(depth<2)return;
	int i=l,p=l,q=mid+1;
	while(i<=r)
		if(p>mid||(q<=r&&a[q]<a[p]))b[i++]=a[q++];
		else b[i++]=a[p++];
	for(int i=l;i<=r;++i)a[i]=b[i];
}

int main(){
	ios::sync_with_stdio(false);
	cin>>m;while(m--){
		cin>>n;
		a=new int[n];b=new int[n];
		for(int i=0;i<n;++i)cin>>a[i];
		mergesort(0,n-1,0);
		for(int i=0;i<n-1;++i)cout<<a[i]<<" ";
		cout<<a[n-1]<<endl;
		delete[] a;delete[] b;
	}
	return 0;
}