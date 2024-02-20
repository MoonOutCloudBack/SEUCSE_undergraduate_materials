# cn 06 the link layer and LANs

link layer：负责把报文从一个节点、通过链路、传送到物理相邻的下一个节点，链路层寻址不考虑网络层地址。

- 理解链路层服务的原理；
  - 错误检测，纠正。
  - 共享广播信道：多路访问。
  - 链路层寻址。
  - 局域网技术：Ethernet，VLANs。
- 实例化和实现不同的链路层技术。

重点：差错纠正（CRC），多路访问，LAN地址与ARP，switch。

## introduction

术语：主机和路由：node，节点；数据包：帧，frame。

不同链路运行不同的链路层协议，提供不同的服务。

- 向下：利用物理层提供的位流服务，介质访问控制 (MAC)；
- 向上：向网络层提供明确的 (well-defined) 服务接口，逻辑链路控制 (LLC) 。

链路层位于：主机的网卡（网络接口卡）、网络适配器。和系统总线相连。

![image-20211214110857990](.\..\..\typora-user-images\image-20211214110857990.png)

数据链路层的功能：成帧，差错控制，流量控制。（链路层很局部，只着眼于一小段物理链路，无法实现拥塞控制）

framing，成帧：

- 将上层IP报文封装在帧里面，加 header 和 trailer。
- MAC地址放在帧首部，表明源地址、目的地址（与IP地址不同）

## error detection & correction，EDC

parity checking，奇偶校验，二维奇偶校验。

checksum，校验和：16bit 分段，相加，取反码作为校验和。

### CRC校验

cyclic redundancy check，循环冗余校验和：

- 原理：
  - D是原数据，搞一个 r+1 位的二进制数 G，搞一个 r 位的校验和 R。
  - <D, R> 能够被 G 整除。如果不能，就是出错了。
  - 可以检测到所有小于 r+1 的连续 bits 的突发查错。
  - （不是二进制出发，是一种模2除法，异或。
- 生成R：
  - D 左移 R 位，然后除 G，得到 r 位的余数，就是R。

## multiple access protocols

如何协调多个 sender/receiver node，实验一个共享广播信道：多路访问链路协议。

### channel partitioning

信道划分（by time / frequency / code）。

- TDMA（time division multiple access），时分多路复用，轮流占用信道，没有使用的时隙被浪费。
- FDMA（frequency division multiple access），频分多路复用，信道频率被划分为不同频段，每个节点占用一个频段，未使用的频段空闲。
  - 统计时分复用，STDMA。

- CDMA，码分多路复用。

### random access (dynamic)

随机接入协议。信道不分片，允许冲突，要能够从冲突中恢复（recover from collisions）。

- 时隙（slotted） ALOHA：
  - 假设所有帧大小相同，把时间划分为刚好够传完一个帧的时隙。
  - 节点同步（共识时隙何时开始），只在时隙起点开始传输帧。
  - 若在一个时隙中有碰撞（两个节点都把帧传出去了），节点以概率P在后续的时隙中重传它的帧 ，直到该帧被无碰撞地传输出去。
  - 单个活跃节点可以持续以全速率发送数据。简单，高度分布式。
  - 碰撞浪费时隙。信道最大利用率为37%。
- pure（unslotted）纯ALOHA：
  - 帧一到达，就立即传输。
  - 冲突可能性增加。我传了一半，你突然开始传，就冲突了。即，全时间段都可能冲突，而非只能在时隙起点冲突。
  - 信道利用率：18%。
- CSMA（carrier sense multiple access），载波侦听多路访问。
  - 传输前监听，如果信道空闲，transmit entire frame；如果信道忙，则延迟发送。
  - 冲突仍然会出现：传播时延导致节点可能监听不到对方的传输。
- CSMA/CD（collision detection冲突检测），used in Ethernet。
  - 冲突检测：希望短时间内侦听到冲突，然后立刻放弃冲突的传输，减少信道浪费。
  - 冲突检测：在有线LAN中容易实现，测量信号强度，很弱就是冲突了；在无线LAN中难以实现，接收到的信号强度被本地传输的信号覆盖。
  - 冲突产生后，端系统重发数据帧：二进制冲突退避算法。利用率比ALOHA好。
    - 思路：错开重发时间，使之随失败次数增多而增加。
    - 退避时间计算：信号在媒体上的往返时间乘以随机数(r)，(r)的取值范围依赖于冲突的次数(i)，0 ≤ r＜2^i。
  - ![image-20220102091659564](.\..\..\typora-user-images\image-20220102091659564.png)
- CSMA/CA（冲突避免），used in 802.11。

### taking turns

轮流协议。

- polling，轮询。
  - master 一个一个问，接下来你要发包吗？
  - 问题：轮询造成的负载，轮询的时延，单点失效问题（master失效）。
- 令牌传递，token passing。
  - 按照次序传递令牌（token，3个字节的一个小特殊帧），谁有令牌谁就有传输权限。
  - 如果环上的某个工作站收到令牌并且要发送信息，它就改变令牌中的一位（该操作将令牌变成一个帧开始序列），在令牌后追加想传输的信息，然后将整个信息发往环中的下一工作站。
  - 当令牌传到目的站点时，目的站点复制一份令牌帧，然后将整个信息发往环中的下一工作站。
  - 当令牌重新传回发送站点时，check数据是否正确，若没有错误则重新生成一个令牌帧，将令牌往后传递，交出控制权。
  - 优点：不会冲突。
  - 问题：token的传输负载，时延（帧在整个环上走一圈），单点失效（token 忘记释放令牌）。
- Bluetooth, FDDI, IBM token ring。

## LANs

局域网：有限地理范围内，host通过传输媒体（双绞线，传输电缆，光纤，而非router）连接起来。

物理相连的主机通信，不需要IP地址，只需要MAC地址。

MAC：

- 48bit，如 1A-2F-BB-76-09-AD。
- 烧在NIC ROM上，有时用软件可重写。
- 扁平地址结构：没有层次化，portability，可以到处乱走，不像IP地址依赖于子网。

### ARP

Address Resolution Protocol。

每一个IP节点都有一个 ARP table：

- MAC和IP地址的映射表；
- 格式为<IP address; MAC address; TTL>；
- TTL是映射的过期时间，一般是20min。

地址解析协议：

- ARP：逻辑地址→物理地址，
- RARP：物理地址→逻辑地址。
- 需要在同一个局域网（LAN）。即插即用，不需要配置。【物理通信只能在局域网内】
- ARP分组封装在链路层 frame 里，包含网络层地址，可以认为是网络层协议。但也包含链路层地址，在这个角度也可以认为是链路层协议。

ARP：

- 广播请求应答，到处问。

- 报文格式：

  ![image-20211216104307918](.\..\..\typora-user-images\image-20211216104307918.png)

  ![image-20211216104420154](.\..\..\typora-user-images\image-20211216104420154.png)

- 启动时机：发IP包时查ARP表，查不到则自动运行ARP协议。

跨局域网通信：

- 目的MAC地址是网关的MAC地址，而非另一个LAN的目的host的MAC地址，见ppt页67【重要】。
- 物理通信只能在局域网内。
- 经过路由器，源IP地址和目的IP地址不变，但是源mac地址和目的mac地址均已改变。
- 通过局域网内传递+网关路由器跨局域网，接力传递。

### Ethernet

有线局域网。

关于帧长：

- 传统的以太网是共享性局域网，采用CSMA/CD（载波侦听多路访问/冲突检测）协议。
- 最小帧长必须大于整个网络的最大时延位（最大时延时间内可以传输的数据位）。 
  - 因为，如果我们自身数据全部发完后，才检测到冲突，则数据会被误认为是正常的数据而被接收，所以我们要确保，还没有发完数据时，就检测到碰撞。
  - 如果帧长度太小，就可能出现网络上同时有两个帧在传播，就会产生冲突（碰撞）而造成网络无法发送数据。
- 如果数据帧太长，就会出现有的工作站长时间不能发送数据，而且可能超出接受端的缓冲区大小，造成缓冲益出。

Ethernet拓扑：

- bus（会有碰撞）。
- star：hub，集线器，物理层设备，作用于bit。后来被交换机switch替代，交换机是无碰撞的。
- 现在流行的是star，如果要嗅探别人的包，需要交换机的权限。

以太网的帧结构：封装IP报文为 Ethernet frame。

![image-20211216110428057](.\..\..\typora-user-images\image-20211216110428057.png)

- 前同步码 preamble：7个10101010 + 1个10101011，用来同步时钟（适配器传输速率偏移）。
- addr：都是 6B 的 MAC。适配器收到非本人MAC地址/广播地址的帧，丢弃。
- CRC：4B。type：2B。

不可靠，无连接：TCP的 timing 机制会察觉 ethernet 丢包。

Ethernet’s MAC protocol: unslotted CSMA/CD with binary backoff，存在冲突域。没有看懂，详见课本p324。

网桥：以太网通过网桥连接，成为更大的以太网。

### switches

看这个视频，讲的蛮好的：https://www.bilibili.com/video/BV12E411B7gm

即插即用 plug-and-play，自学习 self-learning，交换机不需要配置。

switch的接口没有MAC地址。

一些特点：

- 消除碰撞：交换机中没有因为碰撞浪费的带宽，交换机缓存帧，并且绝不会在网段上同时传输多于一个帧。
- 异质链路：交换机将链路彼此隔离。
- 冗余路径：交换机会造成广播帧循环，出现广播风暴。

创建交换表：

- 通过每个接口收到的frame进行学习。
- 当收到帧，交换机记录地址/位置，“learns” 发送节点的 位置和所在的局域网段。

帧的过滤 & 转发：

- 过滤（filtering）：转发还是丢弃；转发（forwarding）帧应该被导向哪个接口。
- 当switch收到frame：
  - 记录当前 sending host（大概就是上一跳吧）的 MAC addr。
  - 查询 switch table，得到dest。
  - 如果查到了，forward。
  - 如果没查到dest：flood，大概就是把这个frame broadcast？

交换机 & 路由器：

- 存储转发：
  - routers：网络层设备（检查网络层报文首部）；
  - switches：链路层设备（检查链路层报文首部）。
- 转发表：
  - routers：使用路由算法根据IP地址进行转发；
  - switches：使用洪泛、自学习、根据MAC地址进行转发。

VLAN：控制广播域。广播域是广播帧能够到达的范围，缺省情况下，交换机所有端口同属于一个广播域，无法互相隔离，会产生广播风暴。

## 综合实例

一台新电脑想访问 www.google.com 。

- 知道IP地址：DHCP（discover）：
  - DHCP → UDP → IP → 802.3 Ethernet。
  - 交换机收到 → 转发 → 路由器收到 → 广播 → 网关收到 → 网关一般部署了DHCP服务器。
  - DHCP discover（谁给我一个IP地址） → DHCP offer（你看看这个怎么样） → DHCP request（嗯好的我想要它） → DHCP ACK（ok没问题它是你的了）。
- 知道DNS server IP地址与MAC地址的对应关系：ARP：
  - 我们先发一个 src=dest 的ARP报文，让大家知道我的IP和MAC对应关系。如果周围有人跟我的IP重复了，会收到一个ICMP差错报告。
  - ARP 查询 DNS server 的IP地址，如果DNS server在本地，则得到DNS的MAC。如果不在本地，router reply，告诉我router的MAC。
- 知道 www.google.com 的IP地址：DNS：
  - IP datagram 从校园网到外网，被【RIP，OSPF，IS-IS，BGP】算法 route 到 DNS server。
- 给 www.google.com 发HTTP报文：TCP：
  - 三次握手。
  - TCP发了一个HTTP request，google 的 server 回复。

## 刘波老师的一个答疑

- 请问BGP使用TCP半连接，OSPF被封装在IP报文里，为什么说它们是网络层协议呀？

  - BGP和OSPF比较特殊，在网络层使用，所以称他们为网络层协议。但又在IP中封装，这也反映了TCP/IP架构的分层不够清晰之处。 

- 请问为什么不一定“一个IP地址和一个接口对应”呢？是因为在不同的内网中，不同的主机可以拥有相同的IP地址，因此一个IP地址可以对应多个接口吗？

  - 在公网上，一般来说，一个IP地址与一个接口对应，但有内部地址的情况，Internet上有很多主机会使用同一个内部IP地址。从这个角度理解是不一一对应的。【一个IP → 多个 MAC】

  - 而且，还有些特殊情况，假设有两台设备都想在本地网络上提供相同的服务，它们可以以一种方式编程，这将允许访问负载在两者之间分配。这需要特殊配置。

  - 一台设备或接口可以拥有多个 IP 地址吗？是的，有些系统可能允许创建与接口或设备相关联的某种类型的多个“虚拟”接口来提供此功能。【一个MAC → 多个IP】



















