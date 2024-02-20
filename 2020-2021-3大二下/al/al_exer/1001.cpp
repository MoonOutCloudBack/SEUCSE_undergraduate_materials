#include<iostream>
#include<string>
using namespace std;

long long n,ans;string s;
int main(){
	ios::sync_with_stdio(false);
	cin>>n;getline(cin,s);  // 玄学，不知道为什么
	for(int i=0;i<n;++i){
		getline(cin,s);
		ans=0;
		for(int j=0;j<s.length();++j)
			if(isdigit(s[j]))++ans;
		cout<<ans<<endl;
	}
	return 0;
}
