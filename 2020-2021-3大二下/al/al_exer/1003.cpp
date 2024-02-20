#include<iostream>
using namespace std;

int m,n,t;int *a=NULL;

void bubblesort(int *a,int n){
	// for(int i=1;i<n;++i)
		for(int j=1;j<n;++j)
			if(a[j-1]>a[j])
				t=a[j],a[j]=a[j-1],a[j-1]=t;
}

int main(){
	ios::sync_with_stdio(false);
	cin>>m;
	while(m--){
		cin>>n;a=new int[n];
		for(int i=0;i<n;++i)cin>>a[i];
		bubblesort(a,n);
		for(int i=0;i<n-1;++i)cout<<a[i]<<" ";
		cout<<a[n-1]<<endl;
		delete[] a;
	}
	return 0;
}
