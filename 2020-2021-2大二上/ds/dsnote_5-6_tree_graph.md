# 5-6章的笔记

## 树 Tree

度：子女个数；树高：叶节点/平凡树高为1，深度是树高/树高-1（目前的定义是树高，第七/八章的定义是树高-1）；

二叉树：第k层最多有$2^{k-1}$个节点，高为k的树最多有$2^k-1$个节点；

设度为1的节点n1个，n2，n0，则n0+n1+n2-1=2n2+n1 => n0-1=n2；

满二叉树：填满；完全二叉树：一层一层顺序填；从1开始编号，lch是2i, rch是2i+1；、

二叉树的数组表示；树状数组？

### 二叉树的遍历 Traversal

得到节点个数，表达式前缀中缀后缀什么的；

#### 中序 inorder

中序遍历lch，访问root，中序遍历rch；

p初始为root；若p非NULL：入栈，去lch；取p为top，pop，访问p，p去rch；循环条件是栈非空或p非NULL；

```cpp
void inorder(node* root){
    if(root->lch)inorder(root->lch);
    visit(root);
    if(root->rch)inorder(root->rch);
    //后面前序和后序都差不多，不写递归实现了
}

void inorder(node* root){
    stack<node*> s;
    s.push(root);node* p = root->lch;
    while (p||!s.empty()) {
        while (p)s.push(p), p = p->lch;
        p = s.top(), s.pop();visit(p);
        p = p->rch;
    }
    //用五边形树模拟成功
}
```

#### 前序 preorder

访问root，前序遍历lch，前序遍历rch；

ppt上也没有给出非递归的实现，复习时间真的是不够了，我就感性分析一下；

若p非NULL：入栈，访问，去lch；取top，pop，p赋为rch；p初始为root；循环条件是栈非空或p非NULL；

#### 后序 postorder

后序遍历lch，后序遍历rch，访问root；

感性分析非递归：

p初始为root；若p非NULL：入栈，去lch；取p为top，若p已访问rch访问p，若p未访问rch更改状态为已访问rch并p去rch；循环条件是栈非空或p非NULL；

#### 层次序 levelorder

队列；这个比较好想；

```cpp
void levelorder(node* root){
    queue<node*> q;q.push(root);
    while(!q.empty()){
        node* p=q.front();visit(p),q.pop();
        if(p->lch)q.push(p->lch);
        if(p->rch)q.push(p->rch);
    }
}
```

#### 确定一棵二叉树

前序中序/中序后序/层次序+前序还是中序貌似也可以，做题之后估计就知道了；

给定一个前序排列，可能有多少种树？

1(c0)，1(c1)，2，5，14，42，卡特兰数；
$$
\frac{C_{2n}^n}{n+1}
$$

### 线索二叉树 Threaded Binary Tree

中序线索二叉树：若无lch，lch指向中序遍历的先驱；若无rch，rch指向中序遍历的后继；

用两个字段leftThread rightThread记录线索/子树；

#### 插入新节点

将p插入到s的lch；p->lch=s->lch,s->lch=p,p->rch=s;

s的lch原来是前驱线索，p肯定更先被中序遍历，修改前驱线索；p的后继是s；

将p插入到s的rch；p->rch=s->rch,s->rch=p,p->lch=s; 感觉上是对称的；

### 堆 Heap

最小堆：子女大于父母，root最小；最大堆：子女小于父母，root最大；

#### 调整算法siftDown：

```cpp
void siftDown(int start, int m){
    //私有函数: 从结点start开始到m为止（可以取到m）, 自上向下比较,
	//如果子女的值小于父结点的值, 则关键码小的上浮,
	//继续向下层比较, 将一个集合局部调整为最小堆。
    int i=start;int j=2*i;
    while(j<m){
        if(j<m&&a[j]>a[j+1])++j;
        if(a[j]>a[i])break;
		int temp=a[i];a[i]=a[j],a[j]=temp;
        i=j,j=2*i;
    }
}
```

#### 调整算法siftUp：

```cpp
void siftUp(int start){
    //私有函数: 从结点start开始到结点0为止, 自下向上比较,
	//如果子女的值小于父结点的值, 则相互交换,
	//这样将集合重新调整为最小堆。
    int i=start,j=i/2;
    while(j>0){
        if(a[j]<a[i])break;
        int temp=a[i];a[i]=a[j],a[j]=temp;
        i=j,j=i/2;
    }
}
```

#### 初始化

```cpp
void build(){
    //从cursize/2即倒数第二层节点开始向下调整，时间复杂度是O(n)
    for(int i=cursize/2;i>0;--i)
        siftDown(i,cursize);
}
```

#### 插入

```cpp
void push(int data){
    a[++cursize]=data;
    siftUp(cursize);
}
```

#### 删除

```cpp
int pop(){
    int ans=a[1];a[1]=a[cursize--];//取值范围是[1,cursize]
    siftDown(1,cursize);
    return ans;
}
```

### 二叉搜索树 Binary Search Tree

搜索用递归/迭代写；

删除叶节点/只有一子女的非叶节点；

删除有两子女的非叶节点：用lch中最大的或rch中最小的替代它；

### 哈夫曼树 Huffman Tree

外部路径长度EPL=所有外节点/叶节点到根的路径长度之和；

内部路径长度IPL=所有内节点到根的路径长度之和；

扩充二叉树中只有度为2的内节点和度为0的外节点/叶节点，每个节点有权值；

扩充二叉树的带权路径长度 定义为 树的各外结点到根的带权路径长度之和；

霍夫曼编码，决策树；

### 森林 Forest

树的表示：广义表表示，双亲表示（"节点: parent" 的键值对），子女链表表示；

子女-兄弟表示：data firstChild nextSibling；

#### 森林的二叉树表示

根是T1的root；左子树是T1的子树们形成的森林；右子树是T2…Tn形成的森林，递归写就行了应该；

#### 森林的先根遍历

访问T1的root，先根遍历T1的子树们形成的森林，先根遍历T2…Tn形成的森林；

二叉树的前序遍历；

#### 森林的后根遍历

后根遍历T1的子树们形成的森林，后根遍历T2…Tn形成的森林，访问T1的root；

二叉树的后序遍历；

#### 森林的层次序遍历

依次遍历所有树的root；依次遍历所有树的第二层节点；……

可以用栈，但是不能用二叉树表示了；

### 并查集

沿路径修改什么的；

## 图 Graph

有向图边<u,v> u tail, v head;

无自环无重边，回路不一定简单，可以绕成8字（考过还错了）；也就是回路不一定是环（首尾相接的简单路径）；

表示：邻接矩阵，有向边从行到列；邻接表，链表样子；

邻接表算出度，反向邻接表算入度；正交表，希望不要考；

### 图的遍历 Traversal

#### 广度优先遍历 Breadth Frist Search BFS

```cpp
bool* visited=new bool[n];
memset(visited,false,sizeof(visited));
void bfs1(int start){
    queue<int> q;q.push(start);
    while(!q.empty()){
        int k=q.top();q.pop();
        if(visited[k])continue;
        visit(k),visited[k]=true;
        for(int i=0;i<e[k].size();++i)
            if(!visited[e[k][i]])q.push(e[k][i]);
    }
}
void bfs2(int start){
    queue<int> q;
    visited[start]=true,visit(start),q.push(start);
    while(!q.empty()){
        int k=q.top();q.pop();
        for(int i=0;i<e[k].size();++i)
            if(!visited[e[k][i]])
                visited[e[k][i]]=true,visit(e[k][i]),q.push(e[k][i]);
    }
}
//bfs1和bfs2貌似是一样的，我也不确定，过会再说
```

邻接矩阵O(n^2)，**邻接表O(n+e)**；

#### 深度优先遍历 Depth Frist Search DFS

```cpp
bool* visited=new bool[n];
memset(visited,false,sizeof(visited));
void dfs(int k){
    visit(k);
    for(int i=0;i<e[k].size();++i)
        if(!visited[e[k][i]])visited[e[k][i]]=true,dfs(e[k][i]);
}
```

邻接矩阵O(n^2)，**邻接表O(n+e)**；

### 生成树 Spanning Tree

dfsbfs生成生成树；

关节点：删这个点之后图会有2个及以上连通分量；

没有关节点的连通图：重连通图；

#### 最小生成树 Minimum Spanning Tree

贪心策略；

##### kruskal

将边对权值排序；选未选过的权值最小的边，若不成环，则加入MST；选n-1条；

排序用快排nlogn，判环用并查集；

```cpp
void kruskal(){
    sort(e,e+m);int c=0;
    for(int i=0;i<n-1;++i){
        while(find(e[c].from,e[c].to))++c;//若边c的首尾属于同一连通分量，取下一边
        MST[i]=e[c];uni(e[c].from,e[c].to);++c;//合并，取下一边
    }
}
```

排序O(eloge)，或者一次寻最小权值边用优先队列O(loge)，初始化优先队列O(e)；

反正最后复杂度是O(eloge)；

##### prim

从连通网络中的某一顶点u0出发；

选择与它关联的具有最小权值的边(u0 , v), 将其顶点加入到生成树顶点集合U中。

每一步从一个顶点在集合U中，另一个顶点不在集合U中的各条边中，选择权值最小的边(u, v)加入到MST里；

分析一下：

需要一个判断顶点u是否在MST集中的bool数组isin；

每次for循环遍历所有顶点，若其isin遍历其边，若边的to非isin比较其权值，每次都是O(e)；

wy老师她用堆搞prim，我不知道她是怎么搞的。

嗯，wy老师的prim复杂度貌似是eloge；

可能是首先维护一个堆，每次找边的时候若两个都isin就pop，最后会找到一个；

然后对于新加入的v节点，插入一下它的边；
$$
很抱歉，刚刚考试通知出来了，我没有时间有条理地整理内容了；\\
接下来的内容可能会没什么条理。
$$

### 最短路 Shortest Path

### AOV Activity-on-Vertex

偏序关系，无环；

#### 拓扑排序 Topological Sorting

从偏序关系到全序关系；

list a vertex with no predecessor;

then delete this vertex together with all edges leading out of it from the network.

Repeat the above;

until all vertices have been listed or all remaining vertices have predecessors. 

为了建立入度为零的顶点栈,可以不另外分配存储空间, 直接利用入度为零的顶点的count[ ]数组元素.

设立一个栈顶指针top, 指示当前栈顶位置, 即某一个入度为零的顶点。

栈初始化时置top = -1。

count[top]记录的是前一个无前驱节点下标；链接栈啊，记录的都是前一个被装入的节点。

复杂度O(n+e)；先扫描所有边维护前驱计数，e；

再扫描所有顶点一个一个删，删的时候把e个边都删了，n+e；

### AOE Activity-on-Edge

如果在无 有向环 的带权有向图中；

用有向边表示一个工程中的活动 (Activity)，用边上权值表示活动持续时间 (Duration)，用顶点表示事件 (Event)；

这样的有向图叫做用边表示活动的网络, 简称 AOE ( Activity On Edges ) 网络；

路径长度最长的路径叫做关键路径(Critical Path)；

Ve(i)：事件Vi （顶点）的最早可能开始时间，是从源点V0 到顶点Vi 的最长路径长度。

Vl[i]：事件Vi 的最迟允许开始时间，是在保证汇点Vn-1 在Ve[n-1] 时刻完成的前提下，事件Vi 的允许的最迟开始时间。

e[k]：活动（边）ak (k=<i,j>) 的最早可能开始时间，是从 源点V0 到 顶点Vi 的最长路径长度。e[k] = Ve[i]；

l[k]：活动ak 的最迟允许开始时间，l[k] = Vl[j] - dur(<i,j>) 其中, dur(<i,j>)是完成 ak 所需的时间。

l[k]-e[k]：时间余量，最迟-最早，==0为关键活动。

注意，l、e、和余量都是对活动研究的。

#### 关键路径 Critical Path

先排拓扑序，然后递推算；

正向算Ve，对每个顶点遍历一次边，O(n+e)；

反向算Vl；对每个顶点遍历一次边，向后算，if (Vl[k]-dur < Vl[j]) Vl[j] = Vl[k]-dur；

仅计算 Ve[i] 和 Vl[i] 不够，还须计算 e[k] 和 l[k]；

遍历边；e[k] = Ve[i]，l[k] = Vl[j] - dur(<i,j>)；若为关键活动则输出；

（话说如果题目中出现DAG就是在暗示用拓扑排序搞事情，比如求最短路什么的；遇到一个这样的题；















