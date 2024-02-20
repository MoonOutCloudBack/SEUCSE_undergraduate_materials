子网划分 + 路由必然要考。

拥塞控制 / 写seq# ack# / 算校验和 / 算第一章时延应该是选择题。

对于任何过程，都请考虑DNS、ARP、DHCP。



## 协议汇总

- 应用层：
  - HTTP（80 TCP）
  - DNS（53 UDP）：想上网 / 查电子邮件服务器，域名 → IP地址。
  - SMTP（25 TCP），POP3（110 TCP），IMAP（143 TCP）
  - DHCP（c→s 68 | s→c 67 & UDP）：动态获取host的IP地址。
    - discover：广播，目的IP全255，目的MAC全FF。
    - offer：单播 / 广播。
    - request：广播，目的IP全255，目的MAC全FF。
    - ack：告诉我 IP地址、缺省路由器IP地址（第一跳路由器地址，默认网关）、DNS服务器IP地址、网络掩码。
- 传输层：TCP（首部至少20B），UDP（首部8B）。
- 网络层：IP（首部至少20B），ICMP（IP，在主机与路由器之间传递控制信息）。
- 路由协议：BGP（179 TCP半连接 应用层/网络层√），OSPF（IP 传输层/网络层√）。
- 地址解析协议：ARP（frame 网络层/链路层√），IP地址 → MAC地址。
  - 报文格式：包含发送者IP+MAC，想知道的IP+MAC。
  - operation字段：request 1，reply 2。


看为哪一层的功能服务（得知网络层路径，得知链路层地址）。





## 简答/计算/大题押题

### 概述

服务与协议的比较。

分组交换和电路交换的比较。

解释存储转发、尽力而为。

算存储转发 / 电路交换时延。

### 应用层

网络应用程序的体系结构：CS和P2P的比较。

填seq# ack#。

三次握手 & 四次挥手。

cookie是什么，有什么作用。

【email整个过程】

SMTP与HTTP的比较。

DNS迭代查询与递归查询的比较。

### 传输层

TCP/UDP 多路复用分解的区别。

拥塞控制 & 流量控制的区别。

TCP拥塞控制，cwnd变化图。

计算UDP校验和。

简述rdt协议的推导过程，总结可靠性机制。

计算TCP可靠传输流水线的信道利用率。

【TCP & UDP 的 原理 & 应用场景】

### 网络层

IP地址划分子网，写子网路由表。

IP报文分片+重组。

link state / distance vector 算节点间距离。

若支持主机接入Internet，必须配置哪些设施。（比如说DHCP、DNS、router、OSPF（？）、BGP（？））

【tracert实现原理】

### 链路层

【简述 CSMA/CD 和 CA 的应用背景 & 工作原理】

计算CRC校验码。

为什么链路层对帧的长度有限制。

简述交换机自学习的过程。

### 综合

【访问一个网页的全过程】

地址有哪些，作用是什么，如何互相映射，映射关系怎么样（一对一 / 一对多 / 多对多）。

定时器与缓存在哪里有应用？（应用层 传输层 网络层）



## 玄学小题押题

### 概述

ARPANET创新地采用了分组交换技术【判断】。

流量强度 = La/R，R 为链路带宽，L 为分组长度，a 为平均分组到达速率。

RFC是请求评论的意思。Internet标准以RFC文档的形式公开。

### 应用层

需要提供安全的应用层服务，通过什么来实现？应用层自己增加安全性，或者SSL。

HTTP 1.1缺省为持久连接，支持流水线机制（可以连发好多请求，而非收到响应后再发下一个请求，响应需要按序），经历较少的慢启动过程，减少往返时间。

TCP连接何时关闭？keep alive字段为close时，传完这次就关闭。

web cache 的 conditional GET：在HTTP request msg中，If-modified-since:\<date>。未修改：返回状态是304（Not Modified），继续用 cached data；修改过：200+新数据。

HTTP（80 TCP），DNS（53 UDP），SMTP（25 TCP），POP3（110 TCP），DHCP（68 UDP），BGP（179 TCP半连接）。

POP3无状态，IMAP有状态。

在电子邮件的协议首部，能够发现报文发送主机的 IP 地址吗？答：不能，首部存放的是发送主机的主机名，需要通过 DNS 服务转换成 IP 地址。

BS架构采用 标准的TCP/IP和HTTP协议（选择题）。

socket 是 os 提供的一系列用于网络通信的接口，位置在应用层和传输层之间。

#### DNS

DNS的区域传输使用了TCP连接（好像是改进版），不过地址解析仍然是UDP。

root DNS server：13个根节点，标号A到M。每台 root server 是一个冗余服务器网络，不是单个服务器。

本地DNS服务器，local DNS server，不属于DNS层次架构。

DNS 使用 UDP：报文数据量小，收到损坏的包可以重发。

DNS记录格式：(name, value, type, TTL)。

- TTL：生存时间，决定 record 应当从 cache 中 remove 的时间。
- type=A：name 是 hostname，value 是 IP address；
- type=NS：name 是 domain (e.g., foo.com)，value 是 domain 的 authoritative server 的 hostname；
- type=CNAME：name 是某台主机的别名，value 是 规范主机名。
- type=MX：value 是别名为 name 的 mailserver 的 规范主机名。一个公司的邮件服务器和其他服务器可以使用相同别名，一个通过CNAME查规范主机名，一个通过MX查。

### 传输层

rdt3.0 又叫做 alternating-bit protocol。

TCP的seq#，是字节流编号！

无连接服务不能提供流量控制，因为不能协商窗口大小。

### 网络层

缓解IP地址不够用的手段：CIDR、NAT、IPv6。

IP 头部 TTL 减 1，IP 头部 校验和 更新。

IP的偏移字段单位是8B：因为 offset 13位，length 16位，中间差3位。为了充分利用offset字段。

DHCP 租期：长短跟对地址数量的需求量有关系，假如有大量用户，为了避免地址不够用，应该设短，如果设长了，可能会导致有人拿不到地址而无法上网。如果是家庭网，可以设置很长。

DHCP 使用UDP：因为不知道server在哪，只能广播，因此不能使用面向连接的TCP。

NAT router：IP datagram 出NAT子网时，router 会改 src IP地址；进NAT子网时，router 会改 dest IP地址。好像不止如此，router还会魔改 TCP / UDP 的 port。

OSPF应用在大规模网络：本身就收敛快。层次结构（area 是划分的OSPF域，连接 area 的 router 叫做 backbone）。

router的转发匹配：最长前缀匹配，硬件实现（三态内容寻址存储器，TCAMs，1s一百多万条）。

【判断题】域内路由器的转发表仅由IGP协议决定。【错误】因为想出AS就要去网关，这个转发表项就不是IGP协议决定的。

域内路由器也在跑EGP协议（iBGP）。

### 链路层

目的IP地址全1，目的MAC地址也全F。

switch的接口没有MAC地址。

经过路由器，源IP地址和目的IP地址不变（不一定），但是源mac地址和目的mac地址均已改变。

IEEE制定的无线局域网标准802.11。

二层交换机连接的、具有不同 IP 网络号的两台主机，不可以直接通信。





