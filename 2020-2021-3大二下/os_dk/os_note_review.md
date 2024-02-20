# os review

20分判断，理性对待，把syllabus过一遍。

30分简答，送分题。？？

40分：原来和课件的题差不多，但是今年改形式了。要知其所以然。

10分：同步。自己从零开始写一个解决方案，代码都是自己写。

“不知道对不对…反正很简单。”

复习方式：看little book或刷题课件。



## 20分判断 & 30分简答

好啦现在我们来看syllabus！借助课件+笔记，弄懂syllabus上的概念。

### 13 I/O systems

polling和interrupt-driven是【及时响应硬件要求】的两种方案？

快设备：polling；慢：interrupt-driven。hybrid：先polling一会，然后等interrupt。

DMA是第三种方案。

### 12 file-system implementation

磁盘memory结构：super+inode bitmap+data bitmap+inode table+data blocks；

inode也被叫做FCB，file control block。

大多数都是小文件，＜2KB，因此direct pointer居多。

extents：pointer+length，一个pointer指向length个连续block。

FAT，file-allocation table，linked。NTFS，new technology file system with MFT。

3种allocation method：

- contiguous：现代文件系统不使用，但有一些hybrid。ext4，ntfs。
- linked：上文的fat。
- indexed：指针全在FCB中。ext2，ext3。

### 11 file-system interface

file descriptor：一个integer，进程私有，read/write sys call用它去读写文件。

file descriptor：进程私有，好像子进程也会继承。process第一个打开的文件的descriptor=3，0是std input，1是std output，2是std error。

hard link：创建一个新entry，对应不同的readable name，但是inode # 是一样的。inum一样就是同一个文件，我们找它的时候从inode table去找，就会找到同一块区域。

symbolic link：是一种特殊的type，可以说是另一种file。

hard/symbolic 比较：

把2 soft link到1上，remove 1，然后2就no such file or directory。

把2 hard link到1上，remove 1，2不受影响。

### 10 mass-storage systems

RAID不考，真是太好了！

这一章关于：

- secondary storage device的：物理结构，性能评价指标。I/O time。
- disk scheduling algo。和RAID。

一下子读写4KB。

T_I/O=T_seek+T_rotation+T_transfer。寻道，等它转到合适位置，传输数据。

T_seek_avg=1/3\*从最里到最外。T_rotation=1/2\*转一整圈。好像常有大题。

性能评价指标：R_I/O，rate，数据量/时间。

- random workload：按seek+rotation+transfer公式算，见ppt页14/48。
- sequential workload：就是max transfer rate，最大带宽。

- RPM：rotation per minute；好像计算会有大题？

disk scheduling：感觉要深度学习了。

- 有很多人想读磁盘（os，system/user process），因此给我们发来I/O request。
- 所以os维护一个request的queue，对于每一个disk/device维护一个。
- 现在我们有一堆request：我们按照什么顺序I/O最省时间呢？
- FCFS，first come first serve：计数单位是 head movement of xxx cylinders。
- SSTF，shortest seek time first：我现在去哪最近，我就去哪，会starve。
- SCAN，elevator：先去track 0并在这一路serve掉比开始track小的request，然后一路往上把所有其余都serve掉。注意不去最上面的track 199，只要把最靠上的183 serve掉就停止。
- C-SCAN：这个时间更好算：先往上一路serve并跑到最上面的199，然后接着【什么也不干】跑到最下面的0，然后从0往上一路serve。
- LOOK，SCAN的另一个版本：不再执着地跑到track 0，而是serve掉最底下的request就掉头往上。
- C-LOOK，C-SCAN的对应版本：不再执着地跑到两头（0和199）。
- 好像计算常考？
- SPTF，shortest positioning time first：考虑rotation时间的SJF？

原来的scheduling是os做，但现在是disk自己做了。

### 09 virtual memory

内容多起来了。但没有什么需要master的。

demand paging：只在需要某页的时候把它调入内存，相比于把整个程序全部调入内存。non-demand paging就是一下子把整个program全load进内存了。

page fault：就是valid-invalid bit=invalid，表示内存里没有这一页，包括无效VM页的情况。（如果一次性把page全装进memory的话，就无需区分是否装入了）

- 处理：先查VM->PM的页表，看看这一页是否合法，不合法abort。
- 然后找一个free frame或者swap out一个页，然后把这一页从backing store load进来，修改页表表示这一页现在valid。
- 最后重新开始那个指令，而不是下一条指令。

effective access time：(1-p)T_memory_access+p(T_page_fault_overhead+T_swap_page_out+T_swap_page_in)。

COW，copy-on-write：parent/child process可以共享page frame，即使VM的page不一样。只copy改动的页，write的时候再copy。

modify/dirty bit：记录是否被修改，修改才写回disk，否则discard，仅此而已。

reference string：一串页号，合并同类项后。

page replacement algo：当没有free frame时，要把哪一页swap out/discard呢？

- OPT，belady's MIN algo：换掉最长时间内不会被用的页。
- FIFO：belady's anomaly。
- LRU，换掉【在最遥远的过去被最后一次使用】的页。
  - 用一个counter来记录上一次访问的时间，换掉counter最小的那一个。
  - 用linked stack，把访问的页放在栈顶，一次动6个指针，换掉栈底。
- LRU approximation：
  - reference bit，访问时置为1，换掉为0的。
  - additional-reference-bit，8位，访问时把最高位置为1，每隔一段时间右移1位，当作无符号整数比较大小，换掉最小的。
  - second-chance，clock replacement。FIFO+reference bit。按clockwise遍历所有页：若rb=0，则换掉，break；若rb=1，则置为0。

belady's anomaly：FIFO时（LRU和OPT不会），frame变多，page fault数量反而上升。

equal/proportional/priority allocation：frame在进程间如何分配？

- equal：根据目前进程数量平分，pool里要维持一些free frame。
- proportional：根据multiprogramming程度决定process sizes。
- priority allocation：若进程i出现page fault，swap out一个它的页，或抢一个priority低于i的进程的页。

global/local replacement：global就是process之间可以互相牺牲frame，local就是替换时只能牺牲自己的frame。

thrashing：进程忙着把页换入换出。

working-set model：

- Δ：working-set window，一个固定的page reference数量。
- WSS是working-set size，最近Δ里访问了多少不同的页。下标i是进程i。
- 把所有进程的WSS求和，就是总共需要的frame数量，locality的近似。
- 得到WSS：把Δ分成n段，对每个页维护n个reference bit。n越大越精确。
- 比WSS更加直接的方法：PFF，page-fault frequency。低就拿掉进程的frame（更加multiprogramming），高就降低multiprogramming程度。
- locality移动的过程中，PFF很大。移到了某个locality之后，PFF变小了。

memory-mapped files：file和image用相似的方式处理：demand paging。不用I/O sys call了，直接写MEM。什么时候写回呢？periodically或者close()时。

哦，并不是所有页表都是进程私有的，反向页表inverted table就不是。

### 08 main memory

CPU可以直接access的storage：主存和registers，不是只有主存。

address translation：base & limit/segment/paging。

internal/external fragmentation。

base & limit：PA=base+VA；VA∈[0,limit]。VA在0到limit之间。

base和limit是放在MMU里的，MMU在CPU里面貌似。

segment：VA：segment # offset，PA=base+offset，base由segment作为index查段表得到。

paging：VA：VPN虚页号 # 页内地址；PA：PFN实页号 # 页内地址。PFN由VPN作为index查页表得到。

page size的讨论：考虑到xxx的影响，我们希望page size尽量大/小。

- fragmentation (small page)，小：internal fragmentation更小。
- page table size / page faults (large page)，大：页更大页表更小；类比cache的强制缺失和容量缺失。
- I/O overhead (large page)，大：对磁盘来说零碎的小I/O远远弱于整块的大I/O。顺序：转的时候都读了。零碎：不停地转以找到下一个读写的地方。若page小，各个page可能零散地放在磁盘的各个地方。
- locality (small page)，小：空间局部性，用更小的页精确定位各个数组。

一般来说页大小相对小，4KB或8KB。

TLB表项中ASID指示该VPN属于什么进程。

EAT，effective access time：(1+ε)α+(2+2ε)(1-α)。ε：一次访问TLB与一次访存时间比例。α：命中率。

若命中：TLB+访存。若不命中：TLB miss+访存页表+TLB建立新项+访存。

页表结构：VA=seg # VPN # offset。页表项前n位指示segment。

paging & segment，段页式管理。

hierarchical page table，多重页表：page directory table：管理页表。若一个PDE invalid，则不分配页面。需要PDBR和PDLR。

hashed（clustered）/inverted page table（PFN->VPN，整个系统只有一个）。

### 07 deadlocks

呀！死锁必考！想起来了！

system model：两种节点：process和resource。边：process指向resources表示p需要r，r指向p表示r分配给p。

4个必要条件：（押题了押题了，这个可能是简答。

- mutual exclusion：每一份资源都互斥地被使用。
- hold and wait：允许一个p拿着某些资源等其他资源的情况。
- no preemption：只有一个p自己决定放弃它现在占有的资源，这些资源才可用。
- circular wait：循环等待，r-a graph有环。

prevention/avoidance/detection/recovery的异同：

- prevention：程序层面的逻辑，编程时破坏上述4个必要条件。
- avoidance：需要一些priori的information，是os层面的措施。
- detection，recovery：允许系统进入死锁状态然后恢复。

safe state：若存在process的序列P1~Pn，保证Pi所需的资源可以被【目前available的资源+被Pj们占有的资源，j<i】满足。这样，若Pi所需的资源暂时不能获得，则等它前面的process执行完。可以顺利从P1执行到Pn。unsafe推不出deadlock。

resource-allocation graph alg：对每个资源只有1份的情况，single instance。增加虚边claim edge。若request edge转向为assignment edge之后连带虚边不会形成环，则分配。

banker's alg：对每个资源多份的情况，multiple instances。超重要！

- safety alg：不断找need<=work的进程，finish它们，直到所有进程执行完（safe state）或执行不下去（unsafe）。
- resource-request alg，对进程i：
  - 如果request\[i]>need\[i]，不合法！注意这一步。
  - 若request\[i]<available，继续，否则等着吧。
  - 假装给他，available-=request\[i],allocation\[i]+=request\[i],need\[i]-=request\[i]，看看是否safe。unsafe就不分配。

wait-for graph：对single instance的情况，Pi->Pj若Pi在等Pj。若有环就有死锁。periodically调用这个算法查环，查环是o(n^2)的。

detection：multiple instance的算法，可能会问detection alg和safety alg的区别。一点点小区别：detection初始化时对allocation\[i]=0的进程直接finish\[i]=true，finish表示是否可以以后释放资源。还有就是detection比较的是真实的request，safety是priori的need。

### 前6章

线程不能拥有自己的signal handler，一个进程用一个handler。（考过）

管道只能单向通信。如何双向通信呢？接两个管道就好了。（考过）

scheduler activation是用户线程库和kernel之间的通信方案，通过light weight process这一数据结构。

scheduling criteria有哪5个？CPU利用率，throughput，turnaround time，response time，waiting time（ready queue）；

scheduling algos：FCFS，SJF，priority，RR。MLFQ。

multilevel queue is one queue。是一个queue而非多个。

asymmetric multiprocessing，不对称的，single-queue multiprocessor scheduling，多处理器只维护一个queue（这个queue可能是multiqueue），会有cache affinity的问题。symmetric，对称的，one queue per CPU。但是有可能会负载不平衡，load imbalance，一个CPU很忙但另一个的queue是空的。

- 互斥访问：mutual exclusion。
- 空闲让进：progress。若目前没有process在critical section，并且我提出了request，那么我等的时间应该不是无穷。
- 有限等待：bounded waiting。我提出request进入critical section，在我之前进入的process数量应该有限。

peterson's solution。

### 以前考过的简答

什么是临界区问题，临界区问题与同步问题的不同。？？

external/internal fragmentation。





## 40分大题

基本可以看出大题是什么了！

CPU scheduling。

mutex。不一定。

process/pthread APIs。

死锁。必然考！

页置换，写reference string，用各种algo模拟。

页表计算，访存时间计算，磁盘时间计算，磁盘调度。感觉可能搞成1个大题。







## 10分semaphore





