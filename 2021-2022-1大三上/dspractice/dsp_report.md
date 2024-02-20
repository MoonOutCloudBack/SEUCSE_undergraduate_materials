# 数据结构实践报告

<p align='right'>09019106 牟倪


[TOC]



# Ⅰ 模拟矩阵相乘

## 1.1 问题描述

模拟矩阵相乘，实现最初版本的存储管理器。

## 1.2 系统结构

- 为矩阵编写一个类。
- 用文件模拟磁盘，将矩阵元素映射到文件内。在文件首部放置行首指针的offset，之后对元素进行顺序存储。将文件存储在类里。
- 为每个文件维护一个缓存，缓存的宽度为w。每次去文件读矩阵元素时，都把该元素以及其后的w-1个元素放到缓存中。
- 在类里维护上次读文件时所读数据的坐标 (prex, prey)。接下来读数据 (x, y) 时，首先考察是否满足 prex=x 且 0<=y-prey<w（即该数据在上次缓存范围中），如果满足则直接读缓存，如果不满足则读文件、更新缓存相关信息。
- 写数据的情形和读数据类似。

## 1.3 功能模块设计

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

## 1.4 测试结果与分析

### 1.4.1 实验数据

测试数据生成 & 实验流程：

- 将大小为 10\*10、100\*100、1000\*1000、10000\*10000 的单位矩阵相乘。
- 同时，cache大小w分别设为 1、5、10、50。
- 同时，分别采用 ijk, ikj, jik, jki, kij, kji 6种循环顺序。
- 分别记录两个乘数矩阵与一个结果矩阵的cache miss次数。

控制台结果截图（部分）：

![image-20211125201903651](.\..\..\typora-user-images\image-20211125201903651.png)

运行结果表格（部分）：

![image-20211216152836426](.\..\..\typora-user-images\image-20211216152836426.png)

### 1.4.2 实验结论

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

## 1.5 实验总结

经过本次实验，我熟悉了C++语言的使用，为接下来的实验做铺垫。

# Ⅱ 用快速排序实现外排序

## 2.1 问题描述

使用快速排序，实现外部排序。

## 2.2 系统结构

- 将内存划分为input，small，large，middle group四块，我们使用四个数组来模拟。
- 每当input为空时，从磁盘中读入数据充满input区（使用input缓存，减少磁盘读写次数）。
- 然后，从input读入数据，充满middle区。在middle区维护一个双端优先队列（这里直接使用 C++ STL 的 multiset，基于红黑树，支持自动排序）。
- 接下来，
  - 当新读到的数据小于middle区最小值时，将数据写入small区。
  - 当新读到的数据大于middle区最大值时，将数据写入large区。
  - 否则，将middle区最小值弹出，写入small区，将新数据写入middle区。

- 当small和large区满的时候，写到新文件，文件需要进行递归编号。（此时，只能保证【small里所有元素＜middle里所有元素＜large里所有元素】，但small区和large区的元素是无序的，需要继续进行递归的排序）
- middle区的数据已经排好序了（双端优先队列），将middle区数据按照排好的顺序写回文件。
- 然后，对small区数据写入的文件、large区数据写入的文件，进行递归的外排序。
- 把所有文件合并，整合得到最终的答案文件。

## 2.3 功能模块设计

- 文件操作：

  + 在设计读出写入操作的函数中，`++disk_count`。

  + 实现函数`vector<int> readFile(string name, int offset, int n)`，读取数据。
  + 实现函数`vector<int> readFileAll(string name)`，读取文件所有数据。
  + 实现函数`void writeFile(string name, int offset, vector<int>data)`，向文件写入数据。
  + 实现函数`void newSpace(string name, int n)`，新建一个名为name的文件，开辟存放n个数据的空间。
- 外排序：
  - 实现函数 `void externalSort(string name, int l, int r, int num)`。
  - 参数：文件名`name`，排序起始位置 `l`，排序终止位置 `r`，编号 `num`。
  - 功能：对name文件中，从第 l 个数据开始到第 r 个数据进行外排序。

- 衡量外排序性能：维护全局变量`disk_count`，记录文件I/O次数。

## 2.4 测试结果与分析

### 2.4.1 实验数据

测试数据生成 & 实验流程：

- 随机生成1000以内的随机整数，作为需要外排序的实验数据。
- 同时，数据量分别设为 10、100、1000、10000、100000。
- 同时，分别采用不同的input区、small区、large区、middle区大小。
- 分别记录文件I/O次数`disk_count`。

运行结果截图（toy case）：

![image-20211220103512267](.\..\..\typora-user-images\image-20211220103512267.png)

运行结果表格（部分）：

![image-20211220192230387](.\..\..\typora-user-images\image-20211220192230387.png)

第一个表格是数据量与访问磁盘数量的关系；

第二个表格是改变input区大小、small区大小、large区大小后，磁盘I/O次数的变化。

### 2.4.2 实验结论

+ 当数据量小于内存容量时，可以直接用内排序完成排序，此时磁盘I/O次数为常数。
+ 总磁盘I/O次数随数据量增加而增加，且增加速率较快，应该不是线性增长。
+ 减小input区大小，对磁盘I/O次数影响较大。
+ 减小small区大小，对磁盘I/O次数有一些影响；减小large区大小，对磁盘I/O次数影响较小。可能是因为，当快排读入新数据的大小在middle区范围内时，我们会将middle区最小值弹出写入small区，将新数据写入middle区，因此small区的读写次数比large区更多。
+ 总结：虽然快排对于内排序的性能很好，但是对于外排序的性能不太理想。

## 2.5 实验总结

经过本次实验，我进一步熟悉了C++语言的文件处理，了解到递归是很有力的思想。

# Ⅲ 用归并排序实现外排序

## 3.1 问题描述

使用二路归并排序，实现外部排序。

## 3.2 系统结构

- 生成初始归并段：

  - 每次从磁盘读入数据，直到充满内存区域，然后使用内排序方法（如快速排序）进行排序。
  - 将排序后的数据写入新文件，从而产生一系列初始归并段，并保存归并段文件名数据（names）。
- 归并：

  - 将内存划分为 input1，input2，output 3块。
  - 对归并段数组（names）进行递归的归并排序，首先对左右两部分进行归并排序（递归调用函数），得到两个有序文件（记为left_file，right_file），然后进行归并。注意，文件仍需递归编号。
  - 每当input1为空时，从left_file中读入数据，充满input1区。每当input2为空时，从right_file中读入数据，充满input2区。
  - 对input1和input2的数据进行归并，归并到output区。
  - 每当output区数据满，将数据写到新文件。

算法的描述图如下：

![image-20211222215733654](.\..\..\typora-user-images\image-20211222215733654.png)

## 3.3 功能模块设计

- 文件操作：
  + 在设计读出写入操作的函数中，`++disk_count`。

  + 实现函数`vector<int> readFile(string name, int offset, int n)`，读取数据。
  + 实现函数`vector<int> readFileAll(string name)`，读取文件所有数据。
  + 实现函数`void writeFile(string name, int offset, vector<int>data)`，向文件写入数据。
  + 实现函数`void newSpace(string name, int n)`，新建一个名为name的文件，开辟存放n个数据的空间。
- 生成初始归并段：
  - 实现函数 `vector<string> generateRes(string name, int l, int r)`。
  - 参数：原始数据（未经排序）的文件名`name`，排序起始位置`l`，排序终止位置`r`。
  - 功能：对磁盘的数据进行分批的内排序，生成初始归并段，并将初始归并段的文件名以 `vector<string>` 的形式返回。

- 外排序：
  - 实现函数 `string externalSort(vector<string> names, int l, int r, int num, int& outer_count)`；
  - 参数：归并段文件名数组`names`，排序起始位置`l`，排序终止位置`r`，编号`num`。将该区域排序后的数据个数，以引用的方式传给`outer_count`。
  - 功能：对`names`初始归并段数组中，从第`l`个段到第`r`个段的所有数据，进行外部排序。

- 衡量外排序性能：维护全局变量`disk_count`，记录文件I/O次数。

## 3.4 测试结果与分析

### 3.4.1 实验数据

测试数据生成 & 实验流程：

- 随机生成5000以内的随机整数，作为需要外排序的实验数据。
- 同时，数据量分别设为 10、100、1000、10000、100000。
- 同时，分别采用不同的input区、output区大小。
- 同时，分别采用不同的内存大小。
- 分别记录文件I/O次数`disk_count`。

运行结果截图（toy case）：

![image-20211220185733122](.\..\..\typora-user-images\image-20211220185733122.png)

运行结果表格（部分）：

![image-20211220194324073](.\..\..\typora-user-images\image-20211220194324073.png)

### 3.4.2 实验结论

+ 当数据量小于内存容量时，可以直接用内排序完成排序，此时磁盘I/O次数为常数。
+ 总磁盘I/O次数随数据量增加而增加，且增加速率较快，应该不是线性增长。
+ data_scale=1e5时，二路归并外排序的磁盘I/O次数，明显小于快排（实验二）的磁盘I/O次数。
+ 总磁盘I/O次数随内存容量减小而增加，且增加速率较快，应该不是线性增长。
+ 减小input区和output大小，磁盘I/O次数增大，且减小input区和output大小的影响是对称的。
+ 虽然实验数据是随机数，但每次排序结果都相同，不知道什么原因。可能是因为，随机数范围太小，数列每次的分布都比较相近。
+ 总结：二路归并外排序的性能比较理想。

## 3.5 实验总结

经过本次实验，我进一步熟悉了C++语言的文件处理，了解到递归是很有力的思想，感叹归并排序的思想很精简。

# Ⅳ 改进归并排序的外排序

## 4.1 问题描述

对二路归并的外部排序进行优化：

- 对给定长度的归并段序列，基于Huffman树，构造最佳归并顺序。
- 使用多线程技术，同时读写磁盘、在内存中进行归并。

## 4.2 系统结构

- 生成初始归并段：

  - 根据给定的归并段参数，生成特定个数的50000以内的随机数，然后使用内排序方法（如快速排序）进行排序。
  - 将排序后的数据写入新文件，从而产生一系列初始归并段。
- 构造最佳归并顺序：
  - 根据给定的归并段参数，基于Huffman树进行构造：
  - 维护一个multiset，每次拿走长度最短的两个归并段进行归并，并将新的归并段放入multiset中。

- 归并：
  - 将内存划分为 input1，input2，input3，input4，output 共5块。
  - 根据最佳归并顺序，将文件逐个进行归并。
- 多线程：
  - 首先，我们往input1，input2中读入数据；
  - 接下来，我们同时进行以下两件事：
    - 往input3，input4中读入数据；
    - 对input1，input2进行归并，结果输出到output中，如果output满就写回到新归并段。
  - 再接下来，我们同时进行以下两件事：
    - 往input1，input2中读入数据；
    - 对input3，input4进行归并，结果输出到output中，如果output满就写回到新归并段。
  - 直到两个归并段中，某一个归并段被读完。

多线程算法的描述图如下：

![image-20211222215825150](.\..\..\typora-user-images\image-20211222215825150.png)

Huffman树原理的描述图如下：

![image-20211222215851310](.\..\..\typora-user-images\image-20211222215851310.png)

## 4.3 功能模块设计

- 文件操作：
  + 在设计读出写入操作的函数中，`++disk_count`。

  + 实现函数`vector<int> readFile(string name, int offset, int n)`，读取数据。
  + 实现函数`vector<int> readFileAll(string name)`，读取文件所有数据。
  + 实现函数`void writeFile(string name, int offset, vector<int>data)`，向文件写入数据。
  + 实现函数`void newSpace(string name, int n)`，新建一个名为name的文件，开辟存放n个数据的空间。
- 生成初始归并段：
  - 用全局变量 `vector<pair<int, string>> merge_segments` 来记录初始的归并段参数，`int`为长度，`string`为文件名。
  - 实现函数 `void generateRes()`，用来生成初始归并段。
- 构造最佳归并顺序：
  - 用全局变量 `vector<pair<pair<string, string>, pair<int, int>>> plan` 来记录最佳归并顺序。`plan`表示一系列归并动作的`vector`，两个`string`分别是需要被归并的两个段的文件名，两个`int`分别是两个段的数据个数。归并结果的文件名约定为两个文件名的拼接。
  - 实现函数 `void generateMergePlan(vector<pair<int, string>> merge_segments)`。

- 外排序：
  - 实现函数 `void solve()`，按照最佳归并顺序进行归并。
  - 实现函数 `void externalSort(string name1, int count1, string name2, int count2)`；
    - 参数：归并段文件`name1, name2`，相应的归并段大小`count1, count2`。
- 多线程：
  - 实现函数 `void readDataIntoMem(string name1, string name2, int cur1, int cur2, bool input34)`。
    - 参数：归并段文件`name1, name2`，光标位置`cur1, cur2`，是否读进 input3 input4 的bool变量`input34`，为false则读进 input1 input2 。

  - 实现函数 `void mergeSort(string name, int curout, bool input34)`。
    - 参数：归并后写入的文件`name`，归并后写入文件的光标`curout`，是否归并 input3 input4 的bool变量`input34`，为false则归并 input1 input2 。

- 衡量外排序性能：
  - 维护全局变量`disk_count`，记录文件I/O次数。
  - 对外排序进行计时。


## 4.4 测试结果与分析

### 4.4.1 实验数据

测试数据生成 & 实验流程：

- 随机生成50000以内的随机整数，作为需要外排序的实验数据。
- 同时，数据量分别设为 500，5000，50000。
- 分别记录文件I/O次数`disk_count`和归并耗时。

运行结果截图（toy case）：

![image-20211220221945648](.\..\..\typora-user-images\image-20211220221945648.png)

运行结果表格（部分）：

![image-20211220224345648.png](.\..\..\typora-user-images\image-20211220224345648.png)

### 4.4.2 实验结论

+ 当数据量较小时，多线程程序比单线程慢大概一倍，可能是因为，在多线程实现时，为了减小编程复杂性，书写了较多冗余代码。
+ 当数据量达到5e4的时候，多线程和单线程的表现齐平。考虑到冗余代码的问题，我们可以做乐观的假设：当冗余代码被尽量缩减、两个程序在小数据下表现齐平时，多线程程序在大数据量下会比单线程快一倍。我恰好使用了两个线程，因此多线程加速是有效的。
+ 总结：多线程进行了有效的加速。

## 4.5 实验总结

经过本次实验，我进一步熟悉了C++语言的文件处理，了解到递归是很有力的思想，感叹归并排序的思想很精简。

# 附录 源代码

## 实验一

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

## 实验二

~~~c++
#include<iostream>
#include<fstream>
#include<cstring>
#include<string>
#include<algorithm>
#include<cstdlib>
#include<set>
#include<vector>

using namespace std;
const int smallN = 10, largeN = 10, inputN = 10, midN = 100;

int disk_count = 0;
multiset<int> middle;
vector<int> input, small, large;

vector<int> readFile(string name, int offset, int n){
    ++disk_count;
    ifstream infile(name, ios::in | ios::binary);
    infile.seekg(offset<<2, ios::beg);
    vector<int> res;
    for(int i = 0; i < n; ++i){
        int tmp;
        infile.read((char*)(&tmp), 4);
        if(infile) res.push_back(tmp);
        else res.push_back(0);
    }
    infile.close();
    return res;
}

vector<int> readFileAll(string name){
    ++disk_count;
    ifstream infile(name, ios::in | ios::binary);
    vector<int> res;
    for(int i = 0; infile; ++i){
        int tmp;
        infile.read((char*)(&tmp), 4);
        res.push_back(tmp);
    }
    infile.close();
    return res;
}

void writeFile(string name, int offset, vector<int> data){
    ++disk_count;
    ofstream outfile(name, ios::in|ios::out|ios::binary);
    outfile.seekp(offset<<2, ios::beg);
    int n = data.size();
    for(int i = 0; i < n; ++i){
        int tmp = data[i];
        outfile.write((char*)(&tmp),4);
    }
    outfile.close();
}

void newSpace(string name, int n){
    ofstream outfile(name);
    for(int i = 0; i < n; ++i){
        int tmp = 0;
        outfile.write((char*)(&tmp), 4);
    }
    outfile.close();
}

void externalSort(string name, int l, int r, int num){
    if(l >= r) return;

    int t = l, lc = 0, rc = 0;
    middle.clear(); // 清空middle区
    string small_file = "f" + to_string(num<<1) + ".dat"; // 递归的文件命名
    string large_file = "f" + to_string(num<<1 + 1) + ".dat";
    newSpace(small_file, 500);
    newSpace(large_file, 500);
    while(t <= r){
        input = readFile(name, t, inputN);
        t += inputN;
        for(int i = 0; i < inputN; ++i){
            if(middle.size() < midN) middle.insert(input[i]);
            else if(input[i] <= *(middle.begin())) small.push_back(input[i]);
            else if(input[i] >= *(middle.rbegin())) large.push_back(input[i]);
            else {
                small.push_back(*(middle.begin()));
                middle.erase(middle.begin());
                middle.insert(input[i]);
            }
            if(small.size() == smallN){ // small区满，写回
                writeFile(small_file, lc, small);
                small.clear();
                lc += smallN;
            }
            if(large.size() == largeN){ // large区满，写回
                writeFile(large_file, rc, large);
                large.clear();
                rc += largeN;
            }
        }
        input.clear();
    }
    int cur = l + lc;
    vector<int> mid;
    while(middle.size()){ // 写回middle区
        auto t = *(middle.begin());
        middle.erase(middle.begin());
        mid.push_back(t);
    }
    writeFile(name, cur, mid);
    externalSort(small_file, 0, lc, num<<1);
    externalSort(large_file, 0, rc, num<<1 + 1);
    vector<int> tmpa = readFile(small_file, 0, lc), tmpb = readFile(large_file, 0, rc);
    writeFile(name, l, tmpa);
    writeFile(name, r-rc, tmpb);
}

int main(){
    int N = 20; // data_scale
    newSpace("f1.dat", N); // 创建新文件
    vector<int> data;

    //cout<<"original data:\n";
    for (int i = 0; i < N; ++i){ 
        int a = rand() % 1000;
        data.push_back(a);
        //cout<<a<<" "; // 输出原始数据
    }
    //cout<<"\n\n";
    writeFile("f1.dat", 0, data); // 写入数据

    externalSort("f1.dat", 0, N-1, 1); // 外排序

    vector<int> sorted_data = readFile("f1.dat", 0, N);
    //cout<<"sorted data:\n";
    //for (int i = 0; i < N; ++i){ 
    //    cout<<sorted_data[i]<<" "; // 输出排序后数据
    //}
    
    cout<<"\n\ndisk_count="<<disk_count<<endl;
    return 0; 
}
~~~

## 实验三

~~~c++
#include<iostream>
#include<fstream>
#include<cstring>
#include<string>
#include<algorithm>
#include<cstdlib>
#include<set>
#include<tuple>
#include<vector>

using namespace std;
const int inputN = 100, outputN = 200, memN = 400, MAXN = 5000;

int disk_count = 0;
vector<int> input1(inputN), input2(inputN), output;

vector<int> readFile(string name, int offset, int n){
    ++disk_count;
    ifstream infile(name, ios::in | ios::binary);
    infile.seekg(offset<<2, ios::beg);
    vector<int>res;
    for(int i = 0; i < n; ++i){
        int tmp;
        infile.read((char*)(&tmp), 4);
        if(infile) res.push_back(tmp);
        else res.push_back(0);
    }
    infile.close();
    return res;
}

vector<int> readFileAll(string name){
    ++disk_count;
    ifstream infile(name, ios::in | ios::binary);
    vector<int> res;
    for(int i = 0; infile; ++i){
        int tmp;
        infile.read((char*)(&tmp), 4);
        res.push_back(tmp);
    }
    infile.close();
    return res;
}

void writeFile(string name, int offset, vector<int>data){
    ++disk_count;
    ofstream outfile(name, ios::in|ios::out|ios::binary);
    outfile.seekp(offset<<2, ios::beg);
    int n = data.size();
    for(int i = 0; i < n; ++i){
        int tmp = data[i];
        outfile.write((char*)(&tmp),4);
    }
    outfile.close();
}

void newSpace(string name, int n){
    ofstream outfile(name);
    for(int i = 0; i < n; ++i){
        int tmp = 0;
        outfile.write((char*)(&tmp), 4);
    }
    outfile.close();
}

vector<string> generateRes(string name, int l, int r){ // 预处理得到归并段
    vector<int> mem; // 用来内排序的内存
    vector<string> res;
    int t = l, cnt = 0;
    while(t <= r){
        mem = readFile(name, t, min(r-t,memN)); // 将数据读入内存
        sort(mem.begin(), mem.end()); // 内排序
        string segment_name = "R" + to_string(cnt++) + ".dat";
        res.push_back(segment_name);
        t += memN;
        newSpace(segment_name, mem.size());
        writeFile(segment_name, 0, mem); // 写到segment_name文件
        mem.clear();
    }
    return res;
}

string externalSort(vector<string> names, int l, int r, int num, int& outer_count){
    // names是初始段的文件列表
    // 从 文件l 排序到 文件r
    // 结果文件的编号为num
    // outer_count返回数据个数

    if(l == r) { // 就一个文件，不需要归并
        outer_count = memN;
        return names[l];
    }

    int mid = (l + r) >> 1;
    int cnt1 = 0, cnt2 = 0; // cnt1是name1的数据个数，cnt2是name2的数据个数

    string name = "G" + to_string(num) + ".dat";
    string name1 = externalSort(names, l, mid, num<<1, cnt1);
    string name2 = externalSort(names, mid+1, r, num<<1+1, cnt2); 
    newSpace(name, cnt1 + cnt2);
    // 接下来对name1文件和name2文件进行归并，归并到name文件

    int lc = 0, rc = 0, oc = 0, i = 0, j = 0;
    output.clear(), input1.clear(), input2.clear();
    // oc 是写入name数据的光标
    // lc 是读出name1数据的光标
    // rc 是读出name2数据的光标
    // i j 是在内存中归并的input1和input2的指针

    // 开始归并！
    i = inputN, j = inputN;
    // 两个文件都没读完的经典归并
    while(lc < cnt1 && rc < cnt2){

        // 读入数据part----------------------------------------
        if(i == inputN){ // input1用完了，从name1读入
            input1 = readFile(name1, lc, inputN);
            lc += inputN, i = 0; // 更新i
        }
        if(j == inputN){ // input2用完了，从name1读入
            input2 = readFile(name2, rc, inputN);
            rc += inputN, j = 0; // 更新j
        }

        while(i < inputN && j < inputN){
            if(input1[i] <= input2[j]) output.push_back(input1[i++]);
            else output.push_back(input2[j++]); 
            if(output.size() == outputN){ // output满，写回name文件
                writeFile(name, oc, output);
                oc += outputN; // 更新oc
                output.clear();
            }
        }
    }
    // 若name1文件还没读完, 则全部读入input1, 再转到output
    while(lc < cnt1){
        if(i == inputN){
            input1 = readFile(name1, lc, inputN);
            lc += inputN;
            i = 0;
        }
        while(i < inputN && j < inputN) {
            if(input1[i] <= input2[j]) output.push_back(input1[i ++]);
            else output.push_back(input2[j ++]); 
            if(output.size() == outputN){
                writeFile(name,oc, output);
                oc += outputN;
                output.clear();
            }
        }
        if(j == inputN){
            while(i < inputN) {
                output.push_back(input1[i ++]);
                if(output.size() == outputN){ // output满，写回name文件
                    writeFile(name,oc, output);
                    oc += outputN;
                    output.clear();
                }
            }
        }
    }
    // 若name2文件还没读完, 则全部读入input2, 再转到output
    while(rc < cnt2){
        if(j == inputN){
            input2 = readFile(name2, rc, inputN);
            rc += inputN;
            j = 0;
        }
        while(i < inputN && j < inputN) {
            if(input1[i] <= input2[j]) output.push_back(input1[i ++]);
            else output.push_back(input2[j ++]); 
            if(output.size() == outputN){
                writeFile(name,oc, output);
                oc += outputN;
                output.clear();
            }
        }
        if(i == inputN){
            while(j < inputN) {
                output.push_back(input2[j ++]);
                if(output.size() == outputN){ // output满，写回name文件
                    writeFile(name, oc, output);
                    oc += outputN;
                    output.clear();
                }
            }
        }
    }
    // 终于归并完了，写回文件
    if(output.size()){
        writeFile(name, oc, output);
        oc += output.size();
        output.clear();
    }

    outer_count = oc; // name文件的大小
    return name; // 返回文件的name
}

int main(){
    srand(time(NULL));

    int N = 10000; // data_scale
    newSpace("f1.dat",N+1); // 创建新文件
    vector<int> data;

    //cout<<"original data:\n";
    for (int i = 0; i < N; i++){ 
        int a = rand() % MAXN;
        data.push_back(a);
        //cout<<a<<" "; // 输出原始数据
    }
    //cout<<"\n\n";
    writeFile("f1.dat", 0, data); // 写入数据

    vector<string> names = generateRes("f1.dat", 0, N-1);
    int useless = 0;
    string result = externalSort(names, 0, names.size()-1, 1, useless); //外排
    //vector<int> sorted_data = readFileAll(result);
    //cout<<"sorted data:\n";
    //for (int i = 0; i < N; ++i){ 
    //    cout<<sorted_data[i]<<" "; // 输出排序后数据
    //}

    cout<<"\n\ndisk_count="<<disk_count<<endl;
    return 0; 
}
~~~

## 实验四

~~~c++
#include<iostream>
#include<fstream>
#include<cstring>
#include<string>
#include<algorithm>
#include<cstdlib>
#include<set>
#include<tuple>
#include<vector>
#include<thread>
#include<ctime>

using namespace std;
const int inputN = 100, outputN = 100, MAXN = 50000;

int disk_count = 0;
vector<int> input1(inputN), input2(inputN), input3(inputN), input4(inputN), output(outputN);

vector<pair<int, string>> merge_segments;
vector<pair<pair<string, string>, pair<int, int> > > plan;
void init(){
    merge_segments.push_back(pair<int, string>(500, "R1.dat"));
    merge_segments.push_back(pair<int, string>(500, "R2.dat"));
    merge_segments.push_back(pair<int, string>(1000, "R3.dat"));
    merge_segments.push_back(pair<int, string>(1000, "R4.dat"));
    merge_segments.push_back(pair<int, string>(1500, "R5.dat"));
    merge_segments.push_back(pair<int, string>(500, "R6.dat"));
}

vector<int> readFile(string name, int offset, int n){
    ++disk_count;
    ifstream infile(name, ios::in | ios::binary);
    infile.seekg(offset<<2, ios::beg);
    vector<int>res;
    for(int i = 0; i < n; ++i){
        int tmp;
        infile.read((char*)(&tmp), 4);
        if(infile) res.push_back(tmp);
        else res.push_back(0);
    }
    infile.close();
    return res;
}

vector<int> readFileAll(string name){
    ++disk_count;
    ifstream infile(name, ios::in | ios::binary);
    vector<int> res;
    for(int i = 0; infile; ++i){
        int tmp;
        infile.read((char*)(&tmp), 4);
        res.push_back(tmp);
    }
    infile.close();
    return res;
}

void writeFile(string name, int offset, vector<int>data){
    ++disk_count;
    ofstream outfile(name, ios::in|ios::out|ios::binary);
    outfile.seekp(offset<<2, ios::beg);
    int n = data.size();
    for(int i = 0; i < n; ++i){
        int tmp = data[i];
        outfile.write((char*)(&tmp),4);
    }
    outfile.close();
}

void newSpace(string name, int n){
    ofstream outfile(name);
    for(int i = 0; i < n; ++i){
        int tmp = 0;
        outfile.write((char*)(&tmp), 4);
    }
    outfile.close();
}

void generateRes(bool debug){ // 预处理得到归并段
    vector<int> mem; // 用来内排序的内存
    for(int m=0; m<merge_segments.size(); ++m){
        for(int i=0; i<merge_segments[m].first; ++i){
            int a = rand() % MAXN;
            mem.push_back(a); // 生成随机数据
            if(debug) cout<<a<<" ";
        }
        sort(mem.begin(), mem.end()); // 内排序
        newSpace(merge_segments[m].second, mem.size());
        writeFile(merge_segments[m].second, 0, mem); // 写到segment_name文件
        mem.clear();
    }
}

void generateMergePlan(){
    multiset<pair<int, string>> mset;
    // input[i] <= *(middle.begin())
    for(auto seginfo : merge_segments){
        mset.insert(seginfo); // 先全部搞到multiset里
    }
    while(mset.size()>=2){
        pair<int, string> seginfo1 = *(mset.begin()); mset.erase(mset.begin());
        pair<int, string> seginfo2 = *(mset.begin()); mset.erase(mset.begin());
        plan.push_back(pair<pair<string, string>, pair<int, int> >(
            pair<string, string>(seginfo1.second, seginfo2.second),
            pair<int, int>(seginfo1.first, seginfo2.first)
        ));
        mset.insert(pair<int, string>(seginfo1.first + seginfo2.first,
            seginfo1.second.substr(0, seginfo1.second.length()-4) + seginfo2.second)
        );
    }
}

void readDataIntoMem(string name1, string name2, int cur1, int cur2, bool input34){
    if(input34){
        input3 = readFile(name1, cur1, inputN);
        input4 = readFile(name2, cur2, inputN);
    }
    else{
        input1 = readFile(name1, cur1, inputN);
        input2 = readFile(name2, cur2, inputN);
    }
}

void mergeSort(string name, int curout, bool input34){
    int i = 0, j = 0;
    // i j 是在内存中归并的input13和input24的指针
    if(input34){
        // 经典归并
        while(i < inputN && j < inputN) {
            if(input3[i] <= input4[j]) output.push_back(input3[i++]);
            else output.push_back(input4[j++]); 
            if(output.size() == outputN){
                writeFile(name, curout, output);
                curout += outputN;
                output.clear();
            }
        }
        // clear out
        writeFile(name, curout, output);
        curout += output.size();
        output.clear();
        // input3没读完
        while(i < inputN){
            output.push_back(input3[i++]);
            if(output.size() == outputN){
                writeFile(name, curout, output);
                curout += outputN;
                output.clear();
            }
        }
        // input4没读完
        while(j < inputN){
            output.push_back(input4[j++]);
            if(output.size() == outputN){
                writeFile(name, curout, output);
                curout += outputN;
                output.clear();
            }
        }
        // clear out
        writeFile(name, curout, output);
        curout += output.size();
        output.clear();
    }
    else{
        // 经典归并
        while(i < inputN && j < inputN) {
            if(input1[i] <= input2[j]) output.push_back(input1[i++]);
            else output.push_back(input2[j++]); 
            if(output.size() == outputN){
                writeFile(name, curout, output);
                curout += outputN;
                output.clear();
            }
        }
        // clear out
        writeFile(name, curout, output);
        curout += output.size();
        output.clear();
        // input1没读完
        while(i < inputN){
            output.push_back(input1[i++]);
            if(output.size() == outputN){
                writeFile(name, curout, output);
                curout += outputN;
                output.clear();
            }
        }
        // input2没读完
        while(j < inputN){
            output.push_back(input2[j++]);
            if(output.size() == outputN){
                writeFile(name, curout, output);
                curout += outputN;
                output.clear();
            }
        }
        // clear out
        writeFile(name, curout, output);
        curout += output.size();
        output.clear();
    }
}

void externalSort(string name1, int count1, string name2, int count2){
    string name = name1.substr(0, name1.length()-4) + name2;
    newSpace(name, count1 + count2);
    // 接下来对name1文件和name2文件进行归并，归并到name文件

    int cur1 = 0, cur2 = 0, curout = 0;
    output.clear(), input1.clear(), input2.clear();
    // curout 是写入name数据的光标
    // cur1 是读出name1数据的光标
    // cur2 是读出name2数据的光标

    // 开始归并！
    readDataIntoMem(name1, name2, cur1, cur2, false); // input12
    // 两个文件都没读完的经典归并
    while(cur1 < count1 && cur2 < count2){
        thread first(readDataIntoMem, name1, name2, cur1, cur2, true); // 用来读入数据的线程
        thread second(mergeSort, name, curout, false); // 用来归并的线程
        first.join(); second.join();
        cur1 += inputN, cur2 += inputN, curout += inputN<<1; // 因为归并了两个input嘛
        if(cur1 >= count1 || cur2 >= count2) break;

        thread first1(readDataIntoMem, name1, name2, cur1, cur2, false); // 用来读入数据的线程
        thread second1(mergeSort, name, curout, true); // 用来归并的线程
        first1.join(); second1.join();
    }

    // 若name1文件还没读完，则全部读入output。output应该已经clear了
    while(cur1 < count1){
        output = readFile(name1, cur1, outputN);
        cur1 += outputN;
        writeFile(name, curout, output);
        curout += outputN;
        output.clear();
    }
    // 若name2文件还没读完，则全部读入output。output应该已经clear了
    while(cur2 < count2){
        output = readFile(name2, cur2, outputN);
        cur2 += outputN;
        writeFile(name, curout, output);
        curout += outputN;
        output.clear();
    }
}

void solve(){
    for(auto action : plan){
        externalSort(action.first.first, action.second.first,
            action.first.second, action.second.second);
    }
}

int main(){
    srand(time(NULL));
    init();
    //cout<<"original data:\n";
    generateRes(false);
    //cout<<"\n\n";
    generateMergePlan();

    // 输出最佳merge plan
    cout<<"best merge plan:\n";
    for(auto action : plan){
        cout<<"merge "<<action.first.first<<" "<<action.second.first<<", "
            <<action.first.second<<" "<<action.second.second<<endl;
    }
    cout<<"\n\n";

    clock_t start, end;
    start = clock(); // 程序开始计时
    solve();
    end = clock();
    double endtime=(double)(end - start) / CLOCKS_PER_SEC;

    //int N = 0;
    //for(auto seginfo : merge_segments) N += seginfo.first;
    //vector<int> sorted_data = readFileAll(
    //    plan[plan.size()-1].first.first + plan[plan.size()-1].first.second);
    //cout<<"sorted data:\n";
    //for (int i = 0; i < N; ++i){ 
    //    cout<<sorted_data[i]<<" "; // 输出排序后数据
    //}
    //cout<<"\n\n";

    cout<<"disk_count="<<disk_count<<endl;
    cout<<"\nTotal time="<<endtime * 1000<<"ms"<<endl; // ms为单位
    return 0; 
}
~~~

