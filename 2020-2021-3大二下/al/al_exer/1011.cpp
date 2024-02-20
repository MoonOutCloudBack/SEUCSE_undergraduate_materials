#include<iostream>
#include<iomanip>
#include<algorithm>
#include<cmath>
using namespace std;
typedef long long ll;

struct point{ll x;ll y;};

int M,N;double t;point a[50010];

ll dist(int i,int j){
	return (a[i].x-a[j].x)*(a[i].x-a[j].x)+(a[i].y-a[j].y)*(a[i].y-a[j].y);
}

bool operator < (point p1,point p2){return p1.x<p2.x;}

ll binarysolve(point *a,int l,int r){
	int mid=(l+r)/2;ll ans;
	if(l+1==r)return dist(l,r);
	else if(l+2==r){
		ans=dist(l,r);
		t=dist(l,l+1);if(ans>t)ans=t;
		t=dist(l+1,r);if(ans>t)ans=t;
		return ans;
	}
	ans=binarysolve(a,l,mid);
	t=binarysolve(a,mid+1,r);if(ans>t)ans=t;
	int pl=mid,pr=mid;double b=sqrt(ans);
	while(pl>l&&a[mid].x-a[pl-1].x<b)--pl;
	while(pr<r&&a[pr+1].x-a[mid].x<b)++pr;
	for(int i=pl;i<pr;++i)
		for(int j=i+1;j<=pr;++j){
			if(abs(a[i].y-a[j].y)>b)continue;
			t=dist(i,j);if(ans>t)ans=t;
		}
	return ans;
}

int main(){
	ios::sync_with_stdio(false);
	cout.setf(ios::fixed);
	cin>>M;while(M--){
		cin>>N;for(int i=0;i<N;++i)cin>>a[i].x>>a[i].y;
		sort(a,a+N);
		cout<<fixed<<setprecision(2)<<sqrt(binarysolve(a,0,N-1))<<endl;
	}
	return 0;
}
