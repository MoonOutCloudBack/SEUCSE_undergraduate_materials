# 05 CPU scheduling

scheduler：决定接下来CPU跑哪个process。

dispatcher：完成context switch的一系列动作：

- save regs(A) to k-stack(A)
- move to kernel mode（然后track into os
- save regs(A) to PCB(A)
- restore regs(B) from PCB(B)
- switch to k-stack(B)

dispatcher做什么：

- switching context，上下文切换
- switching to user mode，去用户态
- jumping to the proper location in the user program to restart that program，找PC

dispatch latency是dispatcher停止一个process然后开始跑另一个所用的时间。

policy和mechanism相分离？

这一章讲scheduler。

## short-term scheduler

short-term scheduler在ready queue的process里面选一个，把CPU分配给它。

queue是priority queue，优先级有很多种约定。

什么时候要去schedule？

- process从running变成waiting（I/O）
- process从running变成ready（明明可以继续执行的，但是被抢占了）
- process从waiting变成ready（I/O完了，准备抢CPU）
- process terminate

其中1和4是非抢占式的，nonpreemptive。

2和3是抢占式的，preemptive。

## scheduling criteria

CPU utilization，CPU利用率。

throughput，吞吐率，单位时间内多少process完成执行。

turnaround time，周转时间，T_completion - T_arrival。arrive貌似是进入ready queue？

waiting time，一个process在ready queue中等待的时间，（而不是处于waiting状态（阻塞）的时间，quite confusing）。

response time，响应时间，请求提交（进程出现？）到第一次响应（第一次执行？），在time-sharing环境中的评价指标。T_firstrun-T_arrival。

## scheduling algos

### FCFS

first come first served，先到先得，FIFO。

burst time：一次使用CPU（I/O）时间。

在包含I/O的情况中，CPU burst被作为考虑的执行时间。

我们用gantt chart来表示CPU调度情况。

先来一个执行时间超长的程序，FCFS下的turnaround time就会很长。

被叫做Convoy effect，护航效应。

### SJF

shortest job first，短作业优先。现在turnaround time好看一点了。

SJF是一种priority-scheduling algo。一般认为priority越小越优先，题目中会说明约定。（注意坑……课上错过一次）

有starvation的问题，不断有新的短process来，low priority（执行时间长）的process可能永远不被执行。

解决方案：aging，随着时间推移增加它们的priority。

只有在多个process可选的时候，才会SJF。如果process一个一个来，那么还是FCFS。所以可以让执行时间很长的process第一个来，turnaround time又很大了。

解决方案是preempt，抢占。

注意，SJF不是抢占的！不是！（考过，错很多

#### HRRN

是SJF的一个修改版本。下一个job有最小response ratio。
$$
response\_ratio=\frac{total\_exec\_time}{estimated\_run\_time}
\\
=\frac{estimated\_run\_time+waiting\_time}{estimated\_run\_time}
\\
=1+\frac{waiting\_time}{estimated\_run\_time}
$$

### preemptive SJF

shortest time-to-completion first，STCF。最短【执行直到结束时间】的job优先。

shortest-remaining-time first，SRTF。最短剩余执行时间的job优先。

貌似是一旦有新作业到来，就重新按priority维护queue，选priority最大的。

其中的priority是【剩余的执行时间】。

复习：turnaround time是从进入queue到执行结束的时间。

其实performance只是一个度量，另一个度量是fairness。

并且考虑到interactive performance，response time也是很重要的。

response time是从arrival到第一次跑的时间。

preemptive SJF不太公平，并且对response time不友好。

### RR

RR，round-robin，轮流的意思。aka time-slicing。

以time slice为单位调度，而不是一run就run到completion。

抢占式的策略：time interrupt。

time slice不是越短越好，需要考虑context switch的时间。

RR在turnaround time上表现很差，所有job这样轮着执行，最后才一起执行完。



以上几个算法都假设我们知道process的burst time。但其实我们不知道。

估计burst time：和前一次的执行时间相近。

用exponential averaging来算：$\tau_{n+1}=\alpha t_n+(1-\alpha)\tau_n$。一般α=1/2。

（自回归诶……

### multilevel queue

MLQ，有很多不同的queues，每一个queue有不同的priority level。

大概就是把这些job先划分为很多priority，然后对于同一个priority的job（在同一个子queue里），有queue自己的调度方式。

multilevel queue is one queue。是一个queue而非多个。

一个process在且仅在一个queue里。permanently。

MLQ的basic rules：

- if A的priority>B的priority，则A去run，B不run。这是抢占式的，只要有priority更高的job，无论现在run的是哪个，CPU都会去runpriority更高的。
- 若AB的priority相同，他们就RR。

举个例子：

（虽然这个看起来好像和第二个rule有点冲突。事实上，这是两部分的内容，以下是以前被使用的解决方法，现在则貌似更偏向于MLQ了。

对foreground的部分：RR，for response time。

对background的部分：FCFS。

这两个部分要平衡一下：

若先前台再后台（fixed priority scheduling），则可能starvation。

time slice：比如说前台0.8，后台0.2。

p32要考，==期末的大题题型==。

multilevel queue中进程不会从一个queue到另一个queue。

## multilevel feedback queue

MLFQ是MLQ的一种。

（MLFQ不是使用最多的，因为它包含了很多小的（可能是simple的）排序类型？？

什么是feedback？根据process的behavior来改变它的priority。

对于一个新来的job，假设它是一个短作业，给它最高的priority。

如果它真的是一个short job，它很快就跑完了。

若它其实是long job，它会慢慢地下移到底下的queue。

规则：

- 把新进入系统的job放在最高的优先级；
- 若它用完了自己的时间片（而不是在执行中间就I/O去了，后者被认为是interactive的前台任务），则它的priority被减小（move down one queue）。
- 若它没用完，则保持在当前优先级。

### limitations

starvation：若interactive job很多，它们会联手用完所有CPU时间，long job会starve。

欺骗scheduler：把时间片用到0.99然后放弃，其实占用很多CPU，但是假装interactive。

changeable behaviors：它从后台变到前台，但是现在的MLFQ没有提高priority的机制。

增加规则：

- 在一段时间S后，把所有job移动到topmost queue里。被称作priority boost。

（听说有点像aging……此时不需要那种前台0.8后台0.2的时间分配了。

但是gaming the scheduler的问题没有解决。

改变规则4：

- 只要一个job用完了它对应priority的time allotment，就降低priority。

time slice：不同的priority不一样，priority高的queue时间片更短。

（这里可能乱序给出队列名（Q0Q1Q2），要按照时间片长短确定优先级关系。

### MLFQ rules

- 若A的priority高于B，则Arun，这是抢占式的。只要有比B更优先的、立即能跑的job（设为A），即使B正在run，也要接下来run A。也就是从高到低一个一个queue地run。
- 若AB的priority相同，则它们RR。也就是同一个queue的job RR。
- 新进入系统的job被放在topmost queue里。
- 若一个job用完了它当前priority的allotment，它就降级。
- 在一段时间S后，所有job被移到topmost queue。

## 一些别的问题

### lottery scheduling

彩票调度。

试图保证每一个job拿到特定份额的CPU时间，而不是优化turnaround和response time。

给进程一些彩票，每次用抽奖的方式决定接下来跑哪一个。

还有一种deterministic的方案，给每一个进程一个步长值stride，希望频繁执行的process的stride更小。每次执行那个pass value（每次stride累加）最小的进程。

但是相比probabilistic的方案有一个问题，就是要维护global的pass value。若半路出现一个进程，它的pass value应该初始化成多大？

### thread scheduling

在支持kth的os中，我们目前讨论的调度都是对kth调度，而不是对process调度。

在many-to-one和many-to-many的模型中，thread libraries调度uth们在LWP上跑。

复习一下：

多对一：多uth被映射到一个kth；

多对多：多uth被映射到多个kth；

这些MLFQ是对kth（而不是process）来调度，每个kth可能又对应很多uth。

uth在kth争取来的短暂的时间中如何调度呢？这是user libraries中定义的。 

system-contention scope (SCS)，process-contention scope (PCS) （调度在process内）。

听说会混着考……

### cache

多CPU时，一个CPU对应一个cache。

cache affinity（process affinity）cache亲和性。

一个process在一个CPU上跑，它的cache上存了一些这个process的状态。那么下次再在这个CPU上跑这个process会更快。

#### asymmetric/symmetric multiprocessing

asymmetric，不对称的，single-queue multiprocessor scheduling（SQMS），只维护一个queue（这个queue可能是multiqueue）。就会有cache affinity的问题。

symmetric，对称的，MQMS。one queue per CPU。但是有可能会负载不平衡，load imbalance，一个CPU很忙但另一个的queue是空的。

migration：把job从一个CPU移到另一个。

### real-time CPU scheduling

soft real-time systems：没有强制ddl。

hard real-time systems：有强制ddl。

Interrupt latency：从interrupt到来到开始处理它的时间。

Dispatch latency：dispatcher切换CPU跑的process的时间。















