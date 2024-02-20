# 03 process

## 一些02 os structure的内容

陷进os里，trap into os。

system call handler ∈ os。

区分policy和mechanism，把它们分开。

policy：我们要干什么。mechanism：我们怎么干。

举例scheduler：policy：短作业优先。mechanism：什么叫做短？有多优先？如何优先？

### 关于os structure

simple MS-DOS，non simple but not complex UNIX；

module-layer-hybrid-microkernal。

## 正式开始第三部分进程

process就是正在执行的program，就是program+machine state。

一个可执行文件ELF的结构：代码段（指令），数据段，栈（有生命周期的变量）和堆（临时申请内存空间）。

program是如何变成process的？loading，初始化以上的段，大概？

### process的状态state

- new：刚刚被创建，还没有load进内存；
- ready：load进内存了，只差CPU了；
- running：在运行；
- waiting：阻塞，比如说I/O。
- terminated；

看这个图，超重要：

![img](./../../typora-user-images/os_process_state.png)

如果是interrupt，比如说时间片没了time interrupt，那么变成ready。

如果是system call，比如说I/O，那么阻塞。等到I/O完了，进程状态变成ready。

### process的结构

设计process的目的：并发。因此要求程序能够停止后再执行，因此记录machine state。

os的核心数据结构：一个进程的列表，和用来track进程状态的PCB，Process Control Block，进程控制块。

上下文切换，context switch。PCB在os memory里面。每一个进程都有一个kernal stack，也在os memory里面。

#### 关于上下文切换：

当这个进程被打断：硬件把context压到kernal stack里面。os一旦开始跑了，machine state就会被破坏。因为os还没有开始跑（os是个程序啊），所以只能用硬件保持context。

最终的目标是把REGs存储到PCB里面。现在先暂存到k-stack里面。

当CPU决定切换switch进程：

call switch() routine:

​	save regs(A) to PCB(A)

​	restore regs(B) from PCB(B)

​	switch to k-stack(B)

就是先把A的regs存到A的PCB里面，然后把B的regs（机器状态）从B的PCB里面拿出来。大概这样？

既然一个进程一个k-stack，为什么还要往PCB里存？

1，PCB是用来干这个的；2，k-stack里面什么都存，很快压进去别的东西regs就取不出来了。

### process的scheduling

这里有几个队列：

- job queue，是所有process的队列
- ready queue，in main memory的队列
- device queue，等着用device的队列，一个device有一个

说到main memory, 内存和主存是一回事吗？

内存由主存和ROM，还有cache和regs组成。

#### short-term scheduler：

有时候只有这一个scheduler。为了interacting，它必须有。

is invoked frequently (milliseconds) -> (must be fast);

#### long-term scheduler:

为了多道程序设计，multiprogramming。

选择哪些进程可以从new变成ready。

process被分为两类：

- I/O-bound process: 整天I/O，很少使用CPU。
- CPU-bound process：few very long CPU bursts，就是说I/O很少，基本上一直在用CPU。

bound是边界的意思，可能是被这种设备的性能制约。

#### Medium-term scheduler：

long-term scheduler只能把process移进内存，不能移除。

如果想降低multiprogramming的程度，就引入medium-term scheduler。

它用swapping把process swap out出main memory。

看这个图：

![process state switching](.\..\..\typora-user-images\image-20210322100422171.png)

### process creation

父进程parent创建子进程child。所有的进程都是由init创建的。

每一个进程都用唯一的pid识别。

当parent创建child时，child完美地复制了parent的内存空间，包括机器状态。完全一样。

child可以继续执行parent的程序。或者child load一个它自己的程序，它脱胎换骨了。

parent wait（阻塞）until children terminate；

#### 拿UNIX的fork()和exec()举个例子

fork：创建一个子进程，注意啊，此时子进程已经被创建了，它和父进程一起在ready和run里面等待调度了！一般认为，父进程要子进程为它完成一些任务，因此父进程等子进程执行完毕。但是，CPU如何知道哪个是父进程哪个是子进程？

- fork这个函数，如果是父进程就返回子进程pid，如果是子进程就返回0。
- 要去编写这个逻辑，如果发现返回值不是0，则自己是父进程了，wait(NULL)。如果是子进程，它就用exec去load和execute一个新程序（也许会，也许直接继续执行了）。
- 不过在分析这种题的输出的时候，父进程子进程调度的时机不一定一样，输出可能乱序。

不能有孤儿，父进程结束子进程必须结束。cascading termination，级联的结束，所有子子孙孙进程都结束。

### interprocess communication, IPC

两种models: shared memory & message passing

shared memory只需要kernel参与一次，就是初始化共享内存区域。

message passing中，每次IPC都要kernel参与，kernel维护一个message queue。

#### producer-consumer problem

就是一个process生产，一个process消耗。

如果shared memory，buffer的大小有限制bounded；

要去维护一个循环队列。

```c
while(((in+1)%BUFFER_SIZE)==out); // 干等的空循环
buffer[in] = next_produced;  // producer
in=(in+1)%BUFFER_SIZE;
```

其实，按照这个逻辑只能放SIZE-1个元素。如果强制放满并不加其他的标志变量，无法区分满和空。

shared memory的机制是进程自己写的，唯一调用system call的时候是初始化和回收shared memory。机制要synchronization。

在message passing中，直接调用send和receive system call，很省事。

#### message passing的synchronization

blocking send：sender一直block，直到receiver收到信息；

blocking receive：receiver一直block，直到sender发了一个消息（貌似这样）；

non-blocking send：sender发了消息，然后去干别的事情，才不管receiver什么时候收到。

non-blocking receive：receiver一直收，收到消息或者NULL，不管sender是否发消息。

如果send和receive都是blocking的，那么就约会了（rendezvous）。

#### 哪个更好？

shared memory用更少sys call，所以比message passing更快。

如果处理器的core多起来，那么message passing会更好，shared memory那里cache一致性会很麻烦。

message passing在小规模数据交换时很好。

#### 一些其他细节

socket，插座的意思，不太明白；

remote procedure call (RPC)，使用stub存根来进行，也不太明白。

管道pipe，是用来把两个process的输入输出接在一起，一个的输出直接接另一个的输入。

管道只能单向通信。如果想要双向通信怎么办呢？接两个管道就好了。（考过）

ordinary pipes，父进程子进程之间通信。named pipes，没有这个限制；

















