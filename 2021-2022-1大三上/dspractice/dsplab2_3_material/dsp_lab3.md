# 数据结构实践 实验3

<p align='right'>09019106 牟倪


[TOC]



## 一、问题描述

使用二路归并排序，实现外部排序。

## 二、系统结构

- 生成初始归并段：

  - 每次从磁盘读入数据，直到充满内存区域，然后使用内排序方法（如快速排序）进行排序。
  - 将排序后的数据写入新文件，从而产生一系列初始归并段，并保存归并段文件名数据（names）。
- 归并：

  - 将内存划分为 input1，input2，output 3块。
  - 对归并段数组（names）进行递归的归并排序，首先对左右两部分进行归并排序（递归调用函数），得到两个有序文件（记为left_file，right_file），然后进行归并。注意，文件仍需递归编号。
  - 每当input1为空时，从left_file中读入数据，充满input1区。每当input2为空时，从right_file中读入数据，充满input2区。
  - 对input1和input2的数据进行归并，归并到output区。
  - 每当output区数据满，将数据写到新文件。


## 三、功能模块设计

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

## 四、测试结果与分析

### 4.1 实验数据

测试数据生成 & 实验流程：

- 随机生成5000以内的随机整数，作为需要外排序的实验数据。
- 同时，数据量分别设为 10、100、1000、10000、100000。
- 同时，分别采用不同的input区、output区大小。
- 同时，分别采用不同的内存大小。
- 分别记录文件I/O次数`disk_count`。

运行结果截图（toy case）：

![image-20211220185733122](.\..\..\..\typora-user-images\image-20211220185733122.png)

运行结果表格（部分）：

![image-20211220194324073](.\..\..\..\typora-user-images\image-20211220194324073.png)

### 4.2 实验结论

+ 当数据量小于内存容量时，可以直接用内排序完成排序，此时磁盘I/O次数为常数。
+ 总磁盘I/O次数随数据量增加而增加，且增加速率较快，应该不是线性增长。
+ data_scale=1e5时，二路归并外排序的磁盘I/O次数，明显小于快排（实验二）的磁盘I/O次数。
+ 总磁盘I/O次数随内存容量减小而增加，且增加速率较快，应该不是线性增长。
+ 减小input区和output大小，磁盘I/O次数增大，且减小input区和output大小的影响是对称的。
+ 虽然实验数据是随机数，但每次排序结果都相同，不知道什么原因。可能是因为，随机数范围太小，数列每次的分布都比较相近。
+ 总结：二路归并外排序的性能比较理想。

## 五、实验总结

经过本次实验，我进一步熟悉了C++语言的文件处理，了解到递归是很有力的思想，感叹归并排序的思想很精简。

## 六、源代码

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

