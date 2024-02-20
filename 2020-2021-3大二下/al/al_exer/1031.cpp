#include<iostream>
#include<cstring>
#include<algorithm>
#include<cmath>
#include<set>
#include<map>
#include<vector>
#include<stack>
#include<queue>
#include<string>
using namespace std;
typedef long long ll;
const int inf=0x3f3f3f3f;
const ll mod=1e9+7;const double eps=1e-10;
struct pos{double x;double y;double r;};
bool operator < (pos p1,pos p2){
    double temp=p1.r-p2.r;
    if(abs(temp)<eps)return 0;
    return (temp<0);
}

int T,n,t;double d;pos p[10010];bool cover [10010];

int main(){
    ios::sync_with_stdio(false);
    cin>>T;while(T--){
        cin>>n>>d;
        for(int i=0;i<n;++i)
            cin>>p[i].x>>p[i].y,p[i].r=p[i].x+sqrt(d*d-p[i].y*p[i].y);
        memset(cover,0,sizeof(cover));
        sort(p,p+n);int c=0;
        for(int i=0;i<n;++i){
            if(cover[i])continue;++c;
            for(int j=i;j<n;++j){
                if(p[j].r-p[i].r>2*d)break;
                if(cover[j])continue;
                double temp=(p[j].x-p[i].r)*(p[j].x-p[i].r)+p[j].y*p[j].y-d*d;
                if(temp<=eps)cover[j]=1;
            }
        }
        cout<<c<<endl;
    }
    return 0;
}
