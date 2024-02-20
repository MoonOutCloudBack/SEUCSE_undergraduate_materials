# 09 virtual memory

## virtual memory

address binding在execute时，会出现VM的概念，

整个program的code并不是同时被需要，因此我们只load一部分，此时program大小不受内存大小的制约。

CPU->logical memory->virtual memory->physical memory。

virtual memory是面向程序设计的存储模型，提供巨大的地址空间，方便成效编写。

有一些VM的地址是不合法invalid的，如堆和栈之间的大块空间。

对一个CPU产生的LA：是否valid，是否装入present/inpresent。

backing store，后备储存。？？？

## demand paging

请求调页：只在需要某页的时候把它调入内存，而不是把整个程序全部调入内存。

可以理解为paging system+swapping：把需要的页swap in PM。

实现：lazy swapper+pager。

lazy swapper：直到page需要被用，才swap in它。

pager：当换入process时，猜测process被再次换出（context switch）之前会用到哪些页，只调入这些页。

### 硬件support

区分页面在内存/磁盘的valid-invalid-bit。

（是说，如果一次性把page全装进memory的话，就无需区分是否装入了）

此时page table结构：index是page number，PTE是frame number+valid-invalid bit。

valid-invalid bit：这里相当于present。v：in-memory，i：not-int-memory。

那这一页到底是否合法？根据PCB去check。若不合法，直接abort。

若v=invalid：page fault。处理：

- 不合法，about；合法，则page不在memory里。
- 找free frame；若没找到，找victim，若dirty则写回victim到disk。
- 从backing store找到这一页，把它swap into那个free frame。
- 改页表，现在这一页在memory了。v=valid。
- 重新执行page fault的指令，而不是执行下一条指令。

硬件支持：还需要instruction restart。

如果是block move指令，一条指令可以依次move好多B。如果目标地址和源地址还有重叠，把原来的数据改乱了？

因此需要回退/回溯机制。

页23/87：详细的处理page fault的流程。

## page replacement









logical memory & virtual memory：l->m->p。

LM是CPU产生的。



VM比PM大。PM和backing store是两码事，backing store应该是ca说的文件区吧。



demand paging：

请求调页。

只在需要这一页的时候把它调入memory。

paging system+swapping：把需要的页swap in PM。

对一个CPU产生的LA：是否valid，是否装入present/inpresent。

有一些VM的地址是不合法invalid的，如堆和栈之间的大块空间。

lazy swapper：直到page需要被用，才swap in它。

pager：当换入process时，猜测process被再次换出之前会用到哪些页，只调入这些页。



需要硬件支持：区分页面在内存/磁盘的valid-invalid-bit。

（是说，如果一次性把page全装进memory的话，就无需区分是否装入了）

page table：index是page number，PTE是frame number+valid-invalid bit。

valid-invalid bit：这里相当于present。v：in-memory，i：not-int-memory。

这一页到底是否合法？根据PCB去check。

若v=invalid：page fault。处理：

- 不合法，abort；合法，那至少不在memory里。
- 找free frame；
- 从backing store找到这一页，把它swap into那个free frame。
- 改页表，现在这一页在memory了。v=valid。
- 重新执行page fault的指令，而不是执行下一条指令。

硬件支持：instruction restart。

如果是block move指令，一条指令可以依次move好多B。如果目标地址和源地址还有重叠，把原来的数据改乱了？

需要回退/回溯机制。

页23/87：处理page fault的流程。



performance：

处理interrupt的指令、重新启动process、重新执行该指令花费时间都不多，花时间多的主要是I/O。

page fault rate p。
$$
EAT=(1-p)*T_{memory\_access}+p*(T_{page\_fault}+T_{swap\_page\_out}+T_{swap\_page\_in})
$$
p是千分之一的时候，时间就多了40倍。性能很糟糕。



优化：

swap的I/O加速，chunk更大，management更少。

把整个image放到【swap space】。

把页直接discard而非paging out。



inverted page table：

不重要。

反向页表不再包含有关进程逻辑地址空间的完整信息。

如果引用的页面当前不在内存中，则需要该信息请求分页需要这些信息来处理页面错误。

为了获得可用的信息，必须保留一个外部页表（每个进程一个）（可以在后台存储中）页面错误现在可能会导致虚拟内存管理器生成另一个页面错误，因为它在外部页面表中分页，需要在备份存储中定位虚拟页面



copy-on-write，COW：

parent/child process可以共享page frame，即使VM的page不一样。

只copy改动的页。

pool：free frame放在pool里，希望总是有一些free frame。

vfork()配合exec()使用，COW，parent悬挂等待，非常高效。



page replacement：

需要的支持：dirty bit，Prevent over-allocation of memory by modifying page-fault service routine to include page replacement

流程：

- 找desired page在disk上的位置；
- 找free frame：
  - 找到。
  - 没找到：选一个victim frame，若dirty就写回disk。
- 把desired page写到free frame，更新page table和frame table。
- 等到这一process拿到CPU时，重新执行page fault的指令。

需要的algo：Frame-allocation，Page-replacement。

evaluate这些algo：在reference string上跑。

reference string：每次访存的虚页号，重复的缩略成一个数字。



OPT algo：换掉【最长时间内不会被用】的页。





20210528 67

页40/87：最开始全是page faults，强制缺失。很容易犯错！！

OPT algo MIN没有用处，how can we look into the future? 但是可以作为评判标准，把别的做法与它做比。



现在在讲page replacement。

FIFO：

实现：维护一个FIFO queue。

Belady's Anomaly：frame（MEM中的位置）变多反而可能增加page faults的数量。

听说期末考试会考页42/88的图。



LRU：least recently used。

书上的翻译是错的：不是最近最少使用，而是【在最遥远的过去被最后一次使用】。

用counter实现：每次访问这一页时，counter<-当前时间，replace时遍历所有页。

用stack实现：double link form，每次访问某页就把它放在top，每次要改6个指针，但是replace时不需要搜索（要被换掉的页在bottom）。

LRU和OPT不会有Belady's Anomaly。这个结论很重要。

因为是stack algo。【n个frame时在MEM中的页】总是【n+1个frame时在MEM中的页】的子集。



LRU approximation：

LRU需要特殊硬件支持，并且还是很慢。

reference bit：

每一页都有一个bit，页被访问时置为1，替换掉bit为0的页。

若没有页bit=0？



Additional-Reference-Bits Algorithm：

8位，最左端记录这次访问，每次访问完右移1位。把这8位看作unsigned int，值最小的就是被换掉的page，这样一定能选出来一个page。



Second-chance algorithm：

FIFO+reference bit。

按clockwise遍历所有页：若rb=0，则换掉，break；若rb=1，则置为0。

真的是break吗？？



Enhanced Second-Chance Algo：

pair：(reference, modify)。

00：最近没用过，没改过：很合适，换掉！

01：最近没用过，改过：换掉时写回。

10：最近用过，clean：很可能再用。

11：最近用过，改过：很可能再用，换掉时写回。

好像是先polling一遍，发现00直接换，发现1x变成0x；没发现00就再polling一遍，此时肯定有00或01了。这一遍发现00换掉，没发现00就再polling，找一个01换。



其他的：counting algo：不常见。

LFU MFU。



Page-Buffering Algo：

了解即可，页53/88.



ppt上有一些performance的分析。



Page-Buffering Algorithms 

• Keep a pool of free frames, always 

– Then frame available when needed, not found at fault time – Read page into free frame and select victim to evict and add to free pool 

– When convenient, evict victim 

• Possibly, keep list of modified pages 

– When backing store otherwise idle, write pages there and set to non-dirty 

• Possibly, keep free frame contents intact and note what is in them

 – If referenced again before reused, no need to load contents again from disk 

– Generally useful to reduce penalty if wrong victim frame selected 



allocation of frames：

frame在进程间如何分配？

举个例子：一个move指令可能需要6个页，指令两个页（横跨页面），from 2个 to 2个。



fixed allocation：

equal：根据目前进程数量平分，pool里要维持一些free frame。

proportional：根据multiprogramming程度决定process sizes。



priority allocation：

若进程i出现page fault，替换一个它的页，或抢一个priority低于i的进程的页。

global replacement：。。。没听懂

local replacement：。。。



NUMA：Non-Uniform Memory Access

memory和memory之间并不是一样的！在多CPU多MEM的情况下，最好分配离run该thread的CPU更近的MEM。



thrashing：

a process is busy swapping pages in and out。frame实在太少。

当multiprogramming程度过高，所有process的frame都很少，它们只好一直swapping（I/O），然后os以为它们是I/O bound了，然后再引入一个process……很糟糕。



为什么demand paging work？因为locality model。

thrashing何时出现？Σsize of locality > total memory size





20210601 34

thrashing：# of frame＜size of locality。

working-set model：工作集模型。

Δ，working-set window，往前回溯page reference的个数，窗口大小。

WSS_i，working set size of process i：在most recent Δ中被access的page数量。

下标是进程！

ΣWSS_i：D，total demand frames，locality的approximation。

如果D＞m（PF个数），thrashing。



Keeping Track of the Working Set：

大致流程，问题。

approximation？？？



Page-Fault Frequency：More direct approach than WSS

控制PFF在l和u之间，通过给process frame/拿走process的frame。

为什么PFF不能过低？multiprogramming。



WSS和PFF的关系：

一个process跑的时候，locality keeps moving。locality移动的过程中，PFF很大。移到了某个locality之后，PFF变小了。



Memory-Mapped Files：不太重要。

进程的文件管理：内存映射的方式。

代码->image。map backing store和普通disk。

就是file和image用相似的方式处理：demand paging。

把file demand paging进MEM，然后就在page上读写，直到page被换掉。

就是不用I/O sys call了，直接写MEM。

什么时候写回呢？periodically或者close()时。



Memory-Mapped File Technique for All I/O：

explicitly？？？



Allocating Kernel Memory：

和user memory不同。

KM需要各种大小的memory：它有各种data structure，PCB等。

并且某些KM需要连续的memory：如memory for device I/O。

Often allocated from a free-memory pool。



Buddy Allocator：见ppt页81/87。

advantage：quickly coalesce unused chunks into larger chunk，合并是很简单的。



Slab Allocator：

cache由相同大小的object组成。cache满，但是里面的object可能是empty的。

slab：多个slab组成一个cache。

slab首址+object大小*编号。



prepaging：预取页面。

预取的页面有多少会被用？不用是一种浪费。省掉的page faults与多余调入的页的cost相比怎么样？

program structure：程序结构，交换内外层循环。

I/O interlock：

I/O联锁-页有时必须锁定到内存中

考虑I/O—用于从设备复制文件的页面必须通过页面替换算法锁定，以避免被选择逐出

将页固定到内存中



