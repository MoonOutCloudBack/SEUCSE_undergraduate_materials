#include<iostream>
using namespace std;

int m,n,t;int *a=NULL;

void quicksort(int l,int r,int depth){
	if(depth==2)return;
	if(l>=r)return;
	if(l+1==r){
		if(a[l]>a[r])t=a[l],a[l]=a[r],a[r]=t;
		return;
	}
	int pivot=a[l],pos=l;
	for(int i=l+1;i<=r;++i)
		if(a[i]<pivot)
			t=a[i],a[i]=a[++pos],a[pos]=t;
	a[l]=a[pos],a[pos]=pivot;
	quicksort(l,pos-1,depth+1),quicksort(pos+1,r,depth+1);
}

int main(){
	ios::sync_with_stdio(false);
	cin>>m;while(m--){
		cin>>n;
		a=new int[n];
		for(int i=0;i<n;++i)cin>>a[i];
		quicksort(0,n-1,0);
		for(int i=0;i<n-1;++i)cout<<a[i]<<" ";
		cout<<a[n-1]<<endl;
		delete[] a;
	}
	return 0;
}
/*
0: 6 3 7 8 5 1 4 2 4 9 10
1:
6 3 7 8 5 1 4 2 4 9 10
4 3 5 1 4 2 6 8 7 9 10
对8 7 9 10，再排一次：
7 8 9 10
对4 3 5 1 4 2，再排一次：
2 3 1 4 4 5
a: 2 3 1 4 4 5 6 7 8 9 10
*/