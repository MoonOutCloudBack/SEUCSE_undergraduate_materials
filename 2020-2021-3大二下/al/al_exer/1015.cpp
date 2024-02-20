#include<iostream>
#include<algorithm>
#include<cstring>
#include<utility>
#include<set>
#include<vector>
using namespace std;
typedef long long ll;

int M,N,C;int a,b,h;vector<pair<int,int>> p;
multiset<int> s;

int main(){
	ios::sync_with_stdio(false);
	cin>>N;for(int i=0;i<N;++i){
		cin>>a>>b>>h;
		p.push_back({a,-h});p.push_back({b,h});
	}
	sort(p.begin(),p.end());s.insert(0);
	int old=0,ans=0;
	for(auto i:p){
		if(i.second<0)s.insert(-i.second);
		else s.erase(s.find(i.second));
		ans=*s.rbegin();
		if(ans!=old)cout<<i.first<<" "<<ans<<endl,old=ans;
	}
	return 0;
}

/*
about multiset:
s.insert(1);s.insert(5);s.insert(-4);
cout<<(s.find(4)==s.end())<<(s.find(1)==s.end())<<endl;
s.erase(s.find(1));
cout<<(s.find(1)==s.end())<<endl;
*/