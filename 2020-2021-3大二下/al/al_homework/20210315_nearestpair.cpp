#include<iostream>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<ctime>
#include<cstdlib>
using namespace std;

struct node{
	double x;double y;
	bool operator < (node n){return x<n.x;}
};
int n;node a[10010];
double dis(int i,int j){
	return sqrt((a[i].x-a[j].x)*(a[i].x-a[j].x)+(a[i].y-a[j].y)*(a[i].y-a[j].y));
}
double calc(int l,int r){                         
	if(r-l==1) return dis(l,r);
	else if(r-l==2){
		double d0=dis(l,l+1),d1=dis(l+1,r),d2=dis(l,r);
		if(d0>d1)d0=d1;if(d0>d2)d0=d2;
		return d0;
	}
	int m=(l+r)/2;double dt=2e9;
	double d1=calc(l,m),d2=calc(m+1,r),d=d1<d2?d1:d2;
	int p=m,q=m+1,piv=(a[m].x+a[m+1].x)/2;
	while((piv-a[p].x)<d&&p>=l)--p;++p;
	while((a[q].x-piv)<d&&q<=r)++q;--q;
	if(p>m||q<=m)return d;
	for(int i=p;i<=m;++i){
		for(int j=m+1;j<=q;++j){
			if(a[j].y>a[i].y+d||a[j].y<a[i].y-d)continue;
			dt=dis(i,j);
			if(d>dt)d=dt;
		}
	}
	return d;
}

int main(){
	ios::sync_with_stdio(false);
	srand(time(NULL));
	int nlist[10]={10,20,30,50,80,100,200,500,800,1000};
	cout<<"n\tt(s)"<< endl;

	// 对n=[10,20,30,50,80,100,200,500,800,1000]各跑100次，看运行时间
	for(int t=0;t<10;++t){
		clock_t startTime,endTime;
		int times=100;startTime=clock();
		while(times--){  // 100次
			n=nlist[t];double xt,yt;  // 手动输入n
			for(int i=0;i<n;++i){
				xt=rand()+rand()/double(RAND_MAX);  // 随机数输入
				yt=rand()+rand()/double(RAND_MAX);
				a[i].x=0.5*xt-sqrt(3)/2*yt;  // 旋转，避免被卡
				a[i].y=sqrt(3)/2*xt+0.5*yt;
			}
			sort(a,a+n);
			// cout<<calc(0,n-1);  // 结果正确，在某刷题网站上测试过了
		}
		endTime = clock();
		cout<<n<<"\t"<<(double)(endTime-startTime)/CLOCKS_PER_SEC <<"s"<< endl;
	}
	return 0;
}
