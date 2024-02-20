#include"Matrix.h"
#include<string>
#include<cstdlib>

using namespace std;

Matrix::Matrix(string name, int n, int w1, int* a)
{
	// init
	disk=fstream(name,ios::in|ios::out|ios::binary);
	int value;
	for(int i=1;i<=n;++i){
		value=i*n;
		disk.write((char*)&value, sizeof(value));
	}
	for(int i=0;i<n*n;++i){
		value=a[i];
		disk.write((char*)&value, sizeof(value));
	}
	w=w1;
	cache=new int[w];
	miss=0;

	// read a[0][0]
	disk.seekg(0, ios::beg);
	int offset; disk>>offset;
	for(int i=0;i<w;++i){
		disk.seekg(offset+i, ios::beg);
		disk>>cache[i];
	}
	prex=prey=0;
	++miss;
}

Matrix::~Matrix()
{
	delete cache;
	disk.close();
}

void Matrix::reset(){
	miss=0;
	// read a[0][0]
	disk.seekg(0, ios::beg);
	int offset; disk>>offset;
	for(int i=0;i<w;++i){
		disk.seekg(offset+i, ios::beg);
		disk>>cache[i];
	}
	prex=prey=0;
	++miss;
}

int Matrix::read(int x, int y)
{
	// data in cache
	if(prex==x && y-prey>=0 && y-prey<w)
		return cache[y-prey];
	// data in disk
	disk.seekg(x, ios::beg);
	int offset; disk>>offset;
	for(int i=0;i<w;++i){
		disk.seekg(offset+y+i, ios::beg);
		disk>>cache[i];
	}
	prex=x, prey=y;
	++miss;
	return cache[0];
}

void Matrix::write(int x, int y, int value)
{
	// data in cache
	if(prex==x && y-prey>=0 && y-prey<w){
		cache[y-prey]=value;
		return;
	}
	// data in disk
	// write back
	disk.seekp(prex, ios::beg);
	int offset; disk>>offset;
	for(int i=0;i<w;++i){
		disk.seekg(offset+prey+i, ios::beg);
		disk<<cache[i];
	}
	// read new data
	disk.seekg(x, ios::beg);
	disk>>offset;
	for(int i=0;i<w;++i){
		disk.seekg(offset+i, ios::beg);
		disk>>cache[i];
	}
	prex=x, prey=y;
	++miss;
	cache[0]=value;
}
