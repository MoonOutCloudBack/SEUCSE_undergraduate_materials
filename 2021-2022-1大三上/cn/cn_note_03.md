# cn 03 transport layer

- understand principles behind transport layer services：
  - multiplexing & demultiplexing，多路复用 & 多路分解；
  - reliable data transfer，可靠数据传输；
  - flow control，流量控制；
  - congestion control，拥塞控制；
- learn about Internet transport layer protocols：
  - UDP：connectionless transport，无连接的传输；
  - TCP：connection-oriented reliable transport，面向连接的可靠传输；
  - TCP congestion control，TCP的拥塞控制。

## 概述

核心网：网络层、数据链路层、物理层；传输层、应用层：仅在网络边缘存在。

传输层的服务和协议，为不同 host 的 process 直接提供 logical communication，逻辑上的通信。

传输层协议运行在端系统：

- send side：将应用层的 message 转换成传输层的segment：分割成小块，并加上 transport layer header。（fyh说，把 message 切成 segment，然后包装成 packet）
- receive side：把segments重组成message，交给应用层。

就像邮局的柜台，提供服务的接口。负责传输的人不是我，我只是控制一下传输逻辑，并且调用下层功能，下层真正去传输。

传输层协议运行在端系统（应用层在端系统上，根据上一章笔记）。提供接口：socket。

### 传输层与网络层

- 网络层：host之间；
- 传输层：process之间。

举个例子，cresc 的电脑 epsilon 和手机 psi 上分别有12个进程，这24个进程喜欢每天相互通信，因此每天要往来 12(某方的12个进程)\*12(每个进程都跟对方的12个进程通信)\*2(双方通信)=288 条消息。每天，epsilon上的E / psi上的P 负责收集12个进程的消息，然后【通过网络层协议】一股脑发送给另一方 end system，也负责收到另一方的一大包消息后，将消息解包分发给12个进程。

- 应用层报文：一个进程想跟另外一个进程说的话。
- 传输层协议：E 和 P，负责从 host 粒度到 process 粒度。所以说，为不同 host 的 process 提供 logical communication。
- 网络层协议：主机之间通信。

==选择题==：自底到上从物理层到应用层，第一个【面向端到端通信】（端口的意思）的层是【传输层】而非网络层，第一个实现【计算机到计算机之间】（点对点/节点到节点）逻辑通信的是【网络层】。如果自上而下，第一个提供端到端通信的可能是应用层。

internet网络层的协议，IP（网际协议）：尽力而为交付服务（best-effort delivery service），“尽力”但不做任何确保（如确保交付、完整性、按序），不可靠服务。

传输层协议：TCP：可靠服务+拥塞控制；UDP：不可靠服务，没有流量控制/拥塞控制。

## multiplexing & demultiplexing

多路复用 & 分解服务，是所有计算机网络都需要的。

- multiplexing at sender：在 host 的不同 socket 中收集 message，为每个 message 加上 header，从而得到 segment， 把 segment 交给网络层。
- demultiplexing at receiver：将传输层 segment 中的数据，交付到正确的 socket。
- goodput：有效载荷；payload：载荷；其他部分（并非有效数据）：各层的header。
- 基本原理：
  - 传输层的segment：源端口号（16bit）+目的端口号（16bit）+其他header字段+应用数据message。（标明源端口号，是为了方便你回复我。）
  - 端口号：16bit，0~65535。0~1023 称为 周知端口号（well-known port number），保留给周知应用层协议（如HTTP80，FTP21）。
- UDP：无连接（connectionless）的多路复用 & 多路分解。
  - 传输层自动为socket分配端口号 或 使用（python的）`bind()`方法绑定一个端口号。
  - server 使用一个 socket 服务所有 client。
  - 定位 UDP socket：IP地址+端口号。
  - 当segment到来：将它 direct 到对应的 socket。
- TCP：面向连接（connection-oriented）的多路复用 & 多路分解。
  - 定位 TCP socket：源IP地址+源端口号+目的IP地址+目的端口号，4元组。
  - 与UDP不同：两个具有不同源IP地址（源端口号）的到达TCP segment（只要source有一点点不一样），将被定向到两个不同的socket，【即使destiny是完全一样的】，除非TCP segment携带了初始创建连接的请求。
  - 也就是，server host 支持多个并行的 TCP socket，每个socket与一个process建立连接。web服务器为每个TCP连接使用一个新进程/线程。
  - 总结：server 使用一个监听 socket 和多个连接socket 服务多个客户 。

## UDP

仅仅提供多路复用 & 分解服务 + 少量的差错检测。

header：8 Byte。（没有TTL）

- 16b 源端口号+16b 目的端口号+16b UDP长度+16b UDP检验和。
- UDP长度：单位是 byte，包括 header 的长度。
- UDP检验和：提供差错检测功能，用于确定 segment 的 bit 是否改变。
  - 发送数据：
    - 把校验和字段设置为0；
    - 把【需要校验的所有数据】切成16位的很多段，当成反码，都加起来；反复将溢出的部分（`sum>>16`）加到最低位上，直到最高位为0；
    - 把得到的结果取反（0变成1、1变成0），存入校验和字段中。
  - 接收数据：
    - 把【需要校验的所有数据】看成以16位为单位的数字组成，依次进行二进制反码求和，要加上校验和字段；
    - 把计算结果求反，看结果是否为0，（即原结果是否全1）；
    - 如果等于0，说明传输正确，校验和正确。否则，校验和就是错误的，协议栈要抛弃这个数据包。
  - 为什么用反码：
    - 用反码求和时，交换16位数的字节顺序，得到的结果相同，只是字节顺序相应地也交换了（可能是，ab+cd=ef，则ba+dc=fe）；而如果使用原码或者补码求和，得到的结果可能就不同。
    - TCP校验和覆盖TCP首部和TCP数据，而 IP首部中的校验和只覆盖IP的首部，不覆盖IP数据报中的任何数据。
    - TCP的校验和是必需的，而UDP的校验和是可选的。

DNS：使用UDP的应用层协议。

优势：

- 【发送数据时机】的【应用层控制】更加精细：UDP立刻将数据答辩并交给网络层，而TCP的拥塞控制可能会延迟segment发送。
- 无需建立连接，快速高效。
- 不维护连接状态，可以支持更多的活跃客户。
- 分组首部开销小。

![image-20211104160613316](.\..\..\typora-user-images\image-20211104160613316.png)

然而，TCP被越来越多地应用到流媒体传输中（课本）。

UDP应用可以实现可靠数据传输，在application中建立可靠性机制。

## 可靠数据传输原理

只考虑单向（unidirectional）数据传输（但控制信息双向流动）。

使用有限状态机（FSM，finite state machines）描述 sender 和 receiver。（状态图理解即可。）

==选择（xxx条件要用xxx协议），简答（端到端可靠协议需要什么措施）==。

### rdt1.0: reliable transfer over a reliable channel

sender / receiver：1种状态 。见ppt，非常简单，无需任何反馈，因为channel是reliable的。

### rdt2.0: channel with bit errors

可能发生 bit 翻转（01互换）错误。

（好像，假设我们可以通过差错检测，知道一个packet受损，只不过不知道原来的信息是什么。不会发生 true 受损成 false，receiver 浑然不知就当作 false 的情况。

- 差错检测：需要额外发送校验码。
- 接收方反馈：肯定确认ACK（好哒~），否定确认NAK（我收到了乱码，能再说一遍吗？）。
- sender：2种状态，wait for call from above，wait for ACK or NAK。
- receiver：1种状态，wait for call from below。
- 致命弱点：没有考虑 ACK/NAK packet 受损的可能性！
  - 不要询问“你的回答是什么”，这样会给 sender 引入一个新状态，并且这个询问也可能受损，所以要再引入一个新状态……
  - 不可以直接重传，因为可能导致发送重复的分组，receiver 不知道这是重复的 packet，还是新的下一个 packet。
  - 可以在 ACK/NAK 增加足够的校验位，使得sender 可以检测+恢复数据。只要 channel 不丢包，就是可行的。
  - 最简单的方法：给分组编号，添加 sequence number 字段。
    - 对于目前的情况（不丢包），编号 1bit 就足够了。
  - rdt2.1：状态数翻倍（编号0和编号1）。
    - sender：wait for call 0 from above → wait for ACK/NAK 0 → wait for call 1 from above → wait for ACK/NAK 1。
    - receiver：wait for 0 from below → wait for 1 from below。（receiver发送的应答也包含编号）
    - 因为是停等协议，0 和 1 就够了。
  - rdt2.2: a NAK-free protocol，不使用NAK。
    - NAK 等价于 对上一个packet发【带编号的ACK】，sender收到冗余（duplicate）ACK之后，也会觉得不太对劲。

### rdt3.0: channels with errors and loss

（又叫做 bit交替协议，alternating-bit protocol。

实现：

- 如果 sender 经过一段时间后，还没有接收到 receiver 的 ACK，那么就重传 packet。
- receiver 不担心收到 duplicate packet ，可以通过编号确认。
- 需要倒计时定时器（countdown timer）。
- sender 的 FSM 和 rdt2.2 差不多，只是会 spin-waiting 等待一段时间。
- 超时 || 收到NAK：resend。

评价：correct but inefficient，网络协议 限制了 物理资源的使用。

- 停等协议，两个人大多数时间都在互相等待。
- 解决方法：流水线 pipelining，sender 发送多个 packet 而无需等待确认。
  - 增加序号范围；
  - sender 和 receiver 缓存多个 packet；
- 对序号范围和缓存的要求，由【协议如何处理丢失/损坏】决定。
- 流水线差错恢复基本方法：回退N步（go-back-N，GBN），选择重传（selective repeat，SR）。

#### 回退N步，go-back-N【重点】

（又叫做 滑动窗口协议，sliding-window protocol。

限制：pipeline中，未确认的packet数量，不能超过N（window size）。

packet序号范围：

- base：最早的未确认分组的序号。
- nextseqnum：最小的未使用序号（下一个待发packet的序号）。

![image-20211104201148750](.\..\..\typora-user-images\image-20211104201148750.png)

策略：

- sender：
  - 上层调用 rdt_send()：首先检查窗口是否已满（即有N个已发送未确认 packet）。如果未满，则 send 一个新 packet。
  - 收到一个ACK：ACK 是累积确认（cumulative acknowledgement），表示 receiver 已收到序号为n之前所有（包括n在内）的 packet。重启 timer。
  - 超时：重传【所有已发送未确认】的 packet。仅使用一个 timer，对最早的已发送未确认 packet 定时。若timeout(n)，重传 packet n，和窗口中索引顺序号高于n、已发送但未确认的 packets（上图中黄色 packet）。
- receiver：
  - 【重要】receiver only sends cumulative ACK，只发送累计确认。
  - 收到 packet n，且上一个 packet 是 n-1（按序），则发送 ACK n。否则，【丢弃 packet n】（而非暂存），并为上一个按序接收的 packet 发送 ACK。
  - receiver 的 window size = 1。
  - 比如 receiver 一直发 ACK2，则可以确定 packet 3 丢掉了。

缺点：当 window size 和 RTT 都很大时，流水线里面很多 packet ，丢失一个 packet，就会导致大量 packet 重传。

#### 选择重传，selective repeat【重点】

基本思想：

- sender 和 receiver 分别维护一个 window，窗口大小可以不一样，receiver 窗口可以大一些。
- receiver 单独确认（individually acknowledges）所有正确接收的 packet；缓存收到的packet，数据到齐后，按序交付给上层。
- sender 仅重传没有 ACK 的 packet，为【每个】未确认 packet 计时。

![image-20211104204525258](.\..\..\typora-user-images\image-20211104204525258.png)

策略：

- sender：
  - 上层调用 rdt_send()：find out 一个可用的序号，如果在窗口里就 send，否则不 send（将 packet 缓存，或者把数据还给上层）。
  - 收到一个ACK：将对应 packet 标记为已确认。如果 标号==base，则 base 向前移动，移动到最小未确认 packet 那里。
  - 超时：重新发送 packet n，restart timer。
- receiver：
  - 接收 [rev_base, rev_base+N-1] 的 packet：产生 ACK，缓存 packet。
  - 接收 [rev_base-N, rev_base-1] 的 packet：必须产生 ACK，虽然已经交付过了（为了 sender 的 window 前进）。
  - 交付：比如，依序收到 0 1 3 4 5 2，则 收0交付0，收1交付1，2丢失了，收3 4 5，sender重发2，收2 交付2 3 4 5。总之就是，连续packet，按序交付。

一个问题：

- 比如说，sender发送了packet 0123，receiver都接收并发送ACK，然后sender又发来了0123。第二波0123，是因为ACK全超时而重发，还是使用旧编号的全新packet？
- 因此，【window size】需要 ≤【序号空间大小的一半】。

GBN和SR的比较【考点】：

- receiver 窗口大小：GBN 1，SR 至少 sender 窗口大小。
- 区别比较：两方的【发送、接收、应答（累计确认/单个确认）、重传（全部重传/单个重传）】方式的区别。

考试题：

![img](.\..\..\typora-user-images\QQ图片20211111090159.png)

解法：充分利用 RTT，在一个 RTT 内马不停蹄地发 packet。

考试的时候，为了简便，把 k 当作 1000 来算。

ans=2\*(125ms+250ms)/(1000B/8kB/s)=750ms/125ms=6。

## TCP



overview：point-to-point（点对点，一个sender一个receiver），reliable（有序的字节流），pipelined，全双工（双向数据流动），面向连接，流量控制，拥塞控制。

### segment structure

一些概念：

- MTU，maxinum transmission unit，最大传输单元：在TCP/IP协议族中，IP数据包能经过一个物理网络的最大报文长度，其中包括IP首部。
- MSS，maximum segment size，最大报文段大小：TCP报文的最大数据报长度，不包括TCP首部。在TCP链接过程中由双方协商得出，SYN字段的选项包含了这个信息。
- 如果MSS+TCP首部+IP首部＞MTU，IP报文分片，≤ 则正常发送。一般来说，MSS=MTU - IP首部大小 - TCP首部大小。

首部至少有20个byte。（没有TTL，只有IP有TTL）

![image-20211111091939418](.\..\..\typora-user-images\image-20211111091939418.png)

- 最大段长度（MSS）：
  - TCP段中可以携带的最大数据字节数。
  - 建立连接时，每个主机可声明自己能够接受的MSS，缺省为536字节。
- 窗口比例因子（window scale）：
  - 建立连接时，双方可以协商一个窗口比例因子。
  - window size：（好像就是receive window）对方收到ACK前可以发送的字节数，即发送窗口大小。
  - 实际 window size = window size * 2 ^ window scale
- 选择确认（SACK）：
  - 最初的TCP协议只使用累积确认；
  - 改进的TCP协议引入选择确认，允许接收端指出缺失的数据字节。
- 发送序号（sequence number）：数据载荷中，【第一个字节】在【字节流】中的序号。
  - 初始序号的选取：每个TCP实体维护一个32位计数器，该计数器每4微秒增1，序号不是从1开始；建立连接时，从中读取计数器当前值（依赖具体实现，见连接管理）。
- 确认序号（acknowledge number）：【期望接收的下一个字节】的序号。
  - 下一次对方发来 segment 的发送序号，就是上一次我发过去 segment 的确认序号。【可能考填写】
  - TCP标准没有规定，如何处理乱序到达的segment。或许可以用一个缓存。
  - 题外话：如何设置TCP超时？
    - 应该大于RTT，但RTT会变化。
    - 太短：过早超时，不必要的重传，浪费带宽；太长：对segment丢失反应不灵敏，延时增大。
    - 如何估计：用exponential smoothing。

### reliable data transfer

特征：pipelined segment 流水线数据段，cumulative ack 累计确认， single retransmission timer 单个重传定时器。

触发 retransmissions： timeout events（超时事件），duplicate acks（重复确认）。

sender：

- 收到应用数据：
  - 创建并发送TCP segment。seq # ，是segment第一个字节的字节流编号。
  - 若当前没有定时器在运行（没有已发送、未确认的报文段），启动定时器。【仅对最早未确认的报文段，使用一个重传定时器】
- 超时：
  - 重传包含最小序号的、未确认的报文段，重启定时器。
  - 【仅在超时后，重发最早未确认的报文段】
- 收到ACK：
  - 如果确认序号（累计确认）大于基序号（已发送未确认的最小序号），推进发送窗口（更新基序号）。
  - （累计确认可以解决 premature timeout，见ppt页91）
  - 如果发送窗口中还有未确认的报文段，启动定时器，否则终止定时器。
  - （定时器用来确认丢包，触发重传）

receiver：

- 具有所期望序号的按序报文段到达， 所有期望序号之前的数据都已确认：【延迟应答，delayed ACK】等下一个segment 500ms，如果没来就send ACK。可能是不想频繁发送ACK占用带宽。
- 具有所期望序号的按序报文段到达，另一个按序报文段等待ACK传输（上一种情况又收到一个segment）：马上发送单个累计ACK，确认两个按序segment。
- 比期望序号大的失序报文段到达，检测出时间间隔：立即发送冗余ACK（相当于NAK），指明下一个期望报文段的序号。
- 能部分或完全填充接收数据间隔的报文到达：倘若该报文段起始于间隔的低端（窗口可以推进了），则立即发送新的累计ACK（我还想要后面的segment）。

TCP fast retransmit 快速重传：

- 动机： long delay before timeout，通过重复 ACKs 检测丢失的数据报。
- 如果sender收到了重复的3个 ACK 报文段, 意味着该报文段指示的下一个期望 segment 丢失/损坏；在超时触发前快速重传该 segment。            
- （也不是GBN也不是SR，有cumulative ack也有单个重传。）

### flow control

流量控制：

- 维护receiver缓存的容量，接收方【收到segment的速率】和【向上层交付的速率】要匹配。
- receiver 控制 sender 的发送速率，告诉它 你不要发这么快 / 你可以发的再快些。
- 接收端告诉发送方Rwnd大小（segment首部），表明还有多少可用的缓存空间。单位时间内（一个RTT）发送量不能超过Rwnd。

### connection management

在传输数据前，发送/接收方“握手”。

- 两次握手：
  - “来聊天吧” “好啊”。收到对方的消息，立刻建立连接。
  - 失败场景：half-open connection。
    - 我给你发包，你迟迟不吱声，我timeout了给你再发一个，这时候你的应答姗姗来迟，然后我把连接关掉了。在你那边看，你收到我的包后就建立了连接，一段时间后把连接关掉了，然后收到了我重发的包，又建立了连接。也就是说，我把连接关掉了，但你还在建立。
- 三次握手（要求画时序图，知道置位 / ack # 填充）：
  - 第一次握手：建立连接。客户端发送连接请求报文段，将【SYN】置1，Sequence Number为x；然后，客户端进入SYN_SEND状态，等待服务器的确认；
  - 第二次握手：服务器收到SYN报文段。服务器收到客户端的SYN报文段，需要对这个SYN报文段进行确认，设置【Acknowledgment Number为 x+1，即Sequence Number+1】；同时 ，自己还要发送SYN请求信息，将【SYN】置1，Sequence Number为y；服务器端将上述所有信息放到一个报文段（即 SYN+ACK报文段）中，一并发送给客户端，此时服务器进入 SYN_RECV 状态；
  - 第三次握手：客户端收到服务器的 SYN+ACK 报文段。设置【Acknowledgment Number 设置为 y+1】，向服务器发送ACK报文段。这个报文段发送完毕以后，客户端和服务器端都进入 ESTABLISHED 状态，完成TCP三次握手。
  - 总结：SYN → SYN+ACK → ACK。
  - 第一/二个包如果丢失，会超期重传，直到收到确认。
  - 第三个包如果丢失，3种情况：
    - 双方都没有数据发送，等待超期重传；
    - client有数据发送，会把数据和ACK一起发送（即，顺便第三次握手），数据被接收，Server端连接建立；
    - Server有数据发送，但发送不了，会一直周期性超时重传SYN+ACK（第二次握手），直到client确认。 
- 关闭连接：【未完待续】
  - 4次挥手。因为TCP连接是全双工的，所以每个方向都要单独关闭。
  - 收到一个FIN包（FINbit=1），意味着这一方向没有数据流动。也就是说，你给我发FIN，代表你不跟我说话了，但我还可以跟你说话。
  - 第一次挥手：主机1（可以是客户端，也可以是服务器端），设置 Seq # 和 Ack #，向主机2发送一个 FIN 报文段，然后主机1进入 FIN_WAIT_1 状态；这表示主机1没有数据要发送给主机2了，但仍然可以接收数据；
  - 第二次挥手：主机2收到了 FIN 报文段，向主机1回一个 ACK 报文段，Ack # 为 Seq # 加1（表示：我回复的是你上一句话）；主机1进入 FIN_WAIT_2 状态；
  - 第三次挥手：主机2向主机1发送 FIN 报文段，请求关闭连接，同时主机2进入 CLOSE_WAIT 状态；主机2告诉主机1，我也没有数据要发送了，可以进行关闭连接了；
  - 第四次挥手：主机1收到主机2发送的 FIN 报文段，向主机2发送 ACK 报文段，然后主机1进入 TIME_WAIT 状态；主机2收到主机1的 ACK 报文段以后，就关闭连接；TIME_WAIT 时，主机1等待 2个MSL（Maximum Segment Lifetime）后依然没有收到回复，则证明主机2已正常关闭，那好，主机1也可以关闭连接了。


## congestion control

拥塞控制。

- scenario 1，被容量限制：
  - 2 senders，2 receivers，1 router（infinite buffers），output link capacity=R；no retransmission。
  - 两个sender使劲发包，链路上充满了包。包来的很迅速，但被处理的很缓慢。
- scenario 2，超时重传：
  - 1 router（finite buffers）。sender retransmit timed-out packet。
  - 如果重传现象严重（如过早超时），就会congestion。
- scenario 3，多条路径的超时重传：
  - 流量以竞争使用的方式共享。
  - 我和你竞争使用链路，你比我快，因此buffer里很快就全都是你的包了。我先前发过去的包就浪费了（不必要的开销），不得不再换一条路重新发包，因为我卷不过你。

拥塞控制策略：

- 端到端：网络没有反馈，端系统从delay和loss推断。【TCP采用的策略】
- 网络辅助：路由器反馈网络拥塞信息，通过packet里的某个bit（SNA，DECbit，TCP/IP的 ECN、ATM），明确指示发送者发送速率。拥塞控制的信息也可以来源于网络（而非全部来自对面）。

### TCP的拥塞控制【必考】

- 发送方如何感知拥塞？
  - 丢包事件：拥塞造成【丢包】和【分组延迟增大】；无论是丢包还是分组延迟过大， 对于发送端来说都是丢包了。
  - 包括：重传定时器超时，发送端收到3个重复的ACK。
- 发送方采用什么机制限制发送速率？
  - 使用拥塞窗口cwnd，限制已发送未确认的数据量（LastByteSent-LastByteAcked ≤ cwnd）。
  - cwnd是动态的，随发送方感知的网络拥塞程度而变化。
  - 发送速率：rate ≈ cwnd / RTT (byte/sec)。
- 3个机制：
  - AIMD，additive increase（加性增）multiplicative decrease（乘性减）。也被称作拥塞避免状态，congestion avoidance。
    - 每个RTT周期增加 cwnd（拥塞窗口）by 1 MSS（TCP层的最大报文长度，MTU是物理层最大报文长度），直到检测到报文段丢失，此时cwnd折半。
  - slow start，慢启动。
    - 发送方以指数速度增加 cwnd，直到一个报文段丢失事件发生 。
    - initially cwnd = 1 MSS，double cwnd every RTT，done by incrementing cwnd 对每一个收到的 ACK。
    - 增加到阈值，然后线性增加，进入拥塞避免状态（转为AIMD策略）。
  - conservative after packet loss。即，如果检测到丢包：
    - tahoe：慢启动+AIMD → 丢包（timeout或3个重复ACK）→ cwnd=1，ssthresh（慢启动阈值）=1/2 cwnd，转入慢启动状态。
    - reno：
      - 丢包：ssthresh（慢启动阈值）=1/2 cwnd。
      - 收到3个重复ACK，转入快速重传/快速恢复阶段（有些context下也属于拥塞避免），cwnd=1/2 old_cwnd（有时候会cwnd/ssthresh=1/2 old_cwnd+3，因为3个重复ACK，看具体策略描述）。
      - 超时，和Tahoe一样进入慢启动阶段，cwnd=1。
  - 【考法：画图+现象推测】

![image-20211118093313182](.\..\..\typora-user-images\image-20211118093313182.png)

### TCP throughput（了解即可）

是窗口大小的函数，$1.22*MSS/[RTT*(L)^{1/2}]$。

TCP谷歌最新版，high speed。





























