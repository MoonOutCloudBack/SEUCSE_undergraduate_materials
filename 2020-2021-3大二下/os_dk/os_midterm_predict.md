# 期中考试押题

## 简答5*6

os的定义，os的功能，多道处理系统multiprogramming的概念，批处理系统batch和分时系统time-sharing的区别？

操作系统分层设计和按模块设计的区别？

进程是什么，和程序有什么区别，进程如何创建，进程如何终止？

进程的状态有哪些，画状态转换图。

duel mode是什么？简述context switch的过程。

线程是什么，和进程有什么区别，引入线程有什么好处？

uth和kth有什么区别？为什么需要uth，为什么需要kth？

scheduling criteria有哪5个？

policy和mechanism相分离的实例？（CPU scheduling）

进程CPU-bound和I/O-bound是什么意思？

进程调度preemptive和nonpreemptive是什么意思，对程序执行效率有什么影响？

什么是临界区问题，什么是同步问题，它们有什么不同？

为什么自旋锁不适合单处理器系统而适合多处理器系统？

## process & thread API



## simple scheduling

Consider the following set of processes, with the length of the CPU-burst time given in milliseconds:

| Process | Burst Time | Priority |
| ------- | ---------- | -------- |
| P1      | 10         | 3        |
| P2      | 1          | 1        |
| P3      | 2          | 3        |
| P4      | 1          | 4        |
| P5      | 5          | 2        |

The processes are assumed to have arrived in the order *P*1, *P*2, *P*3, *P*4, *P*5, all at time 0. 数字越小优先级越高。

FCFS, SJF, RR, preemptive SJF (time slice=2)

## MLFQ

上一题，MLFQ，3个queue，Q0Q1Q2，每个time allotment是2^i，priority burst的S=16。

## peterson

下列算法能否解决critical section problem？

mutual exclusion，progress，bounded waiting。

不用想太多；若不是wait(cond)，则可以用【两个p把一个值取到reg里】的分析方法。

## semaphore



## 简答答案

os的定义，os的功能，多道处理系统multiprogramming的概念，批处理系统batch和分时系统time-sharing的区别？

os是一直运行在计算机上的程序；是硬件上的第一层软件，在硬件与其他程序之间。

os对电脑的硬件资源做合理分配，把硬件资源封装起来提供友好的接口，协调各个程序的执行，处理中断和异常，进行CPU调度、进程管理、存储管理、文件管理，提供友好的用户界面。

支持将多个作业装入内存，当一个作业等待事件发生（如I/O）时，可以切换别的作业执行，提高CPU利用率。

批处理系统支持用户将一批作业提交给操作系统后就不再干预，由操作系统控制它们自动运行。分时系统支持在一台主机上连接多个终端，同时允许多个用户以交互方式使用计算机，共享主机中的资源。批处理系统注重CPU利用率，分时系统注重响应时间和交互性。

---

操作系统分层设计和按模块设计的区别？

按模块设计是将os的功能划分成许多模块。分层设计是按模块设计的一种，最低层为硬件，最高层为用户接口，要求每层只能调用更低层的功能。

----

进程是什么，和程序有什么区别，进程如何创建，进程如何终止？

进程是程序的一次执行，是os分配资源的最小单位，程序+机器状态。进程是主动的，程序是被动的。

创建：申请空白的PCB和唯一的pid，为进程分配资源，初始化数据，将PCB插入系统的new或ready队列。

终止：调用exit()等system call。父进程终止，子进程级联终止。

---

进程的状态有哪些，画状态转换图。

new，waiting，ready，running，terminate。

---

duel mode是什么？简述context switch的过程。

程序执行的两种模式：kernel mode和user mode，只有在kernel mode才能执行特权指令。

当os要跑的时候：硬件将user register压到kernel stack里，然后转入kernel mode，os去跑相应的handler。当os决定切换进程的时候：os将kernel stack里的kernel register显式地保存在相应进程的PCB里，从另一个进程的PCB和kernel stack里把其register加载到内存里，切换回user mode。

---

线程是什么，和进程有什么区别，引入线程有什么好处？

线程是进程中执行运算的最小单位，是CPU调度的最小单位。

每个进程都有独立的代码和数据空间（进程上下文），进程间的切换会有较大的开销，一个进程包含1--n个线程。同一个进程的线程共享代码和数据空间，每个线程有独立的运行栈和程序计数器(PC)，线程切换开销小。

当一个线程阻塞时，其他线程可以继续跑，减少进程主动放弃CPU的情况。创建线程比进程开销小，线程切换开销小。线程间相互通信代价更小更方便。更好利用多处理器系统。

---

uth和kth有什么区别？为什么需要uth，为什么需要kth？

kth的TCB在kernel space，由kernel支持和管理；uth的TCB在user space，由user的线程库支持，对kernel不可见。

uth的执行没有kernel intervention，效率更高。

如果没有kth，一个线程阻塞会导致整个进程阻塞。kth可以实现线程的调度。

---

scheduling criteria有哪5个？

CPU utilization：CPU利用率，CPU使用时间/总执行时间；

turnaround time：周转时间，T_completion-T_arrival。

response time：响应时间，T_firstrun-T_arrival。

waiting time：等待时间，处于ready queue的总时间。

throughput：吞吐率，单位时间内执行完任务的个数。

---

policy和mechanism相分离的实例？（CPU scheduling）

policy：用短作业优先的方式减少turnaround time。

mechanism：SJF。

---

进程CPU-bound和I/O-bound是什么意思？

CPU-bound：spends more time doing computations; few very long CPU bursts。

I/O-bound：spends more time doing I/O than computations, many short CPU bursts。

---

进程调度preemptive和nonpreemptive是什么意思，对程序执行效率有什么影响？

preemptive：正在执行的进程可能被迫放弃CPU。nonpreemptive：正在执行的进程会一直执行到结束，可能因为convoy effict影响周转时间，响应性很差。

---

什么是临界区问题，什么是同步问题，它们有什么不同？

同步：对两个进程的工作次序有要求。互斥：要求临界区被互斥地访问。

---

为什么自旋锁不适合单处理器系统而适合多处理器系统？

自旋锁不适合在单处理器系统是因为从自旋锁中打破一个进程的条件只有在执行一个不同的进程时才能获得。自旋的进程占用了唯一的处理器，其他进程不能执行，因此自旋进程的状态不会被改变，它会自旋一整个时间片。在一个多处理器系统中，其他进程在其他处理器上执行，从而可以让第一个进程摆脱自旋。





