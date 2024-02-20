# 11 file-system interface

这一章需要掌握的内容有：

- inode number，file descripter，hard/symbolic link。

## file & directory

file：bytes的线性数组。low-level name：inode number。

directory：包含很多entries：user-readable name和low-level name的pair，记录一个文件或者其他目录。

directory也有一个low-level name：inode number。

## file interface

### 基本sys call

create file：open() sys call，返回file descriptor。

file descriptor：一个integer，进程私有，read/write sys call用它去读写文件。

process第一个打开的文件的descriptor=3，0是std input，1是std output，2是std error。

read()：参数：descriptor，读到的内容放在哪个buffer，buffer大小；返回读到byte个数。

write()：参数：descriptor（1是std output），写什么（写哪个buffer的内容），写多少byte。返回写byte数。

close()：参数：descriptor。

当我们从命令行里表示自己要读一个文件时：

- open->read->write到std output；->close；

lseek()：改变目前读写文件offset的位置；参数：descriptor，offset，whence（怎么perform这个seek，SEEK_SET文件头+offset，SEE_CUR当前位置+offset，SEEK_END文件尾+offset）

### 立即写 & rename

大多数file system都有一个写的buffer，write()时写buffer，再找时间把buffer写到file里，delayed write。如何立即写？

fsync() sys call：flush，赶快全写到disk里。参数：fd（file descriptor）。

如果一个file刚刚被创建，我们需要fsync()含有这个文件的directory。为什么？？不用管，flush就行了。

如果flush了一半恰好断电呢？用rename协助解决。

rename() sys call：atomic；参数：char *old, char *new。

fsync时rename，先写~xxx，再把【~xxx】rename成xxx。

所以要不执行成功，要不根本没执行，不会执行一半。

### 得到file info

data about data就是metadata，元数据。

stat()或者fstat() sys call，可以看文件状态，状态被存在结构体inode里。

### permmission bits

UNIX的permission bits：9位，决定（owner/group/other）可以（read/write/execute）该文件。

### remove file & link

remove file：unlink() syscall。

后面link会详谈。

## directory interface

mkdir() sys call：创建目录。

ls() sys call：读directories。

rmdir() sys call：删除目录，只有empty（【.】或者【..】entries）时才能删除。

## link

### hard link

link() sys call：参数：old pathname，new pathname；创建new way to access the file。（就像共享目录，只要有向图里无环就可以）

创建一个新entry，对应不同的readable name，但是inode # 是一样的。

unlink()：删除这一entry，只有使用这一inode的文件数量达到0，才free the inode和相关的data blocks。

### symbolic link

hard link的问题：

- 把父目录hard link到子目录，环出现了！

- 并且inode只在disk上唯一，不能跨disk hard link。


symbolic link是一种特殊的type，可以说是另一种file。

symbolic link是快捷方式吗？？

把2 soft link到1上，remove 1，然后2就no such file or directory。

2 hard link到1上，remove 1，2不受影响。

使用ls命令时：

- 开头：“-” 是常规文件，d是directories，l是soft links。

## file system interface

我们要从一些小file system组装出一个大的目录树！

生成文件系统，mkfs() command，给定device和file system type，生产一个empty file system，只有一个root directory。

正如文件被进程使用之前要打开一样，文件系统用于进程之前需要安装（mount）。

需要得知mount point，把file system安装到mount point，然后就可以组装directory tree了。

Mount them to make their contents accessible。









听说，最后的时间不足以支撑最后一题（同步）拿到满分……













