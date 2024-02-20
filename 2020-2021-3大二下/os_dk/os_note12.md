# 12 file system implementation

这一章需要掌握的内容有：

- FCB，inode；direct/indirect pointer；extent；
- contiguous/linked/indexed allocation；
- bitmap；free list；

## typical file system

经典文件系统。讨论VSFS，very simple file system。

我们去考察：

- data structures：用什么数据结构来管理data和metadata？
- access method：如何把进程读写映射到访问disk？

### overall organization

磁盘在总体上是怎么组织的？

先把disk分成block，block size=4KB。假设磁盘有64个block。

然后划分一块固定区域作为data region，如后56个block。

inode结构体被放在inode table里，track每个file的信息。3-7共5个block。

inode/data bitmap，用来记录这个inode/data block是否可用。1、2block。

第0个block是superblock，记录文件系统中更super的信息，例如有多少inode/data block，inode table的基址是什么。

结构：super+inode bitmap+data bitmap+inode table+data blocks。

### inode

inode也被叫做FCB，file control block。inode，index node。

每个inode对应一个唯一的inumber，也就是directory中与readable name所对应的low-level name。

根据inum在inode table里直接寻址找inode，base+sizeof(inode)*inum。

在一个文件系统中，inum是unique的。但是跨文件系统就不unique了。

#### pointer：指示文件位置

inode里面有一个block，里面全是disk pointer。

可能会有二级（指向一个全是pointer的data block）、三级pointer。

为什么不只搞一个indirect pointer？因为大多数文件都是小文件，＜2KB。

#### extents：指示文件位置的另一种解决方案

ext4。

就是pointer段+length段，指向很多连续的block。通常多个extents指示一个文件。

### FAT & NTFS

FAT，file-allocation table，是原来用的文件系统。

directory entry是文件name等+start block，一个block的末尾是指向下一个block的指针。如果要随机访问的话，很慢。

directory的data是这个directory的table，表格套表格。

NTFS，new technology file system with MFT，master file table。 

MFT 1KB大小，data部分直接记录内容/extents指针。

### 3种allocation method

重要！！

- contiguous：现代文件系统不使用，但有一些hybrid。ext4，ntfs。
- linked：上文的fat。
- indexed：指针全在FCB中。ext2，ext3。

### directory organization

directory的data block里面，content是什么？

inum+reclen+strlen+name。name是不定长的。

删除文件时把这个entry也删除，需要标记一下（如inum=0）。

strlen是这一entry内容的长度，reclen是距下一entry的长度。

用reclen记录可以提供的长度，这样如果有新file创建，可以知道是否可以直接用这个entry。

### access path

如何根据路径找到文件？

`open("/foo/bar",O_RDONLY);`

- 读【/.】root directory的inode。
- 寻找pointers，去读【/.】的data blocks。
- 找到foo的inum；
- 用foo的inum找foo的inode……

open很麻烦。之后再read，就没有这么麻烦了。

read：

- 读inode定位文件位置；
- 读data block；
- 写inode（最后查询时间）；

write，5次I/O：

- 读inode定位文件位置；
- 读data bitmap找空闲block；
- 写那个block。
- 写对应的data bitmap；
- 写inode更新修改时间。

文件创建：

- 读root的inode，读root的data block，读子目录的inode…直到要创建文件的那个目录，读【这个目录】的data block，找一个可以后续插入文件的entry。
- 读inode bitmap，找free inode；
- 写inode bitmap，标记它被使用；
- 写【这个目录】的data block，link readable name和inode #，这个目录又多了一个文件。
- 读新文件的inode，不知道在干什么；
- 写新文件的inode，init它。
- 写上级directory的inode，update最近访问时间。



## 不考的内容

原始笔记：

fast file system：不考的。

经典file system：data到处都是，离FCB也远，seek起来真是太爆炸了。

把文件系统的structure放在一个cylinder里。



inconsistency：我去改bitmap，然后去写一个block，这两个操作之间断电了。然后出现bitmap=1但是数据并没有的情况。

FSCK：file system checking，蓝屏检查。

journaling：加一个transaction begin和transaction end。

先往journal block里写一遍？



log-structured file system：

metadata正好在data后面，sequential读写。

寻找inode：imap。寻找imap：checkpoint region，CR。

对于更改操作：它根本不删原来的，只是找一个地方写一个新的。

































