# 数据结构实践 实验4

<p align='right'>09019106 牟倪


[TOC]



## 一、问题描述

对二路归并的外部排序进行优化：

- 对给定长度的归并段序列，基于Huffman树，构造最佳归并顺序。
- 使用多线程技术，同时读写磁盘、在内存中进行归并。

## 二、系统结构

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


## 三、功能模块设计

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


## 四、测试结果与分析

### 4.1 实验数据

测试数据生成 & 实验流程：

- 随机生成50000以内的随机整数，作为需要外排序的实验数据。
- 同时，数据量分别设为 500，5000，50000。
- 分别记录文件I/O次数`disk_count`和归并耗时。

运行结果截图（toy case）：

![image-20211220221945648](.\..\..\..\typora-user-images\image-20211220221945648.png)

运行结果表格（部分）：

![image-20211220224345648.png](.\..\..\..\typora-user-images\image-20211220224345648.png)

### 4.2 实验结论

+ 当数据量较小时，多线程程序比单线程慢大概一倍，可能是因为，在多线程实现时，为了减小编程复杂性，书写了较多冗余代码。
+ 当数据量达到5e4的时候，多线程和单线程的表现齐平。考虑到冗余代码的问题，我们可以做乐观的假设：当冗余代码被尽量缩减、两个程序在小数据下表现齐平时，多线程程序在大数据量下会比单线程快一倍。我恰好使用了两个线程，因此多线程加速是有效的。
+ 总结：多线程进行了有效的加速。

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

