# 08 main memory

为了方便，程序员都拿VA编程。这一章主要在说VA->PA的转换，以及相应的内存管理。

## 背景

代码program->可执行的镜像image->放到内存，然后才能run。

CPU可以直接access的storage：主存和registers，不是只有主存。

### address binding

将instruction和data与绝对地址binding。

- compile time：如果知道程序将要被放在那里，kernel的代码可能会这样。VA对应唯一的PA。
- load time：装入内存同时确定指令/数据的绝对地址。VA对应唯一的PA。
- execution time：process运行期间，在主存中的位置到处乱变。VA不对应唯一的PA。

### dynamic loading & linking

- dynamic loading：只有被调用时才load进内存；
- static loading：一股脑全load进内存？
- dynamic linking：系统库和程序代码在生成image时就link好。
- static linking：执行时动态linking。动态链接库dll。

CPU产生的LA从0开始。

## address translation

将进程放在MEM中，以对进程透明的方式，给process occupy the whole machine的错觉。

透明是虚拟化的核心。

要translate从VA到PA，并且时空上efficient。

如何实现address translation？

### base and limit

最原始的方案。

base and limit：基地址/界限地址寄存器，限定每一个进程的空间。

满足透明性，protection，在时间上高效但是空间上并不高效。

process内stack和heap之间的空间是internal fragmentation，内部碎片。

#### 地址转换

PA=base+VA；VA∈[0,limit]。这样先checkVA后变换省一次地址变换时间。

若VA>limit register，则trap一个addressing error。

#### 硬件/os support

用MMU实现address translation。context switch时，换一对base和limit。

base和limit是放在MMU里的，MMU在CPU里面貌似。

一个MMU per CPU，因此一对base & limit per CPU。

硬件：特权指令（维护base limit reg），若越界/特权指令在u mode执行raise异常

os：管理base limit reg，处理异常。

## segmentation

第二种address translation的方案。

每一个process可能会有不同的segment，数据段代码段堆栈段等。

每一个segment有base/limit。

segment的分配是任意的，以段为单位塞到MEM的某一个位置。

### 地址转换

VA：segment # offset，用前n位表示在哪个segment。

PA=base+offset，base由segment作为index查段表得到。

offset需判断是否∈[0,limit]。

### issue

os support：segment registers。

每一个segment彼此大小不同，很难利用segment之间的space。

导致external fragmentation：简答题，与internal的比较！

## 如何管理段间的free space

压缩compaction是一个解决方案，但是expensive。

### 利用free space

mechanisms：split，把空间切为两部分。coalesce，把相邻的空间merge together。

policies：

- best-fit，smallest-fit：选当前fit的最小的free space。

- worse-fit；
- first-fit；
- next-fit（指针）。

衡量标准：内存利用率，efficiency。

其实first-fit和best-fit不分上下，但是first-fit显然更快。

ppt页27/72，maybe考试内容。

## paging

paging非常重要。paging之于期末考试，就和同步之于期中考试。

tradeoff between ex/in fragmentation。

把VM切分成相同大小的区域，pages。每一页相同大小。

将PM分为抽屉一样的page frame帧/页帧，一个frame可以contain一个VM page。

### 地址变换

page table：mapping，这个VP（virtual page）对应哪个PF（page frame）？

也就是VPN->PFN的mapping。

大多数页表都是一个process有一个，但是不是全部！！（判断题）因为每一个process都有它独有的virtual memory space。

VA：VPN虚页号 # 页内地址；PA：PFN实页号 # 页内地址。

### 硬件support

进程的页表在哪里？放在kernel physical memory而非MMU，因为它很大。

Page-table base register (PTBR)和page-table length register (PTLR)放在MMU里。

ppt页35/72，页大小4KB，2^20个page，页表大小为2^20*4B=4MB。

### PTE

页表中不需要VPN，VPN作为索引。

PTE内容：PFN、valid bit、present bit、dirty bit、reference bit（伪LRU）、访问保护位（user mode process是否可以access）。

### page size

啊，容易考！考虑到xxx的影响，我们希望page size尽量大/小。

- fragmentation (small page)：internal fragmentation更小。
- page table size / page faults (large page)：页更大页表更小；类比cache的强制缺失和容量缺失。
- I/O overhead (large page)：对磁盘来说零碎的小I/O远远弱于整块的大I/O。顺序：转的时候都读了。零碎：不停地转以找到下一个读写的地方。若page小，各个page可能零散地放在磁盘的各个地方。
- locality (small page)：空间局部性，用更小的页精确定位各个数组。

一般来说页大小相对小，4KB或8KB。

#### popular page size

4KB页大小，32位VA，4B的PTE。一页能装1K的PTE。

32位VA：10 PDoffset+10 页内页表offset+12 页内offset。

若64位VA，则PDoffset有42位，PD也超级大。因此有时候会更多层页表。

### 好处

- flexibility：无论process的user-level逻辑怎么用内存，都可以用这种方式。
- simplicity：好管理。
- efficient：用掉的frame之间没有碎片。

## TLB

paging方案中，一个page里有internal fragmentation，但是不大。空间上efficient。

但是它太慢了，extra memory references are costly。

执行一个访存指令：访存查页表VA->PA，根据PA取指，根据指令内容（数据的VA）查页表VA->PA，根据PA取数据。4次访存，访存本来就慢。

TLB：translation lookaside buffer，address-translation cache。

TLB是全局的，在MMU里。TLB是全局的！！

TLB translation的算法：页41/72。

miss：原来由硬件处理，现在由os处理。

### TLB表项

VPN不是index。其实可能组相联，部分index部分tag。

VPN（的一部分），PFN，V，protection bits，脏位，ASID（指示进程）。

### issue

return-from-trap：

之后执行哪一条指令？当前指令，而不是下一条。

（正常情况下是下一条，保证context switch不犯错。

TLB-miss handling：

若handler的指令也TLB-miss呢？那就继续调用miss handler，然后继续miss…所以TLB miss handler要一直在physical memory里，或在TLB中保留一些条目以供永久有效的转换。反正它不能miss。

context switch：

如何保证不用到别的process的TLB entry？上下文切换时清空TLB（inefficient），或用address-space identifier ASID指示进程号。

其实两个进程不同的VPN可能指向同一个PFN，shared pages。

### EAT

effective access time： $EAT=(1+\epsilon)\alpha+(2+2\epsilon)(1-\alpha)$；

ε：一次访问TLB与一次访存时间比例。α：命中率。

若命中：TLB+访存。

若不命中：TLB miss+访存页表得到PA+直接拿PA访存 / TLB miss+访存页表+TLB建立新项+访存。

原来缺失时ε系数是1，但是按照TLB缺失-建立新项-再访问TLB的逻辑应该是2，所以为了避免歧义，题目中会说TLB访问时间忽略不计。

## 页表的结构

一个进程的VA空间巨大，堆栈之间的VA是无效的，相应的页表项也是无效的。

page table中的internal fragmentation。

因此用hybrid approach：

### paging & segment

段页式管理。对每一个segment（而非每个process）维护一个page table。

虚拟地址划分：VA=seg # VPN # offset。页表项前n位指示segment。

对一个segment需要有两对base/limit，一个指示segment本身，一个指示segment的页表。

页表可以任意大小，找到free space 安置页表更难了。

### hierarchical page table

多重页表，很重要。

基本思想：页表大小可能比页更大，把页表按页大小分割。

若一整页的页表都invalid（即根本没有东西存在对应的VA上，这个VA不对应真实PA，不是未装入主存unpresent），则【不给页表分配真实的页面】，这一页无效的页表只logically存在。

page directory table：管理页表。

- 由page directory entries（PDE）组成。
- 若一个PDE invalid，则不分配页面。见ppt页55/72。
- 需要PDBR和PDLR。

two level paging二级页表：

- advantages：如果construct的比较好，页表将会正好装在整页里。

- disadvantages：time-space tradeoff，对os/硬件来说更复杂。

### hashed page table

处理大于32位地址空间的常用方法。

用虚页号的hash value索引到页表，这个表有一串element，这一串element都被hashed到这个页表了。先找到对应页表，再沿着chain一个一个搜索。

每一个element：VPN+分配的page frame+指向下一element的指针。

但是页表仍然占用很多空间：虚页号+指针是linear/多级页表没有的。

#### clustered page table

一个entry对应多个page（如16个）。

hash function输入：VPN-向下取整(VPN/16)*16。

16个连续的VPN被放在一个簇里。

一个cluster（entry）：首VPN，连续16个PFN（用后4位索引查找），next pointer。

省了VPN和指针的空间。节省查找时间。

### inverted page table

PFN->VPN。表项：pid+VPN。

不是一个process一个，整个系统只有一个inverted页表。

内存空间节省，但是VPN->PFN的时间增加，要找遍整个表。

用hash table把遍历搜索范围limit到一个或几个entry。（不知道怎么实现

TLB还是可以用的。

很难实现shared memory。



















