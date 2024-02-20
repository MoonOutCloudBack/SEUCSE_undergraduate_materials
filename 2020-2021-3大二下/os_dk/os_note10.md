# 10 mass-storage systems

secondary storage device的：物理结构，性能评价指标。

disk scheduling algo。RAID。

## disk structure

track磁道，sector扇区，arm和read-write head。

一个logical block有512byte。disk就是logical block的数组。

逻辑上的logical block->物理上的sectors。写一个logical block的操作是atomic的。

支持更多sector的读写：4KB。

### I/O

$T_{I/O}=T_{seek}+T_{rotation}+T_{transfer}$；

rate: $R_{I/O}=Size_{transfer}/T_{I/O}$；

#### seek time：

把disk arm移到正确的track上。时间很长。

average是full（最里到最外）的1/3。

#### rotation time：

等disk转到desired sector。

average是转一整圈的1/2。

#### I/O性能比较

算速率rate，R_I/O。大小/时间。

random workload：按seek+rotation+transfer公式算，见ppt页14/48。

sequential workload：就是max transfer rate，最大带宽。

RPM：rotation per minute；

### 其他细节

track skew：

arm换道需要时间，sequential的sector安排错开一些。就是arm换道正好reach的时候，正好可以sequential读。









Multi-zoned disk drives：什么东西？？？

outer track的sector数量比inner track更多。



固态磁盘：把cache或buffer按【盘】割裂。



## disk scheduling



disk scheduling：是这一章最重要的！

从request queue上调度，先做哪一个task呢？

（若idle，则立刻做这个task，不存在调度



FCFS：how many tracks move across。

SSTF：

SCAN：elevator。必须去0！





20210604 67

LOOK：不回到0的SCAN。

C-SCAN：更uniform的wait time。

如果是SCAN，到199后又来了一些request，在从199到0的过程中SCAN会serve一些request。但C-SCAN不会。C-SCAN的wait time更加平均。



但以上的方案没有算rotational delay。因此可能不是最好的。

SPTF：shortest positioning time first。

os现在merging，scheduing让drive去做。



disk scheduling的期末笔记：

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



## RAID



RAID：冗余磁盘阵列。

看起来就像disk，和disk一样transparent，但是更大更快更reliable。

RAID由很多disk、易失的不易失的memory和大于等于1个processor来manage。



RAID level 0：根本不是RAID，没有redundancy。

看起来像交叉编址。

chunk size：

性能翻N倍。



RAID level 1：

10比01更好更popular。区别？？？

性能：若half读0half读1，则N\*S而非N*S/2。



RAID level 4：

冗余的校验码。

性能见ppt页42/49。

write：先读要写覆盖的老数据，再写校验码：老校验码xor老数据xor新数据。

为什么随机写R/2？因为每次都要读校验码。



RAID level 5：

rotate的校验码位置。

random性能有改进。



















