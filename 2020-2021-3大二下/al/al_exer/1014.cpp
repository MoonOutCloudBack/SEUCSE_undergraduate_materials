#include<iostream>
#include<algorithm>
#include<cstring>
using namespace std;

int M,N;int a[50010];

int main(){
	ios::sync_with_stdio(false);
	cin>>M;while(M--){
		cin>>N;for(int i=0;i<N;++i)cin>>a[i];
		int sum=a[0],v=a[0];
		for(int i=1;i<N;++i){
			if(v>0)v+=a[i];
			else v=a[i];
			if(v>sum)sum=v;
		}
		cout<<sum<<endl;
	}
	return 0;
}
