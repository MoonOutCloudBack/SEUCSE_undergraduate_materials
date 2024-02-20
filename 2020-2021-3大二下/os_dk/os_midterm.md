# 期中考试准备

全英文试卷，闭卷考试，答题可以半中半英；

20210427（周二）12：00开始。90min。

## T/F

10个T/F，2分一个。

- 线程是CPU调度的最小单位，进程是资源分配的最小单位。
- long-term sch：从作业队列中选择作业并装入内存；short-term sch：从ready queue中选择进程，并分配CPU。
- 在处理interrupt时关中断。
- ppt3 页27/70，When the (timer) interrupt occurs：==User registers== are ==implicitly== saved by the hardware, into the kernel stack. When the OS decides to switch：==Kernel registers== are ==explicitly== saved by the OS, into the PCB in memory.
- thread不能有its own handler！
- 进程能访问自己的PCB吗？？线程能访问自己的TCB吗？？进程和kth的PCB在kernel space里，不能直接访问；uth的TCB在user space里，貌似也不太能直接访问……
- 75题。指针是地址偏移量的意思啊，就是装入程序的时候，它们的程序空间0地址在内存中地址不一样，因此同一个指针对应的不是同一个变量。
- 对称多处理器中的每个处理器都要完成操作系统的所有问题，所有处理器对等，没有主从关系。
- soft real-time systems：没有强制ddl。hard real-time systems：有强制ddl。
- asymmetric multiprocessing，不对称的，single-queue multiprocessor scheduling（SQMS），只维护一个queue（这个queue可能是multiqueue）。就会有cache affinity的问题。symmetric，对称的，MQMS。one queue per CPU。但是有可能会负载不平衡，load imbalance，一个CPU很忙但另一个的queue是空的。
- 临界区是【更改共享变量】（貌似不仅仅是访问）的一段【程序】（而不是共享变量）。
- simple MS-DOS，non simple but not complex UNIX；module-layer-hybrid-microkernal（可扩展性可移植性安全）；

## 简答

概念是什么，比较概念，如进程是什么，进程和线程区别。

6分一个，一共5个。

- 操作系统的定义：

  - 一直运行在计算机上的程序；
  - 计算机上运行的第一层软件，在硬件和系统程序应用程序之间。

- 操作系统的功能：

  - 分配资源，提供友好用户界面；
  - 管理、协调各个程序执行；
  - process management，memory management，storage management；
  - 中断驱动，静静地等待事件发生。

- 计算机结构：

  - 单处理器系统：只有一个主CPU；
  - 多处理器系统，multiprocessing system：
    - parallel system，multicore system；
    - 吞吐量，规模经济（共用内存外设），可靠性容错性；
    - 多核系统：把多个核集成在一个芯片上，芯片内通信比跨芯片快。不是所有multiprocessing system都是multicore的。
  - 集群系统，cluster system，松耦合；

- 操作系统的结构，

  - 多道程序设计：multiprogramming。CPU在内存中同时保持多个任务，当前任务需要等待时切换任务。
  - 分时系统：timesharing，multitasking，也是多道程序设计。response time，交互性。
  - 批处理系统：也是多道程序设计，CPU利用率。

- 并行parallelism并发concurrency区别：parallelism同时执行多个任务，concurrency多个任务在一段时间内轮流执行。

- kernel & user mode，privileged operation：sys call

  - execute a syscall：trap，trap-handler，return-from-trap；
  - 硬件存context（regs等）到kernel-stack里，os把context存到PCB里；

- 策略policy与机制machanism相分离：CPU调度的实例。

  - time-sharing mechanism (context switch) + scheduling policy。
  - By running one process, then stopping it and running another, and so forth.

- process in memory：代码段（指令），数据段（global），栈（有生命周期的变量）和堆（临时申请内存空间）。

- 创建进程：

  - 首先申请空白的PCB和唯一的pid
  - 为新进程分配资源
  - 然后初始化PCB中的数据项
  - 最后将PCB插入系统的就绪队列。

- 画process状态转换图：

  ![img](.\..\..\typora-user-images\PRCC}`_HAE_{MYUPIQIJI.png)

  ![process state switching](.\..\..\typora-user-images\image-20210322100422171.png)

- PCB，process control block：

  - process：running program+machine state。
  - 用来track（追溯吗）process的信息，包括机器状态；

- 线程的好处：

  - 更好的响应性：一个线程被阻塞后，其他线程可以继续执行。
  - 资源共享：线程间资源共享比进程间的message passing和shared memory代价更小更方便。
  - 更经济：创建线程比进程代价更小，上下文切换代价更小。
  - 更好利用多处理器系统。

- 进程间通信，interprocess communication，IPC的model：

  - message passing：
    - 通过kernel：`send(message);receive(message);`。
    - kernel维护一个message queue。
    - blocking：阻塞的，等待别人。synchronous。
  - shared memory：
    - 进程自己约定共享内存维护方式，

- 客户机/服务器通信：

  - 套接字socket，分布式进程间低级形式通信。
  - 远程过程调用RPC。
  - 管道pipe，是用来把两个process的输入输出接在一起，一个的输出直接接另一个的输入。
    - 普通管道（anonymous pipe）在父子关系的进程之间，它是特殊类型的文件，因此也被子进程继承。
    - 管道只能单向通信。如果想要双向通信怎么办呢？接两个管道就好了。（考过）
    - 命名管道不局限于父子关系 ，可以双向通信，通信进程完成后命名管道继续存在，linux和windows都有。

- uth/kth区别？

  - uth：被用户层的线程库管理；对kernel不可见。
  - kth：被操作系统管理；
  - 多对一、一对一、多对多、two-level。

- lightweight process，轻量级线程，LWP。

  - scheduler activation，线程激活，uthkth间通信方案。

- scheduling criteria有哪5个？

  - CPU利用率，throughput，turnaround time，response time，waiting time（ready queue）；



## 大题

10分一个，一共5个。

### process的API

```c
pid_t pid;
pid=fork();
if(pid==0){
    execlp("/bin/ls","ls",NULL);  // 接着就脱胎换骨！后面不会输出了。execlp是exec的一个版本。
    printf("this shouldn't be printed out\n");
}
else if(pid>0)
    wait(NULL);  // child complete
else printf("wrong\n");
```

fork：若是父进程就返回子进程pid，若是子进程就返回0。

```c
int main(){
    int i;
    for(i=0;i<4;++i)fork();
    return 0;  // 好了，现在有16个进程
}
```

各种神奇调度问题。

#### pthread

```c
void *mythread(void *arg){
    myarg_t *m=(myarg_t *) arg;
    printf("%d %d\n",m->a,m->b);
    m->a=-1;
    return (void *)m;
}
int main(){
    pthread_t p;int rc;
    myarg_t args;
    args.a=10,args.b=20;
    rc=pthread_create(&p,NULL,mythread,&args);
    // 线程，线程的参数（默认NULL），函数指针，函数参数的指针
    rc=pthread_join(p,(void **)&args);
    // 线程，返回值
}
```

ppt4 页35/51。调度，i不减，最后i一定是4。

### simple scheduling

![simple scheduling](.\..\..\typora-user-images\image-20210426145432203.png)

注意这里是数字越大优先级越高，与平时的约定不同。

![key](.\..\..\typora-user-images\image-20210426145742854.png)

HRRN：是SJF的一个修改版本。下一个job有最大response ratio。
$$
response\_ratio=\frac{total\_exec\_time}{estimated\_run\_time}
\\
=\frac{estimated\_run\_time+waiting\_time}{estimated\_run\_time}
\\
=1+\frac{waiting\_time}{estimated\_run\_time}
$$



### MLFQ

要模拟吧。

- 若A的priority高于B，则A run，这是抢占式的。只要有比B更优先的、立即能跑的job（设为A），即使B正在run，也要接下来run A。也就是从高到低一个一个queue地run。
- 若AB的priority相同，则它们RR。也就是同一个queue的job RR。
- 新进入系统的job被放在topmost queue里。
- 若一个job用完了它当前priority的allotment，它就降级。
- 在一段时间S后，所有job被移到topmost queue。

这里可能乱序给出队列名（Q0Q1Q2），要按照时间片长短确定优先级关系。

----

在支持kth的os中，我们目前讨论的调度都是对kth调度，而不是对process调度。

在many-to-one和many-to-many的模型中，thread libraries调度uth们在LWP上跑。

复习一下：多对一：多uth被映射到一个kth；多对多：多uth被映射到多个kth；

这些MLFQ是对kth（而不是process）来调度，每个kth可能又对应很多uth。

uth在kth争取来的短暂的时间中如何调度呢？这是user libraries中定义的。 

system-contention scope (SCS)，process-contention scope (PCS) （调度在process内）。

听说会混着考……

----

![MLFQ](.\..\..\typora-user-images\image-20210426151219695.png)

![key-to-MLFQ](.\..\..\typora-user-images\image-20210426151250144.png)

### peterson

不太可能考证明，可能考相关证伪，可能考类似的构造。



### process synchronization

#### 嗜睡的理发师问题

一个理发店由一个有N张沙发的等候室和一个放有一张理发椅的理发室组成。

没有顾客要理发时，理发师便去睡觉。

当一个顾客走进理发店时，如果所有的沙发都已经占用，他便离开理发店；否则，如果理发师正在为其他顾客理发，则该顾客就找一张空沙发坐下等待；

如果理发师因无顾客正在睡觉，则由新到的顾客唤醒理发师为其理发。

在理发完成后，顾客必须付费，直到理发师收费后才能离开理发店，试用信号量实现这一同步问题。

```c
semaphore mutex_count=1,ispaid=0,canserve=0,seat=1;
semaphore seatnum[N];  // init to 0
boolean cansit[N];  // init to true
int count=0;

void server(){
    do{
        wait(mutex_count);
        while(count==0)  // 没人，睡觉
            wait(canserve,mutex_count);
        signal(mutex_count);
        
        int n=-1;
        wait(seat);  // 查看座位
        for(int i=0;i<N;++i) if(cansit[i]=false){
            n=i;break;
        }
    	signal(seat);
        if(n==-1)continue;  // 怎么搞的，不是有人吗，回头再看一遍
        haircut(n);  // 理发
        signal(n);  // 起来付钱走人
        
        wait(ispaid);  // 付钱了吗？
    } while(true)
}
void customer(){
	wait(mutex_count);
    count++;
    if(count==1)signal(canserve);
    if(count>N) {signal(mutex_count);return;}
    signal(mutex_count);
    
    int n=-1;
    wait(seat);  // 找一个椅子坐
    for(int i=0;i<N;++i) if(cansit[i]=true){
        cansit[i]=false;n=i;
        wait(seatnum[n],seat);  // 在这个椅子上等
        break;
    }
    signal(seat);
    if(n==-1)return;  // 怎么搞的，还以为有位置，走了
    
    wait(seat);
    wait(mutex_count);
    cansit[n]=true;count--;  // 我走了
	signal(mutex_count);
    signal(seat);
    
    pay();
    signal(ispaid);  // 我付钱了
}
```

#### 魔改的生产者消费者问题

设有两个生产者进程A、B和一个销售者进程C，他们共享一个无限大的仓库。

生产者每次循环生产一个产品，然后入库供销售；

销售者每次循环从仓库中取出一个产品进行销售。产品不允许同时入库，也不允许边入库边出库；

而且要求生产和消费A产品和B产品的件数都满足一下关系：-n≤【A的件数－B的件数】≤m，其中n、m是正整数。

```c
semaphore mutex_stack=1,mutex_count=1,isempty=0,wait_a=0,wait_b=0;
stack s;
int count_a=0,count_b=0;boolean empty=true;

void A(){
    do{
        wait(mutex_count);
        if(count_a-count_b>=m)wait(wait_a);
        signal(mutex_count);
        
        wait(mutex_stack);
        s.push(produceA());
        wait(mutex_count);
        count_a++;
        signal(mutex_count)
        signal(mutex_stack);
    } while(true);
}
void B(){
    do{
        wait(mutex_count);
        if(count_b-count_a>=n)wait(wait_b);
        signal(mutex_count);
        
        wait(mutex_stack);
        s.push(produceB());
        wait(mutex_count);
        count_b++;
        signal(mutex_count);
        if(empty){empty=false;signal(isempty);}
        signal(mutex_stack);
    } while(true);
}
void C(){
    do{
        wait(mutex_count);
        while(count_a<=0&&count_b<=0){
            empty=true;wait(isempty,mutex_count);
        }
        signal(mutex_count);
        
        wait(mutex_stack);
        product p=s.pop();
        sail(p);
        wait(mutex_count);
        if(isA(p))count_a--;
        else count_b--;
        if(count_a-count_b=m-1)signal(wait_a);
        if(count_b-count_a=n-1)signal(wait_b);
        signal(mutex_count);
        signal(mutex_stack);
        
    } while(true);
}
```

#### 操作系统考研真题.pdf

有一些题。





## 期末题型

20T/F 1

6 简答 5

5 大题 10



