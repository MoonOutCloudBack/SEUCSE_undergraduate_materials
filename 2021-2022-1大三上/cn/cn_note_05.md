# cn 05 network layer: the control plane

每个router运行一种路由算法，与其他router交互，计算forwarding table。

算法分类：全局（complete topology）& 分布式，静态（直接确定路由表）& 动态，负载敏感（ARPAnet）& 负载迟钝（RIP、OSPF、BGP）。

部署局域网/路由（防止生成环）：最小生成树，MST。

本章框架：

- routing protocols：
  - link state。
  - distance vector。
- intra-AS routing in the Internet: OSPF。
- routing among the ISPs: BGP。
- The SDN control plane（了解即可，没有讲）。
- ICMP: The Internet Control Message Protocol。
- Network management and SNMP（也没有讲）。

## routing protocol

### link state algo

global。

每个节点得知global情况（n个节点，nE的message开销），然后自己算自己的路由表。（当某个结点的链路状态改变时，开始洪泛。）

输入：网络拓扑+所有链路的开销。

可以使用dijkstra（时间复杂度：n nodes，遍历 o(n²)，堆 o(nlogn)）。

如何得到【网络拓扑+所有链路的开销】？链路状态路由协议：

- 发现邻居，了解他们的网络地址。
  - 发送hello报文。

- 设置到每个邻居的成本度量。
  - 开销 / 度量 / 代价（边权：自动发现设置或人工配置；
  - 度量：带宽、跳数、延迟、负载、可靠性等；
  - 常用度量：链路带宽（反比）；
  - 可选度量：延迟。发送一个echo包，另一端立即回送一个应答。通过测量往返时间RTT，估计延迟。

- 构造一个分组，分组中包含刚收到的所有信息。
  - 链路状态分组（link state packet，LSP)：发送方标识、序列号、年龄、邻居列表。

- 将此分组发送给其他的路由器（泛洪 Flood）。
  - 每个LSP分组包含一个序列号，且递增；
  - 路由器记录所收到的所有（源路由器、序列号）对；
  - 当一个新分组到达时，路由器根据记录判断：
    - 新分组，洪泛广播；
    - 重复分组，丢弃；
    - 如果是过时分组，拒绝；

- 计算到其他路由器的最短路径。如dijkstra。

问题：震荡（oscillations）：

- 链路开销（边权）= 链路负载，我去走哪一条路，哪一条路开销就增大。
- （我选好路之后，就洪泛告诉别人。不会出现，我得知你可能的选择，又改变了自己的选择。）
- 现象：大家每次发包都换路。

（路由器在同一个子网内，因为可以直连。）

### distance vector algo

定期和相邻交换全部路由表信息

距离矢量 $d_x(y)$ = x到y最短路的代价。

bellman-ford：

- 路由表格式：dest，距离，下一跳节点。
- 每个节点一开始只知道与邻居的路径。
- 节点A的邻居收到A的更新路由表，用bellman-ford来松弛一波，看看自己下一跳过A，能否与其他节点距离更近。如果更近，更新自己的路由表 & 告诉自己的邻居；如果没有更近，不更新路由表，也不会告诉邻居。

特点：

- 迭代、异步：本地迭代触发：本地链路开销发生变化 / 相邻节点 DV 更新消息来到。
- 分布式：当一个节点的 DV 变化时，会通知所有邻居；有必要时，相邻节点又会通知它的相邻节点。

（想到了1044，感觉可以这么做）

- 问题：好消息传的快，坏消息传得慢。无穷计数（不做要求）。
- 一定程度的解决：毒性逆转。我要通过你，你就不要通过我。（不做要求）

### 全局算法 link state & 距离向量 DV 的比较

- 消息复杂度（发送报文个数）：
  - DV：在邻居间交换数据，看情况（听说比LS更好）。
  - LS：n nodes E links，O(nE) msgs sent（比DV差应该是因为，一点变化就要传遍全局）。
- 网络状态信息交换的范围：
  - DV：邻居间交换。
  - LS：全网扩散。
- 网络状态信息的可靠性：
  - DV：部分道听途说。
  - LS： 自己测量。
- 健壮性：
  - DV：计算结果传递，健壮性差。
  - LS： 各自计算，健壮性好。
- 收敛速度：
  - DV：慢，可能有计数到无穷问题。
  - LS：快。算法复杂度O(n²)。

### 层次路由

routers不可能在一个平面内，否则就光交互确定路由表了，迟迟传不了包。

基本思路：

- 把网络分成自治系统（AS，automonous system）。
- 不同的AS，可以使用不同的路由算法 / 路由协议
- 每个AS有一个全球唯一的ID号：autonomous system number，ASN。
- 自治系统内部，使用内部网关路由协议（Interior Gateway Protocols，IGP）。
  - 典型IGP协议：OSPF【重要】，RIP，IS-IS，IGRP，EIGRP……
- 自治系统之间，使用外部网关路由协议（Exterior Gateway Protocols，EGP）。
  - 各自治系统域之间的路由需统一。
  - 典型EGP协议：BGP。

我们需要考虑：inter-AS routing（域间路由）& intra-AS routing（域内路由）。

网关（gateway router）：在AS的边缘，和其它AS相连，执行域内和域间路由的功能，身兼两职。

【判断题】域内路由器的转发表仅由IGP协议决定。【错误】因为想出AS就要去网关，这个转发表项就不是IGP协议决定的。

域内路由器也在跑EGP协议（iBGP）。

## intra-AS routing in the Internet: OSPF

OSPF，open shortest path first。

使用 link state 协议：发Hello报文 - 建立邻接关系 - 形成链路状态数据库 - dijkstra算法 - 形成路由表。

就是一个简单的寻最短路。

链路cost是管理员配置的。

OSPF 消息直接承载在 IP 数据报中（而非TCP / UDP），有5种报文（详见参考资料），不是ICMP通告。

OSPF属于哪一层：有些人说OSPF使用IP协议的功能，应该在IP层之上，即传输层；也有些人说OSPF是路由协议里的一种，所以应该属于网络层。根据参考资料，OSPF报文直接封装在IP数据包内，专门为TCP/IP网络而设计的路由协议，因此属于网络层。

参考资料：http://www.360doc.com/content/18/0723/21/11935121_772706744.shtml

优点：

- 允许多条相同代价的路径（RIP只允许一条），大家分道扬镳，让多条路径承担流量。
- 为不同的服务类型计算各自的路由集。
- TOS (e.g., satellite link cost set low for best effort ToS; high for real-time ToS：传输质量) （不知道什么意思）
- integrated uni- and multi-cast support，同时支持单播和多播。
- hierarchical OSPF in large domains，层次结构。
- 支持子网：子网掩码与每个路由通告相连。（不知道什么意思）
- security：受信任的路由器才能参与OSPF协议，一堆措施（MD5等）。

层次化的OSPF：

- 大家都在跑OSPF，但是是层次化的。仅内部洪泛。
- 内部路由器（Internal Router），区域边界路由器（Area Border Routers，ABR，连接IR和骨干），骨干路由器（backbone router），自治系统边界路由器（Autonomous System Boundary Router，ASBR）。【了解即可】

## routing among the ISPs: BGP

可以看这个视频：https://www.bilibili.com/video/BV1KJ411C73y

![image-20220103115030576](.\..\..\typora-user-images\image-20220103115030576.png)

Border Gateway Protocol。至关重要的协议，将所有东西粘合在一起。

使用 distance vector 协议。

AS内部：iBGP。外部，AS间：eBGP。gateway router 执行 eBGP + iBGP protocols。

BGP session：两个不同 AS 的 gateway router，通过179端口的TCP半连接交换信息（不代表BGP是应用层、可以调用TCP，不要深究）。

通告，BGP path advertisement：

- 网关路由器广播给其他的网关路由器。
- 其他网关路由器广播给AS内的所有路由器。

通过通告，让每个AS知道 经过相邻的AS可达哪些目的地：目的地不是 host IP 地址，而是 CDIR 化的前缀，每个前者表示一个子网（的集合）。

前缀中包含两个重要属性：（带有属性的前缀：路由，route）

- AS-PATH：列出路径上所有的AS，该通告已经经过的AS。
- NEXT-HOP：指明路由下一跳。

有多条AS路径时，选哪一条把包发出去：AS内的热土豆算法。看两个NEXT-HOP，哪个离我代价更小，就选哪个，赶快把包发出去。

当 gateway router 接收到通告，根据【策略】（为自己的付费用户服务，另一端不是我的付费用户，因此不打算让另一端知道我这里也可以走，把它的包丢了）决定是否结束该路由，设置相应属性（如偏好值，是否偏好走这条路径）。

到达一个目的地前缀，可能有多条路由，仅将最优路由加入路由表：

- 首先丢弃下一跳（Next hop）不可达的路由。
- 选偏好值最大的；
- 如果选不出来，选AS_PATH最短的；
- 如果选不出来，选最靠近NEXT_HOP的。
- 如果选不出来，按照BGP标识符选。

总结，路由的基本思想：

- 层次化的路由：节省路由表，减少路由更新流量。
- AS内：单个域内，不需要复杂策略，着重于性能。
- AS间：希望控制路径，策略（是否通告）胜过性能。

## ICMP: Internet Control Message Protocol

被封装在IP数据报里。（看起来和TCP UDP同一层）

格式：类型字段+编码字段。

![image-20211214103453751](.\..\..\typora-user-images\image-20211214103453751.png)

TTL：

- 由发送端初始设置的一个 8bit 字段。（例如：64）
- 每个路由器把TTL减1，或减数据报停留的秒数。大多数延迟小于1秒，因此TTL成为跳站计数器。

![image-20211214104304336](.\..\..\typora-user-images\image-20211214104304336.png)

注意 (9,0) 是路由通告。

差错报告报文：

![image-20211214104410827](.\..\..\typora-user-images\image-20211214104410827.png)

### 应用实例：linux 的 traceroute、windows 的 tracert

- 追踪路由。知道路径和时间。
- 一条路径上，每个设备Traceroute要测3次。
- 源主机发送不可达的 UDP 报文到目的地，先设置 TTL=1，然后 TTL=2，…
- 当报文送到第n个router：把报文discard，send source ICMP message（type 11 code 0，TTL过期的警告报文）。ICMP message 包括 router 的 name & IP address。
- 当UDP报文送达：我们的 destiny port number 在30000 以上，dest host 会送回一个 type 3 code 3 ICMP port unreachable。

### 应用实例：Ping

- 发送 ICMP 回显请求报文（type 8 code 0），测试一台主机是否可达。只保证，当前主机与目的主机间，存在一条连通的物理路径。
- 测试连通性可以用PING命令。（甚至可以判断对方操作系统的类型，通过TTL。WINDOWS 95/98：32，UNIX：255，LINUX：64，WIN7/10：64）
- Ping不通：超时，某些端口不让ping，NAT会导致单向ping通，ICMP包被某些节点过滤，网线刚插到交换机上就Ping通网关（没懂）。









