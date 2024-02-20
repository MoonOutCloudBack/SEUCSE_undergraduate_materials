#include<iostream>
using namespace std;

int m,n,t;int *a=NULL;

void heapsort(int n){
	for(int i=n/2-1;i>=0;--i){
		int parent=i,now;
		while(1){
			now=(parent+1)*2-1;if(now>=n)break;
			if(now+1<n&&a[now]>a[now+1])++now;
			if(a[now]<a[parent])t=a[now],a[now]=a[parent],a[parent]=t;
			else break;
			parent=now;
		}
	}
}

int main(){
	ios::sync_with_stdio(false);
	cin>>m;while(m--){
		cin>>n;
		a=new int[n];
		for(int i=0;i<n;++i)cin>>a[i];
		heapsort(n);
		for(int i=0;i<n-1;++i)cout<<a[i]<<" ";
		cout<<a[n-1]<<endl;
		delete[] a;
	}
	return 0;
}
