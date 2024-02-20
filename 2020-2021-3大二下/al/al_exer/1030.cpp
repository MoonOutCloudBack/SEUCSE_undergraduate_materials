#include<iostream>
#include<cstring>
#include<algorithm>
#include<set>
#include<map>
#include<vector>
#include<stack>
#include<queue>
#include<string>
using namespace std;
typedef long long ll;
const int inf=0x3f3f3f3f;
const ll mod=1e9+7;

int T,n,t;

int main(){
    ios::sync_with_stdio(false);
    cin>>T;while(T--){
        cin>>n;n*=2;stack<int> s;int ans=0;
        for(int i=1;i<=n;++i){
            cin>>t;
            if(t)
                if(!s.empty()&&s.top()<0)ans+=i+s.top(),s.pop();
                else s.push(i);
            else
                if(!s.empty()&&s.top()>0)ans+=i-s.top(),s.pop();
                else s.push(-i);
        }
        cout<<ans<<endl;
    }
    return 0;
}
