# 数据结构实践 实验2

<p align='right'>09019106 牟倪



[TOC]



## 一、问题描述

使用快速排序，实现外部排序。

## 二、系统结构

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

## 三、功能模块设计

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

## 四、测试结果与分析

### 4.1 实验数据

测试数据生成 & 实验流程：

- 随机生成1000以内的随机整数，作为需要外排序的实验数据。
- 同时，数据量分别设为 10、100、1000、10000、100000。
- 同时，分别采用不同的input区、small区、large区、middle区大小。
- 分别记录文件I/O次数`disk_count`。

运行结果截图（toy case）：

![image-20211220103512267](.\..\..\..\typora-user-images\image-20211220103512267.png)

运行结果表格（部分）：

![image-20211220192230387](.\..\..\..\typora-user-images\image-20211220192230387.png)

第一个表格是数据量与访问磁盘数量的关系；

第二个表格是改变input区大小、small区大小、large区大小后，磁盘I/O次数的变化。

### 4.2 实验结论

+ 当数据量小于内存容量时，可以直接用内排序完成排序，此时磁盘I/O次数为常数。
+ 总磁盘I/O次数随数据量增加而增加，且增加速率较快，应该不是线性增长。
+ 减小input区大小，对磁盘I/O次数影响较大。
+ 减小small区大小，对磁盘I/O次数有一些影响；减小large区大小，对磁盘I/O次数影响较小。可能是因为，当快排读入新数据的大小在middle区范围内时，我们会将middle区最小值弹出写入small区，将新数据写入middle区，因此small区的读写次数比large区更多。
+ 总结：虽然快排对于内排序的性能很好，但是对于外排序的性能不太理想。

## 五、实验总结

经过本次实验，我进一步熟悉了C++语言的文件处理，了解到递归是很有力的思想。

## 六、源代码

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

