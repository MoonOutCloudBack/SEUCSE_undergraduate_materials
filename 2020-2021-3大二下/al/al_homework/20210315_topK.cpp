#include<iostream>
#include<cstdlib>
#include<ctime>
using namespace std;

int *a=NULL;
int Partition(int l,int r){
	int pos=l,pivot=a[pos];
	for(int i=l;i<=r;++i){
		if(a[i]<pivot)a[pos]=a[i],++pos;
	}
	a[pos]=pivot;
	return pos;  // 这个是绝对位置哦
}
int topK(int k,int l,int r){
	if(l==r)return a[l];
	int pos=Partition(l,r);
	if(pos==k)return a[pos];
	else if(pos>k)return topK(k,l,pos-1);
	else return topK(k,pos+1,r);
}
int main(){
	ios::sync_with_stdio(false);
	srand(time(NULL));
	int n;cin>>n;a=new int[n];
	for(int i=0;i<n;++i)a[i]=rand();  // 用a[i]=n-i测试过
	int k;cin>>k;--k;  // 语义上第k大是从1开始，但数组的下标从0开始
	cout<<topK(k,0,n-1);
	delete[] a;
	return 0;
}
