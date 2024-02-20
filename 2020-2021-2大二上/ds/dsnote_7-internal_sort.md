# 内排序

## 插入排序

每步将一个待排序的元素；

按其排序码大小，插入到前面已经排好序的一组元素的适当位置上；

直到元素全部插入为止；

### 直接插入排序

```cpp
for(int i=left+1;i<right;++i){
    int data=a[i],j=i;
    while(j>left&&a[j-1]>data)a[j]=a[j-1],--j;
    a[j]=data;
}
```

主程序执行n-1趟；

最好情况每趟比较1次共n-1次，不移动；

最坏情况每趟比较i次，每比较一次移动一次，比较=移动=n(n-1)/2次；

时间复杂度$o(n^2)$。稳定。

### 链表插入排序

```cpp
for(int i=left+1;i<right;++i){
    int data=a[i].data,j=left;
    while(a[j].next<i&&a[a[j].next]<=data)j=a[j].next;
    a[i].next=a[j].next,a[j].next=i;
}
```

稳定。

### 折半插入排序

```cpp
for(int i=left;i<right;++i){
    int low=left,high=i-1,data=a[i];
    while(low<=high){//取low作为位置
        int mid=(low+high)/2;
        if(a[mid]>data)high=mid-1;
        else low=mid+1;
    }
    for(int j=i;j>low;--j)a[j]=a[j-1];
    a[low]=data;
}
```

平均性能比直接插入排序快；

排序码比较次数**与待排序元素序列的初始排列无关**，仅依赖于元素数。插入第 i 个元素时，比较$\lceil log_{2}{i}\rceil+1$次确定插入位置。

比较$o(nlogn)$，移动$o(n^2)$；

稳定。

### Shell Sort

```cpp
int gap=right-left+1;
while(gap>=1){
    gap=gap/3+1;
    for(int start=0;start<gap;++start){
        for(int i=start;i<right;i+=gap){
            int data=a[i],j=i;
            while(j>start&&a[j]>data)a[j]=a[j-gap],j-=gap;
            a[j]=data;
        }
    }
    if(gap==1)break;//再操作一遍也是一样的了
}
```

取一个整数 gap < n 作为间隔，将全部元素分为 gap 个子序列，所有距离为 gap 的元素放在同一个子序列中；

在每一个子序列中分别进行直接插入排序；

gap从大到小；

**不稳定！！**

## 交换排序

两两比较待排序元素的排序码；

如果发生逆序（即排列顺序与排序后的次序正好相反）， 则交换之；

直到所有元素都排好序为止。

### Bubble Sort

```cpp
for(int i=0;i<n-1&&change;++i){
    change=false;
    for(int j=0;j<n-1;++j){
        if(a[j]>a[j+1]){
            int temp=a[j];a[j]=a[j+1],a[j+1]=temp,change=true;
        }
    }
}
```

最坏：最外层循环排序n-1趟，内部比较n-1次；

最好：外层1趟，比n-1次；

时间复杂度是$o(n^2)$。

稳定。

### Quick Sort

任取待排序元素序列中的某个元素作为基准；

按照该元素的排序码大小，将整个元素序列划分为左右两个子序列：

- 左子序列中，所有元素的排序码都小于或等于基准元素的排序码；

- 右子序列中，所有元素的排序码都大于基准元素的排序码；

把基准元素放在在这两个子序列中间；

分别对左右两个子序列重复施行上述方法；

直到所有的元素都排在相应位置上为止。

```cpp
int Partition(int low,int high){
    int pivotpos=low,pivot=a[low];
    for(int i=low+1;i<high;++i)//若无特别说明，都是[low,high)
        if(a[i]<pivot){
            ++pivotpos;
            if(pivotpos!=i){
                int temp=a[i];a[i]=a[pivotpos],a[pivotpos]=temp;
            }
        }
    a[low]=a[pivotpos],a[pivotpos]=pivot;
    return pivotpos;
}
void QuickSort(const int left,const int right){
    if(left<right){
        int pivotpos=Partition(left,right);
        QuickSort(left,pivotpos-1);
        QuickSort(pivotpos+1,right);
    }
}
```

更高更妙的代码：（仅供参考）

```cpp
void quicksort(int l,int r)//两边都取到，[l,r]
{
    int i=l,j=r,mid=a[(l+r)/2];
    do{
        while(a[i]<mid)++i;
        while(a[j]>mid)--j;
        if(i<=j)myswap(i,j),++i,--j;
    }while(i<=j);//结束时i比j大1
    if(l<j)quicksort(l,j);
    if(i<r)quicksort(i,r);
}
```

存储开销$o(log_2n)$；

平均计算时间$o(nlogn)$；

最坏：n-1趟，第i趟比较 n-i 次。计算时间$o(n^2)$；

**不稳定**。

#### 快排的优化

1. a1=a[low],a2=a[high],a3=a[mid]，三者取中做pivot；
2. n<9时插入排序；
3. n<9时return，最后一遍直接插入排序；

## 选择排序

每一趟（例如第 i 趟, i = 0, 1, …, n-2）在后面 n-i 个待排序元素中，选出排序码最小的元素，作为有序元素序列的第 i 个元素；

待到第 n-2 趟作完，待排序元素只剩下1个, 就不用再选了。

### 简单选择排序

```cpp
for(int i=left;i<right-1;++i){
    int mini=i,data=a[i];
    for(int j=i+1;j<right;++j)
        if(a[mini]>a[j])mini=j;
    a[i]=a[mini],a[mini]=data;
}
```

最好：比较 n-1-i 次，不移动；

最坏：比较 n-1-i 次，移动三次：data=a[i]，a[i]=a[mini]，a[mini]=data；

**不稳定**。

### 堆排序

堆初始化；若从小到大排序，则最大堆；

将top与最后一个元素交换；

调整前n-i个元素为新堆；共n-1趟；

```cpp
void siftDown(const int start,const int m){
    //从结点start开始到m自上向下比较,
	//如果子女的值大于双亲的值, 则相互交换, 将一个集合局部调整为最大堆。
    //左闭右开，不取到m
    int i=start,j=2*i+1;
    //j=2*(i+1)-1=2*i+1，以0为根，从0开始建树
    temp=a[i];
    while(j<m){
        if(j+1<m&&a[j]<a[j+1])++j;
        if(temp>=a[j])break;
        else{
            a[i]=a[j];
            i=j,j=2*i+1;
        }
    }
    a[i]=temp;
}
void HeapSort(int n){
    for(int i=(n-2)/2;i>=0;--i)//关于i的初值：2*i+1=n-1
        siftDown(i,n);
    //从后至前调整堆。从倒数第二层的最末一个节点开始调整堆。
    for(int i=n-1;i>=0;--i){
        int temp=a[0];a[0]=a[i],a[i]=temp;
        siftDown(0,i);
    }
}
```

时间复杂度：$o(n)+o(nlog_2n)=o(nlog_2n)$；

空间复杂度$o(1)$；

**不稳定**。

选择排序貌似都不稳定。

## 归并排序

将两个或两个以上的有序表合并成一个新的有序表。

### 二路归并排序

设初始元素序列有 n 个元素，首先把它看成是 n 个长度为 1 的有序子序列（归并项）；

然后开始一趟趟的两两归并；

```cpp
void merge(int src[],int sto[],const int left,const int mid,const int right){
    int k=left,i=left,j=mid;
    while(i<mid&&j<right){
        if(src[i]>src[j])sto[k++]=src[j++];
        else sto[k++]=src[i++];
    }
    while(j<right)sto[k++]=src[j++];
    while(i<mid)sto[k++]=src[i++];
}
void MergePass(int src[],int sto[],const int n,const int len){
    int i=0;
    while(i+2*len<=n){
        merge(src,sto,i,i+len,i+2*len);
        i+=2*len;
    }
    if(i+len<=n) merge(src,sto,i,i+len,n);
    else for(int j=i;j<n;++j)sto[j]=src[j];
}
void MergeSort(const int n){
    int len=1;int* b=new int[n];
    while(len<right-left){
        MergePass(a,b,n,len);len*=2;
        MergePass(b,a,n,len);len*=2;
    }
}
```

时间复杂度$o(nlog_2n)$；

空间复杂度$o(n)$；

稳定。

#### 链表实现和递归

递归的话，就是在MergeSort中先调用两个小MergeSort再去合并。

## 分配排序

### 基数排序

排序码(K1, K2, ..., Kd)；

最高位优先MSD (Most Significant Digit first)：先排K1，再排K2，…

最低位优先LSD (Least Significant Digit first)：先排Kd；

- 不需要再分组，直接对整个元素组排
- 基数排序

```cpp
const int radix=10;
void RadixSort(int d){
    int rear[radix],front[radix];
    for(int i=0;i<n-1;++i)a[i].link=i+1;
    a[n-1].link=0;//初始化，循环链表
    int current=1;//从头开始
    for(int i=0;i<d;++i){
        for(int j=0;j<radix;++j)front[j]=0;//初始化箱子，空箱子
        while(current!=0){
            int k=a[current],q=0;
            while(q<i)++q,k/=radix;k%=radix;//取第i+1位
            if(front[k]==0)front[k]=current;
            else a[rear[k]].link=current;
            rear[k]=current,current=a[current].link;
        }
        int j=0;while(front[j]==0)++j;
        a[0].link=current=front[j],last=rear[j];
        for(int k=j+1;k<radix;++k){
            if(front[k]!=0)
                a[last].link=front[k],last=rear[k];//链接起来
        }
        a[last].link=0;//循环链表
    }
}
```

总时间复杂度$o(d(n+radix))$；

增加n+2*radix个附加链接指针；

**稳定**！基数排序是稳定的诶。







