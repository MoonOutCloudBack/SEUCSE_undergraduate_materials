#include<iostream>
using namespace std;

int M,N,t;int a[50010],b[50010];

int mergesolve(int *a,int l,int r){
	int mid=(l+r)/2,ans=0;
	if(l+1==r){
		if(a[l]>a[r])t=a[l],a[l]=a[r],a[r]=t,ans=1;
		return ans;
	}
	else if(l==r)return 0;
	ans+=mergesolve(a,l,mid);ans+=mergesolve(a,mid+1,r);
	int c=mid-l+1,i=l,j=mid+1,p=l;
	while(p<=r){
		if((a[j]<a[i]&&j<=r)||i>mid)b[p++]=a[j++],ans+=c;
		else b[p++]=a[i++],--c;
	}
	for(int i=l;i<=r;++i)a[i]=b[i];
	return ans;
}

int main(){
	ios::sync_with_stdio(false);
	cin>>M;while(M--){
		cin>>N;for(int i=0;i<N;++i)cin>>a[i];
		cout<<mergesolve(a,0,N-1)<<endl;
	}
	return 0;
}
