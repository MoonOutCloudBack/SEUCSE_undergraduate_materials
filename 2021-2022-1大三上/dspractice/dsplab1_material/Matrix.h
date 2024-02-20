#include<fstream>
#include<string>

using namespace std;

typedef long long ll;

class Matrix
{
public:
	fstream disk;
	int w;
	int* cache;
	int prex, prey;
	ll miss;

	Matrix(string name, int n, int w, int* a);
	~Matrix();
	void reset();
	int read(int x, int y);
	void write(int x, int y, int value);
};

