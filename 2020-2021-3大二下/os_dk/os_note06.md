# 06  Process Synchronization

注意，我们说的是进程同步，而不是线程同步！！

## 一些背景

concurrency：进程的并发问题，同一个时间段很多进程轮流跑，它们修改共享变量，把共享变量改乱了。

data inconsistency：数据不一致。

race condition：竞争情况，result indeterminate。

常常出现的一种情况：对一个变量：

一个进程把它拿到寄存器里打算++，当它将要++时，另一个进程interrupt它然后也把这个变量拿到寄存器里做++，然后写回去；这时第一个进程夺回CPU，把寄存器内的变量做++写回去。变量被++了两次，但是只变大了1。

问题的关键：uncontrolled scheduling。

## critical section

critical section：同时只能有一个进程在这段代码执行，不然就race condition。

保护它：entry section, critical section, exit section, remainder section。

一个简单的想法：用turn变量代表下一个进入cs的进程，如果现在turn是对方就自旋等待（spin waiting），每个进程出cs后就把turn改成对方。

它是行不通的。不仅只能用于2个进程，而且必须轮流访问。若i执行结束了，那么j访问cs之后把turn改成i，但是i不会再把turn改成j，j再也访问不了cs了。不满足空闲让进。

注意，对空闲让进要考虑【另一个process突然terminate】的情况。

### spin-waiting

JMP到上一条指令（while的条件判断），继续判断，一直判断；

在单处理机的情况，如果陷入自旋等待，那么就自旋等待一整个时间片。

#### 关于临界区大小与自旋锁效率

临界区越大，需要time slice越多（好几个时间片），自旋锁的效率会更低。

拿3个进程举例，假设要m个time slice才能跑完cs一次。m=3时：

1跑，2自旋等待，3等待；1跑，2等，3等；1跑完，2终于可以跑cs，3等；

m越大，有一个process在cs里时别的process自旋等待浪费的时间片就越多。

因此我们需要os的支持，如de-scheduling，如后面的sleep。

#### os支持

yield()：把自己从running变成ready。

queue：park() & unpark(tid)，精准叫醒。

spin-waiting不可避免。guard的spin-wating？仅考虑cs的大小。页48/105，pdf的49页。

#### 关于p/th自旋等待效率

p/th用自旋等待来竞争cs，哪个效率更高？

th。首先context switch代价更小，其次用户层可以定义th scheduling的方式，或许能避免长时间自旋等待，即发现已经自旋1/3个time slice了就赶快把它de-schedule。

### cs solution需要满足的3个条件

- 互斥访问：mutual exclusion。
- 空闲让进：progress。若目前没有process在critical section，并且我提出了request，那么我等的时间应该不是无穷。
- 有限等待：bounded waiting。我提出request进入critical section，在我之前进入的process数量应该有限。

### 对kernel分类讨论

preemptive & non-preemptive：

non-preemptive：处于kernel mode的process会一直run，直到退出kernel mode、阻塞或自愿放弃CPU。

若kernel是non-preemptive的，essentially free of race conditions in kernel mode；在kernel mode时根本没有race condition。

因为，每个process的user space（内存）是自己的，若有共享的data只能是在磁盘（外设），访问外设就要track into os（或者我们叫它kernel mode）。如果不被打断，就没什么问题。（若有共享的data只能是在磁盘：不是很确定）

但是，preemptive更responsive。

## mutex lock互斥锁

hardware synchronization primitives，硬件同步原语。

原语primitives：不会被中断；

mutex lock是一种cs problem的解决方案。

把和lock有关的代码放在cs旁边，这样cs执行起来就像一个single atomic instruction。

用lock：声明一个lock variable，就像声明integer一样。这个lock变量代表锁的状态，available或locked，就像一个boolean。

### lock()

尝试拿到lock，如果没有别的thread/process（ppt上是th，但是不是process synchronization吗，我已经晕了）拿着lock，那么我们就拿lock进入cs。如果lock被别人拿着，那么这个函数不return，直到拿到lock再return大概？

### unlock()

我们free这个lock。若没有别人在等这个lock，那么lock的状态就被simply changed to free。不然，在等的人就会注意到lock状态的变化（它们的lock()函数还没return啊），把lock拿走。

### pthread的lock

```c
// entry & exit
int pthread_mutex_lock(pthread_mutex *mutex);
int pthread_mutex_unlock(pthread_mutex *mutex);
// initialization
pthread_mutex_t lock=PHTREAD_MUTEX_INITIALIZER;
int rc=pthread_mutex_init(&lock,NULL);
assert(rc==0);
// destruction
pthread_mutex_destroy(&lock);
// other versions
int pthread_mutex_trylock(pthread_mutex_t *mutex);
int pthread_mutex_trylock(pthread_mutex_t *mutex,struct timespec *abs_timeout);
```

## mxl的solution：没有特殊硬件支持

关中断：

lock()关中断，unlock()开中断。

不能满足bounded waiting（提出申请后在我前面进入cs的p是有限个），不断有新的p要访问cs，我可能永远访问不了cs。需要一个排队机制。

不安全（恶意进程滥用关中断），multiprocessor时不work（关中断只针对某一个processor），很可能失去中断（执行时错过开中断的代码？），并且低效。

用一个boolean变量做lock：

很有可能两个p把这个flag变量取到reg里，一看发现 呀可以进，然后都进入cs了。也就是说，这个变量成为新的cs了。

### Peterson's solution

Dekker’s and Peterson’s Algorithms，很重要。

只用于两个processes。

注意：==假设load store指令是atomic，that is，cannot be interrupt==。是很强的假设！

```c
int turn;  // 指示下一个进入cs的process
Boolean flag[2];  // 指示我是否ready进入cs
```

```c
// for process i
do{
    flag[i]=true;  // 我想进，我已经准备好了
    turn=j;  // 啊，我先让你进
    while(flag[j]&&turn==j);  // 这时你应该在里面，我自旋等待
    	critical section  // 你出来了，现在我在cs了
    flag[i]=false;  // 我刚刚出来，暂时不再进了
    	remainder section  // 我干别的去了
} while(true);
```

```c
// for process j
do{
    flag[j]=true;
    turn=i;
    while(flag[i]&&turn==i);
    	critical section
    flag[j]=false;
    	remainder section
} while(true);
```

听说考过证明…但是考的更多的是证伪别的方案。

证明：把algo写成instruction，让执行顺序在两边跳来跳去，排列组合。

对互斥访问的证明：while(flag[j] && turn==j)两个判断之间可能也会interrupt。

#### ppt P28的3个错误例子

第一个不满足互斥访问mutual exclusion。第一句两边一判，flag都是false，都跳出循环进去了；

第二个不满足空闲让进progress。同时执行第一句，两个flag都是true了，等着吧；

第三个不满足空闲让进progress。j执行第一句把自己赋成victim，i执行第一句把自己赋成victim，i自旋等待，j发现自己不用自旋等待于是进入cs，然后继续执行remainder section，然后结束了。此时victim还是i，i再也进不去cs了。

### bakery algorithm

可以用于n个process的情况。

unique number+global counter+queue；

queue并不是显式的数据结构，它是由代码实现的。

在进入cs前，每个process获得一个number，这个number比目前在queue中的所有进程的number都要大，比最大的大1；

所有p拿着number和pid的二元组在queue中等待访问cs。（试图让number它unique但是不成，可能会有同时++只+1问题，这时两个number重复了）。

显然越先来排队number越小。进入cs优先级比较规则：number越小越优先，若number恰好一样，则比较pid，pid是两两不同的。

```c
boolean choosing[n];  // 是否正在选号（拿到number），初始化为false
int number[n];  // 全部初始化为0
```

```c
// for process i
do{
    choosing[i]=true;
    number[i]=max(number[0],number[1], ...,number[n-1])+1;
    choosing[i]=false;
    for(j=0;j<n;j++){
        while(choosing[j]);  // 若process j在选号，等它选完
        while((number[j]!=0)&&((number[j],j)<(number[i],i)));  // 现在process j选完号了，若它其实应该比我优先，则自旋等待
    }
    	critical section;
    number[i]=0;  // 0表示没有在排队
    	remainder section;
} while(true);
```

#### 关于choosing

选号的时候，至少有两步。step 1，求max并把结果放在reg里。step 2，number <- max_reg+1。这两步之间可能被打断。

看两个进程的情况：

现在进程ij都在选号，i做了max然后被打断，j做max得到number，开始与所有进程比较优先级。j和自己比没问题；和i比，此时i刚刚做完max但是没有把max+1赋给number，number还是0也就是在队列外，也没问题。j进入cs了。

在cs中的j被interrput，i继续执行，得到值为【原来的max】+1的number。算【原来的max】时，j还是0，因此原来的max是1。事实上，j做max时i的number也是0，情况是对称的，因此两个number应该一样。然后i遍历所有进程比较优先级，和j比发现number一样，若i的pid恰好比j小，则i也进入cs了。

若没有choosing，不满足互斥访问mutual exclusion。

若有choosing，j和i比的时候会发现i还没有选完号，等i选完。此时i的number确实和j一样，但是j和i进行优先级比较是会发现i的pid更小，j会自旋等待i先进cs。

问题在于得到number的操作不是原子操作，可能会被打断。

若一个process取号时被打断，跳到另一个process去取新的号了，确实可能会有重复的number。但是等取完号再比优先级，至少可以根据pid比出结果。

## mxl的solution：硬件支持

若load和store不是原子操作（虽然不太会想象），会更复杂，貌似peterson和bakery都不能用了。

此时需要硬件支持，用FPGA（现场可编程门阵列）烧制成一套不会被打断的硬件，我们就获得了一个原子操作。

### test-and-set

```c
int TestAndSet(int *old_ptr,int new){
    int old=*old_prt;
    *old_ptr=new;
    return old;
}
```

得到旧值，把这个变量改成新值。

为什么第一个参数为指针而不是变量名？因为输入是FPGA的引脚，若参数为变量名则对应的输入为值（如new）。输入指针的意思是告诉FPGA地址（即指针的值）。

```c
typedef struct __lock_t {int flag;} lock_t;
void init(lock_t *lock){
    lock->flag=0;  // 0 available, 1 held
}
void lock(lock_t *lock){
    while(TestAndSet(&lock->flag,1)==0);
    // 为1时，继续赋为1且自旋等待。直到为0，再赋为1，此时拿到锁。
}
void unlock(lock_t *lock){
    lock->flag=0;  // 释放锁
}
```

不满足bounded waiting有限等待，缺少排队机制。

### compare-and-swap

```c
int CompareAndSwap(int *ptr,int expected,int new){
    int actual=*ptr;
    if(actual==expected)
        *ptr=new;
    return actual;
}
```

参数：变量地址，预想值，新值。

若实际值和预想的一样，则把它赋成新值。

```c
void lock(lock_t *lock){
    while(ComepareAndSwap(&lock->flag,0,1)==0);
    // 若为0即cs空闲，则把flag赋为新值1，同时返回0。
}
```

仍然不满足有限等待。

compare-and-swap比test-and-set更强，可以实现lock-free synchronization，不会死锁。

```c
void AtomicIncrement(int *counter,int amount){
    do{
        int old=*counter;
    } while(CompareAndSwap(counter,old,old+amount)==old+amount);
    // 若等于old+amount则已进行增量操作，此时重新取old值
}
```

test-and-set能不能lock-free synchronization呢？？说不定会考喔……

### 满足bounded-waiting的solution

```c
boolean waiting[each i]=false,lock=false;

lock(){  // for process i
    waiting[i]=true;
    while(waiting[i]&&(TestAndSet(lock,1)==1));  // 如果没有被内推且锁被占用
    waiting[i]=false;
}
unlock(){
    j=(i+1)%n;
    while((j!=i)&&!waiting[j])j=(j+1)%n;  // 一个一个遍历
    if(j==i)lock=false;  // 遍历了一圈回到自己，没有人想进入cs，那我就把名额放出来
    else waiting[j]=false;  // 不改变锁的状态，把锁“内推”给id相近的下一个人，告诉它你不用等了
}
```

把锁”内推“给id相近的下一个人了。

## locked data structure

有时候两个process一个改链表头一个改链表尾，它们不冲突的。

一个节点加一个锁，锁又太多了。要权衡一下。

## condition variable

与cs problem不同，它在另一种场景使用，比如：

我们想实现pthread的join函数（进程结束），在有子进程的情况下，我们要等子进程执行完再terminate。

两个问题：父进程怎么等子进程结束，子进程结束怎么告诉父进程。

可以用一个global变量done表示是否结束，done被初始化为0，子进程的函数最后把done赋成1，父进程在done==0时自旋等待。

但是自旋等待效率不高。我们借助os的支持：

### sys call：wait & signal

```c
pthread_cond_wait(pthread_cond_t *c,pthread_mutex_t *m);
pthread_cond_signal(pthread_cond_t *c);
```

wait：让进程sleep，即自己阻塞自己进入waiting state。第一个参数是condition，表示要等这个条件。第二个参数是一个mutex，表示这个mxl别人可以用了。

signal：产生一个信号，唤醒它。参数是一个condition。

wait和signal都是atomic的。

#### signal：hoare vs. mesa

hoare：立刻叫醒，waiting到running。貌似不推荐使用，不应该有waiting直接连到running的箭头。

mesa：改PCB为ready。

若用mesa，要用while循环而非if。叫醒这个进程时if的条件确实被满足了，但这个进程并不立刻执行，等到它执行的时候说不定条件又不满足了。

ppt106页，生产者生产出来两个产品，叫醒两个消费者。一个消费者吃了两个，另一个消费者醒过来也直接开始吃，但是并没有产品，出错了。

### thr_join的实现

A condition variable is an explicit queue，一个cv是一个显式的队列。

和队列有什么关系？？

lock+flag。

```c
int done=0;  // flag，是否做完
pthread_mutex m=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c=PTHREAD_COND_INITIALIZER;

int main(int argc,char *argv[]){
    pthread_t p;
    pthread_create(&p,NULL,child,NULL);
    thr_join();
    return 0;
}
void thr_join(){
    pthread_mutex_lock(&m);
    while(done==0)  // 若还没做完
        pthread_cond_wait(&c,&m);  // free the mutex
    pthread_mutex_unlock(&m);
}
void *child(void *arg){
    printf("child\n");
    thr_exit();
}
void thr_exit(){
    pthread_mutex_lock(&m);
    done=1;
    pthread_cond_signal(&c);  // 需要先wait再signal
    pthread_mutex_unlock(&m);
}
```

需要先wait再signal：

fork之后从CPU来看，父进程子进程是一样的，因此两个进程都有可能调度先执行。

若子进程先执行到`thr_exit()`，则锁住、done=1、信号、释放锁，这个过程中锁一直被占用，`thr_join()`不会执行。

等到join能执行的时候，done=1，会直接执行完而不陷入while loop。

若父进程先执行到`thr_join()`，则陷入while loop，父进程阻塞自己并释放锁。子进程把done改成1，signal父进程，释放锁，结束执行。

下面是错误例子：

```c
void thr_exit(){
    pthread_mutex_lock(&m);
    pthread_cond_signal(&c);
    pthread_mutex_unlock(&m);
}
void thr_join(){
    pthread_mutex_lock(&m);
    pthread_cond_wait(&c,&m);
    pthread_mutex_unlock(&m);
}
```

若先exit再join，则先signal后wait，没有人再叫醒父进程。

```c
void thr_exit(){
    done=1;
    pthread_cond_signal(&c);
}
void thr_join(){
    if(done==0)
        pthread_cond_wait(&c);
}
```

若join把done取到reg里比较，exit把done赋成1，join比较取来的done（是0）和0发现要执行if语句，exit继续执行signal，join再执行if语句wait。先signal后wait。

## semaphore

信号量是一个有整数值的东西，我们可以通过wait()和signal()来操纵它。

### 实现

最简单的实现：

```c
wait(s){while(s<=0);s--;}
signal(s){s++}
// s被初始化为1，只能一个进程进入cs
```

接下来的实现是错误的：（不过对后面有启发？

```c
wait(s){s--;while(s<0);}
signal(s){s++;}
```

标准实现：mutex+integer+condition variable。

```c
typedef struct{
    int value;
    struct process *list;
} semaphore;

wait(semaphore *s){
    s->value--;
    if(s->value<0){
        add this process to s->list;
        block();
    }
}

signal(sepamhore *s){
    s->value++;
    if(s->value<=0){
        remove a process P from s->list;
        wakeup(P);
    }
}
```

wait：先--。若小于0就把自己加到队列里，然后block自己；

signal：++，若小于等于0就在队列移出一个process，然后叫醒这个process。

--后小于0代表没有资源可立刻access。++后小于等于0代表还有process在排队。

这个list是用户定义的，不一定是FIFO的队列，可能有别的优先级定义。

认为semaphore的整数值代表资源的数量（初始化也是这样）。

### 应用

semaphore可以解决所有synchronization的问题，互斥和队列（condition variable）都可以。

并且它可以先signal再wait！用integer记录signal的次数。

并且还efficient，没有spin-waiting。

7个pattern：

#### signaling

thread A有statement a，thread B有statement b；希望先执行a后执行b。

执行的先后顺序，就是队列问题吗？

```c
// thread A
statement a;
signal(s);
```

```c
// thread B
wait(s);
statement b;
```

s的初值应被赋为0。若把s理解为资源数量，则一开始对B来说资源数量为0，A执行完a之后创造了一个资源，数量++，此时B可以拿到资源执行。

#### rendezvous

A有a1a2，B有b1b2。

希望a1在b2之前，b1在a2之前，就像约会一样。

a1b1a2b2, a1b1b2a2, 貌似只要先执行完两个1再执行两个2就可以。

也就是执行2之前确保两个1都执行完。

```c
// A
statement a1;
signal(a);  // 我A的第一句执行完了
wait(b);  // 你B的第一句执行完了吗？
statement a2;
```

```c
// B
statement b1;
signal(b);
wait(a);
statement b2;
```

a和b都应该初始化为0。

##### signal和wait的顺序

若A先s后w，B先w后s，正确但是效率更低，可能多一次context switch？

若AB都先w后s，则死锁：A执行a1然后等B去signal，Bsignal后A再signal，B执行b1然后等A signal。感觉就像，双方都不说自己的进展互相等待，你说我才说？

#### mutex

A和B都执行：`count=count+1;`。

A和B的代码一样：

```c
wait(mutex);
count=count+1;
signal(mutex);
```

mutex初始化为1，资源数量为1。

##### multiplex

同时允许最多MAX个进程进入cs，即资源数量为MAX，mutex初始化为MAX。

#### barrier

n个thread都run这样的代码：

```c
renezvous;
critical point;
```

必须等待所有人跑过rendezvous，才能向后执行。

```c
// initialization
int count=0;
semaphore mutex=1;  // for count
semaphore barrier=0;
```

```c
rendezvous;
wait(mutex);
count=count+1;  // 计数，又跑过去一个
signal(mutex);
if(count==n)  // n个进程都跑过rendezvous
    signal(barrier);
wait(barrier);  
// barrier代表执行顺序，资源数量初始化为0。
// 只有n个进程都跑过，资源数量才为1。
signal(barrier); // 我wait算是占用了一个资源，因此再signal一下生成一个资源
critical point;
```

要count++之后接着把count锁释放，不然会死锁或者效率低。

##### reusable barrier

```c
semaphore barrier1=0,barrier2=0,mutex=1;
```

```c
runedzvous;
wait(mutex);
count+=1;
if(count==n)
    for(int i=0;i<n;++i)signal(barrier1);
signal(mutex);
wait(barrier1);

critical point;

wait(mutex);
count-=1;
if(count==0)
    for(int i=0;i<n;++i)signal(barrier2);
signal(mutex);
wait(barrier2);
```

这样就实现了对critical point的同进同出，反复利用count。

注意对count的操作，包括改变值和判等，都要在mutex里。不然，很有可能判等在一半的时候count被改变。

#### paring

这个是最难的！

要求两类进程（leader和follower）两两配对，如果配对成功就执行，否则等待。

还有一个变种是生成水分子，H2O。两个H一个O配对。

这是一个错误的解决方案：

```c
semaphore leader=0,follower=0;
```

```c
// leader
signal(leader);
wait(follower);
dance();
```

```c
// follower
signal(follower);
wait(leader);
dance();
```

可能会有一个问题，就是它们不是两两配对出去的，而是很多个leader和follower数量满足1：1。会出现H4O2而不是H2O，这个意思。

为什么呢？可能是，对一个leader来说它signal wait，然后一个follower signal wait，它们确实配对成功了。然后CPU去调度，又配对成功了一对，它们4个都可以执行。然后执行，llff，并没有配对执行。

正确做法：

```c
int numi=0,numf=0;
semaphore leader=0,follower=0,paring=0,mutex=1;
```

```c
// leader
wait(mutex);
if(numf>0){  // 若已经有f在等我了
    numf--;  // 找一个f去跳舞
    signal(leader);  //告诉那个f，我们跳吧
}
else{  // 没有f在等我
    numl++;  // 又多了一个单着的l
    signal(mutex);  // 我【试图配对的行动】完成了
    wait(follower);  // 等待f叫我，阻塞自己
}
dance();
wait(paring);  // 等到我们两个跳完再让别人配对
signal(mutex);
```

```c
// follower
wait(mutex);
if(numl>0){
    numl--;
    signal(follower);
}
else{
    numf++;
    signal(mutex);
    wait(leader);
}
dance();
signal(paring);  // 我们两个跳完了
// no signal(mutex);
```

### producer-consumer problem

```c
semaphore empty=MAX,full=0,mutex=1;

void *producer(void *args){
    for(int i=0;i<loops;++i){
        wait(empty);
        wait(mutex);  // 在生产，这是cs
        put();
        signal(mutex);  // mutex用来保护cs
        signal(full);  // full代表可消费的产品数量，生产一个就+1
    }
}
void *consumer(void *args){
    for(int i=0;i<loops;++i){
        wait(full);  // 若产品没了，那就等着吧
        wait(mutex);
        get();
        signal(mutex);
        signal(empty);
    }
}
```

### the dining philosophers

哲学家要么吃要么思考。

```c
while(true){
    think();
    getchopsticks();
    eat();
    putchopsticks();
}
```

导致死锁的解决方案：

```c
int left(int p){return p;}
ing right(int p){return (p+1)%5;}
// chopsticks初始化为1
void putchopsticks(){
    signal(chopsticks[lept(p)]);  // 放下左手的筷子
    signal(chopsticks[right(p)]);  // 放下右手的筷子
}
void getchopsticks(){
    wait(chopsticks[lept(p)]);  // 等左手的筷子
    wait(chopsticks[right(p)]);  // 等右手的筷子
}
```

所有哲学家都执行一遍【拿左手筷子】，死锁了。

解决方案：让第5个哲学家（p=4）先拿右手筷子再拿左手筷子。`if(p==4)`；

听说什么“排序优先级没有环了”。

### readers-writers problem

读者可以同时读，写的同时不能读，读的同时不能写。

#### 1

the first readers-writers problem。

若没有人在写，则读者不能等待。

```c
int read_count=0;
semaphore rw_mutex;  // 读和写的互斥，是否可以写
semaphore count_mutex;  // 改变读者的count

void read(){
    do{
        wait(count_mutex);
        read_count++;
        if(read_count==1)
            wait(rw_mutex);  // 第一个读者去竞争读写权利
        signal(count_mutex);
        
        reading;
        
        wait(count_mutex);
        read_count--;
        if(read_count==0)
            signal(rw_mutex);  // 最后一个读者释放读写权力
        signal(count_mutex);		} while(ture); 
}
void write(){
    do{
        wait(rw_mutex);
        writing;
        signal(rw_mutex);		} while(ture);
}
```

老师的思路：

对于一个情况，可以写则可以读，可以读不一定可以写。

第一个读者竞争“可以写”，最后一个读者释放。我是第几个读者？

#### 1.5

no-starvation。

1里写者可能starve，2里读者可能starve。

```c
semaphore readcount_mutex=1,canqueue=1,rw_mutex=1;
int read_count=0;

void write(){
    do{
        wait(canqueue);  // 我打算写了，读者不能排队了
        wait(rw_mutex);
        writing;
        signal(rw_mutex);
        signal(canqueue);  // 写完了，读者可以一拥而上地读了
    } while(true);
}
void read(){
    do{
        wait(canqueue);  // 现在能排队吗？
        signal(canqueue);  // 若能，则signal回去，不改变状态
        
        wait(readcount_mutex);
        read_count++;
        if(read_count==1)
            wait(rw_mutex);  // 这一步是必要的
        // 若通过canqueue之后CPU调度到一个写者开始写，则还是要竞争一下看看能不能读
        signal(readcount_mutex);
        
        reading;
        
        wait(readcount_mutex);
        read_count--;
        if(read_count==0)
            signal(rw_mutex);
        signal(readcount_mutex);
    } while(true);
}
```

竞争rw_mutex前有一个准备区。

一个writer来了之后，虽然它可能没法立刻拿到rw_mutex，但是它可以拿到准备区啊。reader发现有人准备写，它就不能开始读了。

#### 2

若有一个写者打算写，则不能有新的读者开始读。

对比no-starve：writer读完后，即使还有writer在等，reader也可能一拥而上地读。

准备区外，reader和writer还是平等的，与second不同。

```c
semaphore readcount_mutex=1,writecount_mutex=1;
semaphore readmutex=1,writemutex=1;
int write_count=0,read_count=0;

void write(){
    do{
        wait(writecount_mutex);
        write_count++;  // 现在写者的数量，排队+1个正在写
        if(write_count==1)
            wait(readmutex);  // 不让它读了
        signal(writecount_mutex);
        
        wait(writemutex);  // 写者间的互斥
        writing;
        signal(writemutex);
        
        wait(writecount_mutex);
        write_count--;
        if(write_count==0)
            signal(readmutex);  // 没有人想写了，你可以读了
        signal(writecount_mutex);		} while(true);
}
void read(){
    do{
        wait(readmutex);
        wait(readcount_mutex);
        read_count++;
        if(read_count==1)
            wait(writemutex);  // 我是第一个读者，现在可以读吗？
        signal(readcount_mutex);
        signal(readmutex);
        
        reading;
        
        wait(readcount_mutex);
        read_count--;
        if(read_count==0)
            signal(writemutex);  // 我是最后一个读者，写者们可以写了
        signal(readcount_mutex);	    } while(true);
}
```

真题？听说second是最难的问题了。

### 一点点缺点

没有condition variable的signal_all（broadcast）功能。

priority：若低优先级的p拿到了高优先级p等待的锁，那就暂时把低优先级p的优先级升成高优先级，为了让它更快执行完。会有安全问题。好像是这样的。

## moniter

好像不太重要。

随着OOP的普及，人们开始考虑如何将同步合并到更结构化的编程环境中。

使用管程类：保证一次只有一个线程可以在管程中处于活动状态。

Java管程：在方法/方法集合（set）前添加关键字synchronized。

















## 原始笔记

20210425 34

monitor：管程。

hoare vs. mesa：

hoare：立刻叫醒，waiting到runnning。

mesa：改PCB为ready。



如果不能继续执行，就要while而非if。

while：每次执行都判断。mesa要while。



bounded waiting？

页43/105；

把锁”内推“给id相近的下一个人了。



解决spin-waiting的os support：

yield()：把自己从running变成ready。

queue：park() & unpark()；

spin-waiting不可避免。guard的spin-wating？仅考虑cs的大小。





## dk老师的题目

有一个很有名的游戏叫做魔兽世界，里面有3种角色，分别是abc。玩家上线后排队，组成5人小队才能开始玩，要求队伍里至少有一个abc，其余2人任意。

好难啊！











