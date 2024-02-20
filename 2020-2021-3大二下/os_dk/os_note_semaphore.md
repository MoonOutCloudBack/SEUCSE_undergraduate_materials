# semaphore

这是最后的大题。



分析关系：

哪些是互斥的？哪些是同步的？





## 从复习笔记开始

注意，对空闲让进要考虑【另一个process突然terminate】的情况。

临界区大小与自旋锁效率。

- 互斥访问：mutual exclusion。
- 空闲让进：progress。若目前没有process在critical section，并且我提出了request，那么我等的时间应该不是无穷。
- 有限等待：bounded waiting。我提出request进入critical section，在我之前进入的process数量应该有限。

关中断；peterson。空闲让进要考虑，另一个process突然terminate！！！

bakery。若正在选号，要先等它选完再和它比。否则可能会两个人一起进入cs。

硬件支持：test-and-set，compare-and-swap。

test-and-set能不能lock-free synchronization呢？？说不定会考喔……

满足bounded-waiting的solution：排队机制。

signal：hoare立刻叫醒到running vs. mesa改PCB为ready。

要先wait后signal，不能先signal后wait。

semaphore！初始化成资源数量份数。

- wait：先--。若小于0就把自己加到队列里，然后block自己；
- signal：++，若小于等于0就在队列移出一个process，然后叫醒这个process。

signaling普通同步，rendezvous双重同步，mutex普通互斥，barrier栅障同步，reusable barrier同进同出（好像mutex略有不同），paring两两配对。

paring：最后的signal只需要一个人完成。mutex：配对成功就跳完再让别人跳，试图配对失败就放开mutex。

producer-consumer：命名有点confusing。

readers-writers：第一个读者去竞争读写权利，最后一个读者释放读写权力。

1.5：no-starvation，1里写者可能starve，2里读者可能starve。写者打算写了，读者不能排队了。

1.5和2的区别。

2：readmutex是否可以读（写者操控）好像不是函数开头的简单barrier。

太难了……先做题吧。











## 刷题

### 12 modus hall

After a particularly heavy snowfall this winter, the denizens of Modus Hall created a trench-like path between their cardboard shantytown and the rest of campus. Every day some of the residents walk to and from class, food and civilization via the path; we will ignore the indolent students who chose daily to drive to Tier 3. We will also ignore the direction in which pedestrians are traveling. For some unknown reason, students living in West Hall would occasionally find it necessary to venture to the Mods. Unfortunately, the path is not wide enough to allow two people to walk side-by-side. If two Mods persons meet at some point on the path, one will gladly step aside into the neck high drift to accommodate the other. A similar situation will occur if two ResHall inhabitants cross paths. If a Mods heathen and a ResHall prude meet, however, a violent skirmish will ensue with the victors determined solely by strength of numbers; that is, the faction with the larger population will force the other to wait.



不幸的是，这条路不够宽，不能让两个人并排走。

如果两个Mods的人在路上的某个地方相遇，其中一个会很高兴地走到一边，进入颈部高的漂移，以容纳另一个。如果两个ResHall居民交叉，也会出现类似的情况。

然而，如果一个Mods异教徒和一个ResHall正经的人相遇，一场激烈的小冲突将接踵而至，胜利者将完全由人数决定；也就是说，人口较多的派系会迫使对方等待。

```c
int rescountlr=rescountrl=modcountlr=modcountrl=0;
int reswaitcountlr=...=0
roadmutex=semaphore(1);
enum state={reslr,resrl,modlr,modrl};

reslr(){
    wait(rclrmutex);
    reswaitcountlr++;
    
    compete(){
        if(state=reslrcount||modlrcount)  // 一个方向的
            ;
        else if(state=resrlcount)
        	wait(roadmutex);need
    }
    
    reswaitcountlr--;
    state=resrl;
    signal(rclrmutex);
    
    go();
    
    wait(rclrmutex);
    rescountlr--;
    if(rescountlr==0)
    	signal(roadmutex);
    signal(rclrmutex);
}
resrl(){
    wait(rcrlmutex);
    rescountrl++;
    if(rescountrl==1)
    	wait(roadmutex);
    signal(rcrlmutex);
    
    go();
    
    wait(rcrlmutex);
    rescountrl--;
    if(rescountrl==0)
    	signal(roadmutex);
    signal(rcrlmutex);
}
```

正解：

先barrier一下。





## 原期末考试题怎么做？

### 题干

有一个很有名的游戏叫做魔兽世界，里面有3种角色，分别是abc。玩家上线后排队，组成5人小队才能开始玩，要求队伍里至少有一个abc，其余2人任意。

好难啊！

## 思路

3人的barrier和3到5的匹配。

加入小队写成了一个函数。3人组队也有一个函数。

维护3个队列，3种职业的队列。



以前考过第一写者。读写者是最复杂的同步模式。











