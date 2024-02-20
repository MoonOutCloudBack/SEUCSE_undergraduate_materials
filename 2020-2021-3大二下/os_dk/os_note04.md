# 04 thread

## 基本概念

每一个thread都有自己的regs和stack；

有关concurrency和parallelism：data parallelism和task parallelism。

serial portion 必须顺序执行的部分对多核带来的加速比有不匀称的影响。

## user/kernel thread

用户线程位于内核之上，它的管理无需内核支持；

内核线程由os直接支持和管理。

听说它们的区别是代码来源：user thread的代码来源于load进内存/作为application一部分（大概这样？）的库，而kernel thread的代码来源于os。

user thread对于kernel来说是不可见的。

### user thread

不会有kernel intervention，更快。

但是如果只有user thread的话，因为kernel是看不见它们的，所以一个线程block了整个process就block了，根本不会去切换线程（切换线程要kernel来做啊）。

并且，多核系统中，若无kernel thread，也没法把一个process的各个thread分配到各个core上。因为在kernel看起来那个process是一个整体啊。

## 多线程模型，multithread models

### many-to-one

好多user thread被映射map到一个kernel thread。

听说这就是无kernel thread的情况，就是好多线程映射到一个进程（有点玄学）。

这时线程是不能在multicore system上parallel的。

很少被用。

### one-to-one

每一个user thread被映射到一个kernel thread。

感觉这就是单线程了？不知道。

现在的pc就是这样的。因为m:m控制比较复杂，好像。

这时候user thread的数目会被支持的kernel thread最大数量限制。

### many-to-many

用某些映射机制来把多个user thread映射到多个kernel thread，保证一个user thread被block了剩下的还能跑。

这样user thread的数目不会被限制。通过这种映射机制，kernel thread的数量总是sufficient的。

### two-level model

有些many-to-many，有些one-to-one。是在m:m的基础上允许1:1的情况。

## thread libraries

library就是API，貌似？

### POSIX Pthreads

这是一个标准而非具体实现。user-level和kernel-level都可以的。主要在UNIX上用。

### creation

```c
#include<pthread.h>
int pthread_create(pthread_t*		thread,                 
             const pthread_attr_t*	attr,
                   void*			(*start_routine)(void*),
                   void*			arg
                  )
```

第一个参数是一个指针，它会指向新建的线程。

第二个参数是关于这个线程的一些信息，stack size之类的。

第三个参数是thread entry function的function pointer。

第四个参数是这个函数要的参数。

### completion

```c
#include<pthread.h>
int pthread_join(pthread_t*		thread,
                 void**			value_ptr
                )
```

第一个参数指向要结束的进程，第二个参数指向它的返回值。

## implicit threading

线程由compilers和runtime libraries管理，而不是程序员。

还记得课堂上那个例子吗。输出天花乱坠的时候，程序的正确性已经出现问题了。

### thread pool

线程池里面已经有好多线程了，需要的时候就拿一个用。

优点：比新建线程略快，且可以用pool的大小限制程序中线程数量。

把【task】的逻辑与【创建thread用来完成task（准备工作）】的逻辑相分离。

## 一些别的问题

### fork()和exec()的语义

如果一个线程调用了fork去创建子进程，我们是把父进程的所有线程都复制还是只复制当前线程呢？这样就有两个fork的版本。

exec()呢？它load的这个程序会让整个进程脱胎换骨的，而不是只有这一个线程。

### thread cancellation

cancellation，就是在thread finish之前termanate它。

有两种做法：

- asynchronous异步的，立刻把它结束。
- deferred推迟的，会周期性的check它是否应该被cancel。

pthread的cancel：

```c
pthread_cancel(tid);  // tid是线程id，pid是进程id
```

事实上，thread何时被cancel与thread state有关。

| Mode         | State    | Type         |
| ------------ | -------- | ------------ |
| off          | disabled | -            |
| deferred     | enabled  | deferred     |
| asynchronous | enabled  | asynchronous |

若state为disabled，thread不能被cancel。缺省值是deferred。

deferred时，只有线程执行到cancellation point，才会被cancel。

cancellation point：比如说，pthread_testcancel()；

在Linux系统上啊，thread cancellation是通过signals来处理的。

### signal handling

signal被特定的事件生成（比如点鼠标），会被传递到进程。

signal handler是用来处理信号的，每一种信号都有它的default handler。

用户自定义的handler是可以覆盖default的。举个例子，点叉（关闭）之后，可能某些程序会再弹一个对话框”确定要关闭吗？”，这就是override了。

注意，线程是不能拥有自己的handler的，一个进程用一个handler。（考过）

我们假设signal已经到进程了，它应该去哪个线程呢？

- 对所有线程全发一遍。
- 发到signal适用的那个线程。
- 发到特定的线程。有时候进程会专门建应该用来收signal的线程。

### thread-local storage

有个问题：假设这里有好多线程，它们都在做计数，发现一个目标物体就counter++。怎么算总counter？

每个线程维护一个counter，最后把所有counter加起来。

这些counter不是process内共享的，它们也不在thread stack里。它们在thread-local storage (TLS)里。

为什么不在thread stack里？栈是用来存储局部变量的，有生命周期的问题。

这些counter类似于一个thread的全局变量，它们不是局部变量。

### scheduler activations

在m:m或者two-level模型里，如何维护一个合适的内核线程数量呢？

用一个介质的数据结构来在user thread和kernel thread之间传递信息：

lightweight process，轻量级线程，LWP。

它就是用来连接uth和kth的东西。听说它像虚拟处理器，不知道是什么意思。

uth和kth是一一相连的，不能一个uth连好多kth，也不能一个kth连好多uth。

#### LWP到底是用户层的还是kernel层的

我不知道。

但是uth貌似可以空连一个LWP，这个LWP下面并不连着kth。（这个我不确定

LWP是用户层的东西，怎么做kernel其实是管不着的。

（但是书上说是kernel space的东西，……

听说用户层有多少个LWP暗示了它希望多少kth来，但是多少kth分配给这个process要看怎么调度了，这是kernel的事情。（不确定

#### 到底是uth/kth被阻塞？

注意，thread是abstraction，从load进memory的program和一些机器状态中抽象出来的。program是entity。

或许可以认为thread就是它的TCB，一个data structure。

uth和kth的划分不在内存中体现。

uth和kth的本质区别是被谁管理。

kernel space里没有kth的instruction。其实thread是没有instruction的，是kernel space里面没有kth正在执行的instruction。

kernel看到的是kth的TCB的状态。thread table和TCB在kernel space里。kth是由kernel space支持的。

在user space里看到thread table和TCB，uth是由user space支持的。

#### 通过多线程让进程不阻塞

怎么让os知道阻塞的thread的tid，然后告诉user space它的一个thread阻塞了？

对于一个process：可以申请新的kth，也可以申请释放kth。

对kernel：可以强制回收kth，可以同意申请新kth。

如何通信？Scheduler Activations（线程激活）是用户thread libraries和kernel之间的一种通信方案。

它提供了upcall（听说是回调），这样kernel可以告诉user。

#### 如何通过LWP实现高效的多线程？

现在一个kth通过LWP连着一个uth。然后uth执行到了I/O，我们trap into os，整个进程停止执行，保存机器状态到这个process的k-stack里，然后换os来跑。

注意这里不要混淆哦。I/O interrupt的trap into os根本不是通过LWP联系kth。

其实不是uth在跑，是kth通过LWP跑uth。听说uth、kth和process都是空的，它们都是抽象，并没有实体。uth、kth根本没有自己的代码，它们只是一堆TLS和PC，或许。

kth通过LWP跑uth的时候，发现一个I/O，kth得知自己要I/O了，它sys call去I/O。

它现在安排好了，I/O正在进行但是不占用CPU（DMA或许），但是原先的uth在等I/O的结果，kth不能继续跑那个uth了。

于是kth换一个uth，分配给那个uth一个LWP（书上的说法），去跑那个uth了。

然后那个I/O结束了，kernel发来一个interrupt。这个中断是给kth发的，因为是kth跑uth。

注意这里，如果kth跑uth I/O了然后换一个uth又l/O了，kth记得自己I/O两次，但是根本不知道自己连的是哪个uth。（好像

然后我们开始回调。

有一种方案是kth给整个process发一个signal。第二个方案是抢夺一个kth去执行这个回调处理程序，或者再申请一个kth？我不知道。



















