# 07 deadlocks

## system model

系统模型，两种节点：processes和resources。

各种type的resources有不同个数的instance。

process可能会1. request；2. use；3. release 一个resource。

### resource-allocation graph

用resource-allocation graph来描述系统的状态。

边：process指向resources表示p需要r，r指向p表示r分配给p。

若无环则不死锁。若资源份数=1，则有环就死锁。若份数不=1，则具体分析。

ppt页11/44，第一个图无环不会死锁，第二个图有环死锁，第三个图不死锁。

第2个图：P1P2P3全在互相等待。

第3个图：因为现在看起来P2和P4终有一天会执行完，故现阶段没有死锁？

## 必要条件 & 如何处理

### deadlock的4个必要条件

- mutual exclusion：每一份资源都互斥地被使用。
- hold and wait：允许一个p拿着某些资源等其他资源的情况。
- no preemption：只有一个p自己决定放弃它现在占有的资源，这些资源才可用。
- circular wait：循环等待，r-a graph有环。

注意这只是必要条件，即使4个条件都满足也可能不deadlock。necessary but not sufficient。

circular wait和hold and wait有关系，好像某一个能推出另一个？

### 如何处理deadlock

保证system永远不会进入死锁状态：

- prevention：程序层面的逻辑，编程时破坏上述4个必要条件。
- avoidance：需要一些priori的information，是os层面的措施。

允许系统进入死锁状态然后恢复：

- detection；
- recovery：

假装死锁并没有发生，大多数os包括UNIX都是这样做的。

## prevention

prevention的语义：程序不能保证自己不死锁，只能通过一些逻辑尽量预防。

### mutual exclusion

一般来说不能违背，不能通过违背这个来避免死锁。

lock-free synchronization是怎么回事？？ppt页19/44。

### hold and wait

若一个process想要request一些资源，它目前不能占有任何资源。

想申请更多资源之前，应该放弃现有的所有资源。

all or none。不能占有一些等待一些。

书217页。“一种可以采用的协议是，每个进程在执行前申请并获得所有资源。可以这样实现：要求进程申请资源的sys call在所有其他sys call之前执行。”

书217页。“另外一种协议允许进程仅在没有资源时才可以申请资源。在它申请更多资源前，应释放现在已分配的所有资源。”

可能会starvation，资源利用率低，并发程度低。

### no preemption

书：若一个process等待别的资源，也就是不能立刻执行，那么它所有的资源都可以被抢占，也就是资源已经被隐式释放了。

若一个process发现其他资源不能马上给它，则它放弃现有资源。

出现livelock，ppt页18/44：两个th一遍一遍执行这些代码（锁lock1，若不能锁lock2，则放弃lock1然后再跑一遍这段代码），却总是得不到资源。解决方法是在循环回去之前加一个random delay。

### circular wait

强制规定申请资源的顺序，确保不成环。对资源进行全排序或规定层次结构，应用程序按照约定的顺序获取锁。

比如说要锁两个锁，若lock1的id>lock2的id，则先锁lock1，否则先锁lock2。

## avoidance

通过CPU scheduling。

（ppt页23/45的调度没有concurrency，T1和T2没有time interrupt。死锁和调度是两码事，我们现在只是想保证目前没有死锁。

avoidance的语义：我不调度给你资源，你当然不会死锁了，从而避免死锁。

需要关于进程的先验知识：每个p事先声明需要每一种资源的maximum number。

现在可用的资源、已分配的资源、最大需要资源：确定resource-allocation state。

动态调度，检查r-a state，确保不会陷入循环等待的局面。

### safe state

定义：若存在process的序列P1~Pn，保证Pi所需的资源可以被【目前available的资源+被Pj们占有的资源，j<i】满足。

这样，若Pi所需的资源暂时不能获得，则等它前面的process执行完。可以顺利从P1执行到Pn。

safe=>no deadlock，unsafe推不出deadlock。因为多份资源？

（unsafe推不出deadlock：ppt页28/44，第二个图，不一定死锁。

avoidance：保证程序时时刻刻都在safe state。

每当process申请资源，系统要判断若这个资源被assigned，state是否还safe。

### resource-allocation-graph algorithm

对每个资源只有1份的情况，single instance。

对r-a graph增加第3种边claim edge：从p指向r，表示p或许会申请这个资源。

假设Pi在申请Rj。只有把Pi指向Rj的request edge转向成assignment edge后不会形成环，Rj才能给Pi。环包括claim edge的虚边。

若形成带虚边的环，则若虚边变成实边，就接着死锁了。

### banker's algorithm

对每个资源多份的情况，multiple instances。

#### 符号约定

n：进程数；m：资源类型数。

Available：m维向量，资源还剩下多少。

Max：n*m的矩阵，进程最多需要多少。

Allocation：n*m的矩阵，进程已经被分配了多少。

Need：n*m的矩阵，进程可能还需要多少。

信息是冗余的：Need[i,j]=Max[i,j]-Allocation[i,j]。

#### safety algorithm

1. Work初始化为Available，Finish是n维boolean vector，初始化为false。
2. 找一个这样的进程Pi，若找不到就去第4步；
   - Finish[i]=false，还没完成。
   - Need[i]<=work[i]。
3. 把这个进程结束，Finish[i]=true，Work+=Allocation\[i]。
4. 若找不到进程是因为所有Finish都是true，则safe state。

就是说，可以找到一个安全序列，可以按照序列一个一个执行完。

#### resource-request algorithm

Request[i]是进程i现在【还】想要多少资源，m维向量。

遍历所有进程i，通过如下步骤来决定是否分配？

1. 若Request[i]<=Need\[i]，去第2步。否则raise error condition，process申请的资源数超过了它事先声明的maximum。

2. 若Request[i]<=Available，去第3步。否则，没法立刻把这些资源给Pi，Pi要等待。

3. 假装把资源给进程i了，把state改成下面这样：

   - Available-=Request\[i];
   - Allocation[i]+=Request\[i];
   - Need[i]-=Request[i]; need是可能需要的数量，会比目前的request大。

   若safe，则就这样分配。若不safe，则不分配给Pi，把state改回原来的样子。







20210507 67

banker's algorithm：

假设：

贷款上限、（等待借钱）、有借有还。



20210511 34

死锁考点很多的，死锁很重要的。

r-a graph，4个必要条件，措施们是什么意思，算法分别是什么。

容易错：超过max不可以。即使能找到safe sequence。



detection：

对single instance的情况：

维护一个wait-for graph：Pi->Pj若Pi在等Pj。若有环就有死锁。

periodically调用这个算法查环，查环是o(n^2)的。



multiple instance的算法：detection algorithm。

符号约定：Available、Allocation、Request（目前还想要多少）。

detection algo和safety algo貌似很相像。

finish表示是否会释放资源，true就是不可能释放资源，故若allocation=0就true。

若finish一直是false，即走到第4步，表示资源一直释放不掉，就是陷入死锁。

finish的意思：可前进。

可能会问detection和safety的区别。



roll back，回滚，对与每一个环都至少要杀掉/回滚一个process。



如何从deadlock中recover？

abort所有死锁进程 或 每次abort一个直到不死锁。

按照什么顺序选择这次abort的process？很多选择标准。



算cost函数时考虑回调的次数，有点像ageing。



类型相同的m个资源，进程数n，每个进程申请资源数量[1,m），和小于m+n。不会死锁。为什么？























