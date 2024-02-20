# db 04 database management systems

框架：

- the architecture of DBMS，DBMS的体系架构。
  - the components of DBMS core，组成部分。
  - the process structure of DBMS，进程线程概念。
- database access management，访问管理。
- query optimization，查询的优化。
- transaction management，事务管理。
  - recovery，数据恢复。
  - concurrent control，并发控制。

## DBMS的architecture

### DBMS core 的 components

![image-20211101150115222](.\..\..\typora-user-images\image-20211101150115222.png)

4个模块：

- parser（语法分析）；
- grant checking（权限检查，是否有权执行这条语句）；
- semantic analysis & query treatment（语义分析 & 查询处理，是核心部分）；
- concurrency control，access management，recovery mechanism（与os交互，把逻辑结构映射成文件；访问管理，并发控制，数据恢复）。

接口：

- ufi：机器访问接口，人直接敲命令。
- API：应用程序访问数据库。

### DBMS 的process structure

- single process structure：
  - 将数据库和应用进程捆绑，编译在一起，形成一个exe。
  - 单机的数据库，在本地的单进程上跑。
- multi process structure：
  - DBMS core 单独作为一个 process。
  - 当一个应用发出 connect 请求，core就创建一个 process 与 application process 连接；
  - 交互：双向通信，两个pipe（单机，双向）/ socket（网络）。
- multi thread structure：
  - 只有一个DBMS process，process里面有一个daemon线程，用来监听。比 multi-process 更省资源，可以服务更多的 application。
  - 听到一个SQL请求，daemon就生成一个新的DBMS core thread，与对应的application process进行交互。
  - 交互方式：pipe（本机）/ socket（不在本机）。
  - daemon：有点像 cn 的 TCP 连接。

## database access management

对 database 的 access 最终会被 transfer 到 file 上的 operation。

file 的 structure 和 access route 会 affect 到 data access 的 speed。

一种 file structure 对所有类型的 data access 都 effective，这是 impossible的。

### access type，访问类型

大致 5 种：

- 查询一个 file 的大多数 record（大多数：≥ 15%，考虑到 record 在磁盘物理块的分布，15%的 record 在概率上会散布在所有物理块中）；
- 查询一个特定 record；
- 查询某些 record（＜15%）；
- scope query：查询一个范围，比如分数在60-90之间。
- update。

### file organization，文件结构

- heap file：堆文件，依次存储，records are stored according to their inserted order，最 basic 和 general 的方式。适合遍历查询（数据扫描）。
- direct file：查询文件，record 的 address 用 hash table 生成，适合查询指定信息。
- indexed file：index + heap file/cluster；前两种的结合，heap file 方式存储 + 对某些 attribute 建立 index。
  - index：B+树索引，双向链表顺序遍历，非叶节点精确查询+范围查询。
  - index 并不是越多越好，维护有代价。
- grid structure file：栅格文件，多维数组存储，把经常 query 的多个属性规格化后，映射成数组下标。
- raw disk：比文件管理更底层，原始的物理存储。向操作系统直接申请一块物理连续的空间（比如说在一个磁道上连续存储，文件管理不会分配给我这么好的空间），可以极大提升访问效率。

## query optimization

【复习】通用计算方法 conceptual evaluation：

- 计算所有 relation_list 的 cross-product。
- discard 不满足 qualification 的 tuples。
- 删掉不在 target_list 里的 attributes。
- 如果 DISTINCT，eliminated duplicate rows。

但是效率太低了。

为什么 relational DB 比 hierarchical / network DB 更需要优化？

- 层次和网状数据库的 data model 使用指针表示属性之间的关系，这样的结构也固定了这两种数据库的查询路径，优化空间有限。
- 而 relational data model 提供了query optimization 的空间。relational DB 抽象程度高，查询语言 SQL 是非过程性语言，效果等价的不同查询实现效率相差很大。
- 因此不仅可行，而且很重要，对系统性能影响很大。

优化：

- 首先 rewrite 用户的 query statement，写成执行效率更高的形式；（比如，把嵌套查询写成非嵌套的形式）
- decide 出来 most efficetive 的做法。

查询树：进行语法分析，把 query statement 写成 relational algebra 的形式；根节点：两个叶节点表的 join / 一个叶节点表的 projection selection。从叶到根的顺序，就是DBMS内部操作顺序。

策略：

- 在不得不做两张表的笛卡儿积之前，把表的规模尽可能缩小。即，减小【二元操作】opd 的规模。
- 尽量把 selection projection 这样的【一元操作】，在查询树中下压。
- 存储重复计算单元（空间换时间）。
- 先使用 algebra optimization 得到最优计算顺序，再使用 operation optimization 进行操作层面的优化。

### algebra optimization

代数优化：变换查询树。

听说隔壁软件要考 绘制SQL的查询树，完成代数优化过程，比如：

![image-20211224165222002](.\..\..\typora-user-images\image-20211224165222002.png)

为什么组织 SQL 查询语句时，一般不建议在 Where 条件中使用“or”谓词？

- 答案：使用 Or 谓词的析取选择条件，并没有好的优化条件，只能按其中各个条件分别选出一个元组集，再求这些元祖集的并。并是开销大的操作，而且在 OR 连接的多个条件中，只要有一个条件没有合适的存取路径，就不得不采用顺序扫描来处理这种查询，导致效率大大降低。
- 人话：如果用 or，不太好优化，我们只能对每个条件算出一个 tuple 集合，然后把它们并一起。把 A B 并一起，需要对每个 A 的 tuple 【遍历】B，看 B 中是否有这个 tuple，避免重复添加。因此开销大。
- 对 or 连接的每个条件，都要对整个表进行 selection，如果其中一个selection 的 attribute 没有进行合适的组织（比如 hash table，B+树），就不得不顺序扫描。而对于 and，我们可以先对组织得当的条件进行 selection，缩小表的 scale。

可以利用的规律（看看即可）：（主要优化 cross-product、union、set-difference）

- join/笛卡儿积：
  - exchange：E1×E2=E2×E1。
  - combination：(E1×E2)×E3=E1×(E2×E3)。
- selection：
  - cluster：$\sigma_{F1}(\sigma_{F2(E)})=\sigma_{F1\and F2}(E)$。​
  - 与 cross-product：
    - 如果 F 的 attribute 全在 E1 里，则$\sigma_F(E1\cross E2)=\sigma_F(E1)\cross E2$​。​
    - 如果F是 $F1\and F2$ 的形式，且 F1 的属性全在 E1 里，F2 的属性全在 E2 里，则$\sigma_F(E1\cross E2)=\sigma_{F1}(E1)\cross \sigma_{F2}(E2)$。​
    - 如果F是 $F1\and F2$ 的形式，且 F1 的属性全在 E1 里，F2 的属性在 E1 E2 里都有，则$\sigma_F(E1\cross E2)=\sigma_{F2}(\sigma_{F1}(E1)\cross E2)$。
  - 与 union 和 set-difference：
    - $\sigma(E1\cup E2)=\sigma(E1)\cup\sigma(E2)$，$\sigma(E1-E2)=\sigma(E1)-\sigma(E2)$。
- projection：
  - cluster：$\pi_{A1...An}(\pi_{B1...Bm}(E))=\pi_{A1...An}(E)$，如果A是B的subset。
  - 与 selection 的 exchange：如果 F 中全是 A 的属性，即 selection 是 projection 的 subset，$\sigma_F(\pi_{A1...An}(E))=\pi_{A1...An}(\sigma_F(E))$。
  - 与 cross-product：如果 A 可以被分为 B 和 C，且 B 的属性全在 E1 里，C 的属性全在 E2 里，则$\pi_A(E1\cross E2)=\pi_B(E1)\cross\pi_C(E2)$。

### operation optimization

用更好的访问策略实现操作。

争取一次扫描磁盘，多做几个操作。

join的优化：

- 最基本的嵌套循环：一个表作为外循环，一个表作为内循环，两个表的元素两两比较。效率非常低，瓶颈在I/O。
- 基于磁盘I/O模型的改进：
  - 目标是减少磁盘扫描次数。
  - 两个缓冲，大小是物理块大小的整数倍，一个给外循环表，一个给内循环表。外循环中，一次取好多物理块，内循环也是如此；每次比较时，两个缓冲的内容两两比较。
  - 用空间换时间。
- merge scan：
  - 需要两个关系 R S，在磁盘上【按照连接属性的值】，事先做好外排序。
  - 双指针法，只需要每个表扫一次。
- B+树索引/哈希索引：
  - 把【连接属性上有索引】的表作为内循环表。
  - 根据外循环缓冲中的值，查内循环表的索引，再也不用扫描内循环表了。
  - 注意，如果属性重复值数量达到全部tuple的20%，用索引不合算。20%的tuple，几乎就要把所有物理块扫描一遍了，还不如顺序扫描呢。
  - 另一种做法：如果 R S 要非常频繁地连接，并且很少在连接属性上更新，那么就做一个 R 和 S 的大哈希表。如果 R S 的某一对 tuple 的连接属性相等，它们就会被 hash 到同一个地方。真的要 join 的时候，直接扫描一遍大哈希表就ok了。

## recovery

- prevention：防，尽可能提高系统可靠性。
- recover：治，数据库的数据恢复。

redundancy is necessary，should inspect（检查而非恢复） all possible failure。

### 关于 transaction

- 事务，是与数据库交互的基本单位。
- 性质：（acid）
  - atomic。
  - consistency preservation：操作前后，数据库都 consistency，各个表保持一致。
  - isolation：隔离性。并发事务互相不干扰，每一个 transaction 好像独占数据库。
  - durability：一个事务只要成功完成，影响是永久的，将来出现故障也可恢复。
- 可以把一组 SQL 语句定义成 transaction。如果没有显式定义，默认一条 SQL 语句是一个 transaction。

常用recovery策略：

- periodically dumping。
  - 每隔一段时间（如一个月），把整个数据库备份一次。
  - 会丢失这一个月的修改信息。
- backup+incremental dumping，备份+增量存储。
  - 时间间隔更短，只存储发生变化的部分。比如，每个月把数据库整个备份一次，每天进行一个增量备份。
  - 会丢失这一天的修改信息。
- backup+log，备份+日志。（企业级）
  - 遇到故障：恢复到最近的 backup，然后按照 log 重演对 database的修改过程。
  - 【不会丢失信息】

### 日志

- 流水账，user 对 database 的每一次改变，老值B.I+新值A.I。
- update：B+A；insert：仅A；delete：仅B。
- 要放在 nonvolatile storage 里（像磁盘）。
- 需要记录的数据结构：
  - commit list：已经 commit 的 TID（transaction id）列表。
  - active list：正在运行的 transaction 的 TID。
  - （有些时候被叫做 CTL ATL。清空 CTL/ATL：在做后备副本后，以前的运行记录就失去价值，可以把 CTL 清空。）
  - 每个log：一个前项链表（BI，所有旧值，内容+地址），一个后项链表（AI，所有新值，内容+地址）。
- 必须遵循的规则：
  - commit rule：在进入 commit 阶段之前，必须把 AI 写到 nonvolatile storage（硬盘的log）上。
  - log ahead rule：如果 AI 在 commit 之前被写入 DB（直接修改数据库），那么 BI 需要先被 log 下来。

### recover strategies

redo 和 undo 操作是【idempotent】（幂等）的，重复做好多遍 和 做一遍 效果一样，与 ctrlZ ctrlY 不同。

#### AI → DB before commit

直接改数据库。仅 undo。

- BI → Log（log ahead），AI → DB。commit：TID → commit list，delete TID from active list。
- 我们把BI存下来，然后把DB改成AI。commit阶段，只要表示我done了（把 TID 放到 commit list，然后从 active list 里删除）就ok了。
- 恢复过程：对每个 checkpoint 后的 TID，检查 active/commit list，根据状态采取措施。
  - 若仅 active list，利用 log 中的 BI 来 undo；
  - 若 active+commit list，delete TID from active list；
  - 若仅 commit，则已经完全成功，无需任何操作。

#### AI → DB after commit

确认事务可以成功再改DB。仅 redo。

- TID → active list，AI →  Log（commit rule），TID → commit list，AI → DB，delete TID from active list。
- 我们把AI记下来，等到 commit 阶段，先把TID放到 commit list 里，然后进行一个AI的改，最后把AI从 active list 里删除。
- 恢复过程：
  - 若仅 active list，事务还什么都没做呢，delete TID from active list；
  - 若 active+commit list，则还没完全写入DB，根据 log 中的 AI 来 redo；
  - 若仅 commit，则已经完全成功，无需任何操作。

#### AI → DB concurrently with commit

利用硬盘的空闲时间，见缝插针改DB。undo+redo。

- TID → active list，AI BI → Log（两个rule），AI → DB（利用空闲周期，partially done），TID → commit list，AI → DB（done），delete TID from active list。
- 先把BI AI都记下来，然后就开始改，commit阶段开始了，我不为所动继续改，全改完了就把自己从 active list 里删除。
- 恢复过程：
  - 若仅 active list，可能 partially 写入 DB，undo；
  - 若 active+commit list，可能 partially 写入 DB，redo 且 delete TID from active list；
  - 若仅 commit，则已经完全成功，无需任何操作。

btw，还有一种既不需要 undo 也不需要 redo 的策略，思想是改指针。

### checkpoint

- 目前的DBMS几乎都采用成批延迟更新数据的策略。（比如我把a改成2，一段时间后又改成3，延迟成批写入就只需要写一个a=3）。
- 但如果迟迟不写入，积压了很多操作，会给系统故障的redo带来更大负担。
  - 系统故障：比如DBMS或OS崩掉，并没有丢失数据库的数据，只是现在有些transaction写了一半，不是consistent的。
- checkpoint：DBMS周期性强制写入所有（已 commit 未写入 DB 的 transaction的）后项。发生系统故障时，只需要对最近 checkpoint 之后 commit 的 transaction 做 redo。

## concurrent control

问题：

- lost update：我本来应该基于【你改动的值】做计算，但因为 concurrency，我取到了老值，然后你把新值发送，我又用我的错误结果覆盖。WAW，写写冲突。
- dirty read：我想取 x:y 的键值对，你想修改数据库。但因为 concurrency，我取了 x，你把整个 tuple 改了，我又取到了改完的 y，x 和 y 根本不 match。读写冲突。
- unrepeatable read：我读了 x=6，然后你把 x 改成了60，我又读了一次，发现居然变成了60，明明我什么都没干。这就是不 isolation 的结果，你的操作会影响到我，没有实现用户“独占”数据库。读写冲突。

concurrency consistency 的 criterion：serialization，可串行化。

- 定义：如果这种并行调度策略的结果，和【某一种串行执行】的结果一样，那么 serializable，认为是正确的。
- 仅为了避免出现这种情况：x初值为3，x\*=2 和 x+=1，仅执行了一条（仅\*=2或+=1）。最后结果是7或者8，都认为可以接受。
- 有一个问题：对 WAR RAW 的 transaction，不同串行执行顺序可能会有不同的结果。这样的 transaction，就不要放一起调度了。

为什么serializable的并发调度是ok的？简答题。

- 事务并发执行的调度是可串行化的，就是说，这个并发调度与该事务的串行调度等价。
- 对于串行调度，各事务的操作没有交叉，没有互相干扰，因此不会产生并发执行时的冲突问题。
- 所以，与之等价的事务并发调度也不会产生冲突，即并发结果是正确的。

并发控制：保证最终结果 serializable。

### 实现：locking protocol

最常使用的三种锁协议：

#### X locks

exclusive lock，排他锁。

两段加锁协议：

- 如果所有的加锁请求都在释放锁请求之前，那么这个 transaction 就叫做【两段事务，two phase transaction】，对事务的限制就是【两段加锁协议，two phase locking protocol，2PL】。
- acquire lock 的阶段叫做 growing phase，unlock 的阶段叫做 shrinking phase。
- well-formed：如果在操作 object 前先 acquire 了 lock，就是 well-formed 的。
- 定理：如果一个调度（schedule）的 transactions 都保证 well-formed 且 two phase，那么调度是 serializable 的。（证明是反证法）
- 结论：
  - 如果 well-formed + 2PL，那么 serializable。
  - 如果 well-formed + 2PL + unlock update at EOT(end of transaction)，那么 serializable && recoverable。不会有recovery的多米诺效应。
  - recovery的多米诺效应：
    - 我改数据，改完后unlock，然后除一个零，被os kill掉了，这时候就要rollback。
    - 你刚刚用了我改好的数据，基于我的结果改了你自己的数据，他又基于你的结果改了他自己的数据……这时候我rollback，你们都要rollback。
    - 为什么 unlock update at EOT 可以避免？如果我有update操作，我就迟迟不unlock，除0的时候transaction还没有结束，所以你那时候还不能拿到我的数据，就不会多米诺了。
  - 如果 well-formed + 2PL + holding all locks to EOT(end of transaction)，strict two phase locking transaction，严格的2PL。

#### (S, X) locks

S锁：read access；X锁：lock access。

S与S相容，S与X不相容。（有点像os的 reader-writer 问题）

#### (S, U, X) locks

U锁：update lock。要update的时候，先加一个U锁，真的要update时再换成X锁。

尽量推迟使用X锁的时刻，提高访问效率，减少deadlock。

两个U锁不相容。S锁和U锁相容，U变成X要先等S读完。

与 AI → DB after commit 策略配合使用，写入DB时再升级为S锁。

![image-20211224144658825](.\..\..\typora-user-images\image-20211224144658825.png)

### deadlock & livelock

deadlock：你和我都要写 relation R1 R2，我先acquire了R1的锁、等R2的锁，你先acquire了R2的锁、等R1的锁，我们就一直互相等待。

livelock：我要改写relation R（X锁），ABC三个人轮着读R（S锁），每次我去check R的状态，总有一个人正在读R，我总是没法update R，但其他事情都很正常。

解决：

- livelock相对简单，可以改进调度策略，FIFO，定优先级等。
- deadlock 的 avoidance，防：
  - 操作系统的方法：transaction事先说明申请资源上限，调度保证不会成环。给资源排序，transaction按照顺序申请资源。
  - 但都不适用。有一个加锁粒度的问题：表、tuple、attribute？因此，资源上限没法确定，排序也不现实。
  - 一旦产生冲突（我想要的资源被占有了）就abort，但是很折腾，也没人这么做。
  - 数据库的方法：
    - 时间戳，相当于生日。可以作为事务标识符TID，也可以比较年龄。
    - wait-die：TA想要TB的资源，如果TA比TB old 就等待；否则abort，并且用原来的时间戳retry。
    - 如果我是你的后辈，那么我就蛰伏一下，等到这一波前辈死掉、社会注入新鲜血液，这时我成为了前辈，就可以去排队被占用的资源了。
    - wound-wait：TA想要TB的资源，如果TA比TB young 就等待；否则kill掉TB，抢掉TB的资源，TB用原来的时间戳retry。
    - 如果我是你的后辈，我就乖巧等待；如果我是你的前辈，我就把你kill掉，你再去修炼30年吧。

- deadlock 的 solving，治：
  - detection：设定等待时间限度。当等待时间timeout后，认为我参与了一个死锁，我就抽身而去，undo掉，一段时间后再来做这个事情。
  - 构建wait-for graph，顶点是事务，边是等待关系，我等你则我指向你。如果有环则deadlock。
    - 检测环的时机：一个transaction开始等待，或periodically。
    - 解决deadlock：在环里面挑一个victim（rollback代价最小），undo掉。













