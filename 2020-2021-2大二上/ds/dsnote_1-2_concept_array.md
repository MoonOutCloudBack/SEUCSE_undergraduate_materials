# 1-2章的笔记

## 第一章：一些基本概念

算法的定义：1. 有限步；2. 有输出；3. 无歧义；

O的定义：f(n)=O(g(n))当且仅当存在正值常数c和n0，使得任意n>n0有f(n)<=c*g(n)；

## 第二章：array

### 数组 Array

一维数组直接寻址；二维数组行优先存放：loc\[i][j]=a+(i*n+j)size；列优先存放：loc\[i][j]=a+(i+j\*m)size；

一行一行的存，a00a01a02...a10a11...；列优先是a00a10a20...a01a11a21...；一般来说都是行优先存放；

多维数组：loc\[i0]\[i1][i2]...[in]=a+(i0\*m1\*...\*mn+i1\*m2*...\*mn+...+in)；感觉并不会考；

### 稀疏矩阵 Sparse Matrix

稀疏因子：非零元素数/总元素数，越小越稀疏；

用三元组存放稀疏矩阵元素：int col;int row;int value;

存放在一个数组中，以row为第一关键码col为第二关键码排好序；

#### 转置 Transpose

对原矩阵A扫描一遍；

按A中每一元素的列号；

立即确定在转置矩阵B三元组表中的位置，并装入它；

```cpp
matritem* FastTranspose(matritem *A,int r,int c,int k){
    matritem *B=new matritem[k];
    int rowSize[c];memset(rowSize,0,sizeof(rowSize));
    //rowSize记录矩阵转置前各列，即转置后矩阵各行非零元素个数
    for(int i=0;i<k;++i)++rowSize[A[i].col];
    int rowStart[c];memset(rowStart,0,sizeof(rowStart));
    //rowStart记录各行非零元素在转置后三元组表中开始存放位置
    for(int i=1;i<c;++i)rowStart[i]=rowStart[i-1]+rowSize[i-1];
    for(int i=0;i<k;++i){
        B[rowStart[A[i].col]].row=A[i].col;
        B[rowStart[A[i].col]].col=A[i].row;//行列互换，转置了哦
        B[rowStart[A[i].col]].value=A[i].value;
    	++rowStart[A[i].col]；
    }
    return B;
}
```

### 字符串 String

字串，主串（包含子串的串），字串在主串中的位置（首字符下标）；

提取子串：pos+len-1<=curLen-1, [pos,len)；pos+len-1>curLen-1, [pos,curLen)；这里是左闭右开；

#### 模式匹配 String Pattern Matching

暴力算法Brute Force：遍历主串，第一个字符相同就匹配第二个，不同就换下一个；

最坏情况(n-m+1)m=O(nm)，n主串长度，m字串长度；

##### KMP算法

next特征向量：当模式中第j个字符失配时，应该从next[j]再开始匹配；

next[j]=（注意啊，是0到j-1都匹配的，然后j失配了；我们考察的是0到j-1的字符，所以next0=-1

- -1, j=0；
- k, 从0到k-1的字符都是匹配的，从k开始继续考察；
- 0，其他情况，指第一个就不一样；

实例，会清楚一点：

|       j | 0    | 1    | 2    | 3    | 4    | 5    | 6    | 7    |
| ------: | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- |
|    char | a    | b    | a    | a    | b    | c    | a    | c    |
| next[j] | -1   | 0    | 0    | 1    | 1    | 2    | 0    | 1    |

如果j>0，那么在下一趟比较时，模式串P的起始比较位置是next[j]，**目标串T的指针不回溯**，仍指向上一趟失配的字符；

如果next[j]<0，那就**目标串T的指针进1**，模式串P的指针指向0；

```cpp
int kmp(string pat,string tar){
    int ppos=0,tpos=0;//分别是模式串的指针和目标串的指针
    int plen=pat.length(),tlen=tar.length();
    while(ppos<plen&&tpos<tlen){
        if(ppos==-1||pat[ppos]==tar[tpos])ppos++,tpos++;
        //若是-1，那么是上一次比到了0，next0了；
        //上一次本来想从模式串的第0个字符开始比，但是第0个就不一样，所以ppos=-1；
        //两个pos同时++的过程中，ppos回到了0，tpos前进了1；
        else ppos=next[ppos];//嗯，这时候失配了
    }
    if(ppos==plen)return tpos-plen;//是子串的起始位置
    else return -1;//没找到
}
```

##### 计算next

定义：当模式中下标为j的失配时，应该从next[j]再开始匹配；

两个串，从0到j-1都是一样的，但是第j个不一样了。所以nextj=从0到 j-1的最大首尾相同子串下标+1；

找一个首尾相同的子串，它的下一位会不会与我的下一位相同呢？

如果还是不同就再找首尾相同的更小的子串；说起来很玄，代码如下：

```cpp
void calcnext(string pat){
    next[0]=-1;
    for(int i=1;i<pat.length();++i){
        next[i]=next[i-1]+1;
        while(next[i]!=0&&pat[i-1]!=pat[next[i]-1])next[i]=next[next[i]-1]+1;
        //123412 3 next[5]=3. if pat[next[5]]==pat[6] next[j]=next[j-1]+1;
        //123412 1 else next[j]=next[next[j]-1]+1;
    }
}
```





















