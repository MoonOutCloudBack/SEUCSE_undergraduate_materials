# 数据结构实践 实验1

<p align='right'>09019106 牟倪



[TOC]



## 一、问题描述

模拟矩阵相乘，实现最初版本的存储管理器。

## 二、系统结构

- 为矩阵编写一个类。
- 用文件模拟磁盘，将矩阵元素映射到文件内。在文件首部放置行首指针的offset，之后对元素进行顺序存储。将文件存储在类里。
- 为每个文件维护一个缓存，缓存的宽度为w。每次去文件读矩阵元素时，都把该元素以及其后的w-1个元素放到缓存中。
- 在类里维护上次读文件时所读数据的坐标 (prex, prey)。接下来读数据 (x, y) 时，首先考察是否满足 prex=x 且 0<=y-prey<w（即该数据在上次缓存范围中），如果满足则直接读缓存，如果不满足则读文件、更新缓存相关信息。
- 写数据的情形和读数据类似。

## 三、功能模块设计

矩阵类 `Matrix`：

- 成员变量：
  - `disk`，是一个文件。
  - `cache`，数据类型为int指针。
  - `w`，cache长度。
  - `prex, prey`，上次读文件时所读数据的坐标。
  - `miss`，读文件次数。
- 成员函数：
  - 构造函数 `Matrix(string name, int n, int w, int* a)`：
    - 创建一个名为 name 的文件，`disk=fstream(name,ios::in|ios::out|ios::binary)`。
    - 将n*n的数组a作为矩阵数据，写入文件：
      - 我们期望，在读行 x 列 y 的数值时，可以先通过直接索引 x 找到行 x 的地址 offset，然后通过 offset+y 找到该数据。
      - 因此，首先写入n个行指针的offset，第x行的offset为(x+1)*n。
      - 接下来，依序写入矩阵数据。
    - 初始化cache长度，`this.w=w`。
    - 初始化缓存，`cache=new int[w]`。
    - 初始化读文件次数，`miss=0`。
    - 简单起见，直接`prex=prey=0`，读数据a\[0]\[0]、把数据放到cache里、miss++。
  - 析构函数 `~Matrix()`：
    - 关闭文件、delete缓存空间。
  - 读数据 `int read(int x, int y)`：
    - 参数为行 x、列 y。
    - 我们考察：是否满足`prex==x && 0<=y-prey<w`；若满足，则说明数据在缓存中，直接返回`cache[y-prey]`。
    - 若不满足，则首先读文件，读到指向行 x 的指针offset，然后读 offset+y，读取数据；并读取其后的w-1个数值，存入cache里，更新`prex=x, prey=y`，`++miss`。
  - 写数据 `void write(int x, int y, int value)`：
    - 参数为行 x、列 y、希望写入的值 value。
    - 我们考察：是否满足`prex==x && 0<=y-prey<w`；若满足，则说明数据在缓存中，直接修改`cache[y-prey]`。
    - 若不满足，则：
      - 首先将cache的数据写回文件；
      - 然后读文件，读到指向行 x 的指针offset，然后读 offset+y，读取数据；并读取其后的w-1个数值，存入cache里，更新`prex=x, prey=y`，`++miss`；
      - 最后在cache里修改数据。

## 四、测试结果与分析

### 4.1 实验数据

测试数据生成 & 实验流程：

- 将大小为 10\*10、100\*100、1000\*1000、10000\*10000 的单位矩阵相乘。
- 同时，cache大小w分别设为 1、5、10、50。
- 同时，分别采用 ijk, ikj, jik, jki, kij, kji 6种循环顺序。
- 分别记录两个乘数矩阵与一个结果矩阵的cache miss次数。

控制台结果截图（部分）：

![image-20211125201903651](F:\typora-user-images\image-20211125201903651.png)

运行结果表格（部分）：

![image-20211216152836426](F:\typora-user-images\image-20211216152836426.png)

### 4.2 实验结论

观察实验数据，可以得到以下结论：（记n为矩阵尺寸，w为cache尺寸）

- 对ijk的遍历顺序，A miss=$n^3/w$，B miss=$n^3$，C miss=$n^2/w$。
- 对ikj的遍历顺序，A miss=$n^3/w$，B miss=$n^3/w$，C miss=$n^2/w$。
- 对jik的遍历顺序，A miss=$n^3/w$，B miss=$n^3$，C miss=$n^2$。
- 对jki的遍历顺序，A miss=$n^3$，B miss=$n^3$，C miss=$n^2$。
- 对kij的遍历顺序，A miss=$n^3/w$，B miss=$n^3/w$，C miss=$n^2/w$。
- 对kji的遍历顺序，A miss=$n^3/w$，B miss=$n^3/w$，C miss=$n^2$。

进而可以得到以下结论：

- 如果i是k的外层循环，A miss=$n^3/w$，否则A miss=$n^3$。
- 如果k是j的外层循环，B miss=$n^3/w$，否则B miss=$n^3$。
- 如果i是j的外层循环，C miss=$n^2/w$，否则C miss=$n^2$。

## 五、实验总结

经过本次实验，我熟悉了C++语言的使用，为接下来的实验做铺垫。

## 六、源代码

### Matrix.h：

```cpp
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
```

### Matrix.cpp：

```cpp
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
```

### dslab1.cpp：

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<fstream>
#include"Matrix.h"

using namespace std;

int main()
{
    int mat_size[3]={10, 100, 1000};
    int cache_size[4]={1, 5, 10, 50};
    for(int I=0;I<3;++I)
        for(int J=0;J<4;++J){
            // declaration
            int* a=new int[mat_size[I]*mat_size[I]];
            int* c=new int[mat_size[I]*mat_size[I]];
            memset(a,0,sizeof(a));
            memset(c,0,sizeof(c));
            for(int K=0;K<mat_size[I];++K) a[K*mat_size[I]+K]=1;

            int tempa, tempb, tempc;
            // ijk
            {
            Matrix A("./tempA", mat_size[I], cache_size[J], a);
            Matrix B("./tempB", mat_size[I], cache_size[J], a);
            Matrix C("./tempC", mat_size[I], cache_size[J], c);
            for(int i=0;i<mat_size[I];++i)
                for(int j=0;j<mat_size[I];++j)
                    for(int k=0;k<mat_size[I];++k){
                        // c[i][j]+=a[i][k]*b[k][j];
                        tempa=A.read(i, k);
                        tempb=B.read(k, j);
                        tempc=tempa*tempb;
                        C.write(i, j, tempc);
                    }
            printf("\nmat size = %d, cache size = %d, ijk order\n",
                mat_size[I], cache_size[J]);
            printf("A miss = %lld, B miss = %lld, C miss = %lld\n",
                A.miss, B.miss, C.miss);
            }

            // ikj
            {
            Matrix A("./tempA", mat_size[I], cache_size[J], a);
            Matrix B("./tempB", mat_size[I], cache_size[J], a);
            Matrix C("./tempC", mat_size[I], cache_size[J], c);
            for(int i=0;i<mat_size[I];++i)
                for(int k=0;k<mat_size[I];++k)
                    for(int j=0;j<mat_size[I];++j){
                        // c[i][j]+=a[i][k]*b[k][j];
                        tempa=A.read(i, k);
                        tempb=B.read(k, j);
                        tempc=tempa*tempb;
                        C.write(i, j, tempc);
                    }
            printf("\nmat size = %d, cache size = %d, ikj order\n",
                mat_size[I], cache_size[J]);
            printf("A miss = %lld, B miss = %lld, C miss = %lld\n",
                A.miss, B.miss, C.miss);
            }

            // jik
            {
            Matrix A("./tempA", mat_size[I], cache_size[J], a);
            Matrix B("./tempB", mat_size[I], cache_size[J], a);
            Matrix C("./tempC", mat_size[I], cache_size[J], c);
            for(int j=0;j<mat_size[I];++j)
                for(int i=0;i<mat_size[I];++i)
                    for(int k=0;k<mat_size[I];++k){
                        // c[i][j]+=a[i][k]*b[k][j];
                        tempa=A.read(i, k);
                        tempb=B.read(k, j);
                        tempc=tempa*tempb;
                        C.write(i, j, tempc);
                    }
            printf("\nmat size = %d, cache size = %d, jik order\n",
                mat_size[I], cache_size[J]);
            printf("A miss = %lld, B miss = %lld, C miss = %lld\n",
                A.miss, B.miss, C.miss);
            }

            // jki
            {
            Matrix A("./tempA", mat_size[I], cache_size[J], a);
            Matrix B("./tempB", mat_size[I], cache_size[J], a);
            Matrix C("./tempC", mat_size[I], cache_size[J], c);
            for(int j=0;j<mat_size[I];++j)
                for(int k=0;k<mat_size[I];++k)
                    for(int i=0;i<mat_size[I];++i){
                        // c[i][j]+=a[i][k]*b[k][j];
                        tempa=A.read(i, k);
                        tempb=B.read(k, j);
                        tempc=tempa*tempb;
                        C.write(i, j, tempc);
                    }
            printf("\nmat size = %d, cache size = %d, jki order\n",
                mat_size[I], cache_size[J]);
            printf("A miss = %lld, B miss = %lld, C miss = %lld\n",
                A.miss, B.miss, C.miss);
            }

            // kij
            {
            Matrix A("./tempA", mat_size[I], cache_size[J], a);
            Matrix B("./tempB", mat_size[I], cache_size[J], a);
            Matrix C("./tempC", mat_size[I], cache_size[J], c);
            for(int k=0;k<mat_size[I];++k)
                for(int i=0;i<mat_size[I];++i)
                    for(int j=0;j<mat_size[I];++j){
                        // c[i][j]+=a[i][k]*b[k][j];
                        tempa=A.read(i, k);
                        tempb=B.read(k, j);
                        tempc=tempa*tempb;
                        C.write(i, j, tempc);
                    }
            printf("\nmat size = %d, cache size = %d, kij order\n",
                mat_size[I], cache_size[J]);
            printf("A miss = %lld, B miss = %lld, C miss = %lld\n",
                A.miss, B.miss, C.miss);
            }

            // kji
            {
            Matrix A("./tempA", mat_size[I], cache_size[J], a);
            Matrix B("./tempB", mat_size[I], cache_size[J], a);
            Matrix C("./tempC", mat_size[I], cache_size[J], c);
            for(int k=0;k<mat_size[I];++k)
                for(int j=0;j<mat_size[I];++j)
                    for(int i=0;i<mat_size[I];++i){
                        // c[i][j]+=a[i][k]*b[k][j];
                        tempa=A.read(i, k);
                        tempb=B.read(k, j);
                        tempc=tempa*tempb;
                        C.write(i, j, tempc);
                    }
            printf("\nmat size = %d, cache size = %d, kji order\n",
                mat_size[I], cache_size[J]);
            printf("A miss = %lld, B miss = %lld, C miss = %lld\n",
                A.miss, B.miss, C.miss);
            }
            
            delete[] a; delete[] c;
        }
}
```

