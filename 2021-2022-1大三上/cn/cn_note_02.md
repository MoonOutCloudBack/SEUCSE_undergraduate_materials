# cn 02 application layer

应用层：

- 三种应用架构（application architecture）：C/S，P2P，hybrid。
- 进程通信：相同主机间，不同主机间。
- Web和【HTTP】：
  - 两种连接方式；
  - HTTP协议；
  - cookie；
  - Web缓存：条件get。
- 电子邮件协议：邮件的组成，【SMTP，POP3，IMAP】。
- 【DNS】协议：体系架构，两种查询方式，报文格式。
- P2P协议：文件分发，BitTorrent。

判断题：传输层调用的服务一定是UDP（不对），有时也可能用TCP。但SMTP那些一定是TCP。

## 应用层协议原理

应用层的位置在终端系统上。

### 三种application architecture

- client-server：
  - server运行在host上，IP地址固定。
  - client可能有动态的IP，只与server通信，间歇性请求连接。两个client之间不互相通信。
- P2P：
  - 不需要服务器，请求服务的同时也提供服务（self scalability，自扩张 性，新人带来了服务资源，也增加服务需求）。
  - 同时有客户和服务器进程。
  - IP地址会变，间歇性请求连接。
  - （skype是基于P2P的，效果特别好。）
- hybrid of CS and P2P。

### 进程通信

- 同一host：os（shared memory/message passing）。
- 不同host：socket。

#### socket

- 进程与计算机网络之间的软件接口。
- 进程类比做房子，socket类比做门；想要发送报文时，把报文推出门。
- ip地址唯一识别cn中的端系统。
- 定位目标进程：主机的IP地址+端口号（进程标识符，更具体地说是socket标识符）。仅凭IP地址不能识别进程。

应用层协议规范：要定义/设计的东西。

skype是基于P2P的，私有的，效果特别好。

### 可供application使用的传输服务

下层（运输层，transport layer）提供的服务。

application的需求分类：

- 可靠性：正确+完整，比如在线交易。视频传输无所谓。

- 实时性，timing，比如电话会议、打游戏。

- 吞吐量，throughput：比如多媒体应用。带宽敏感（bandwidth-sensitive）应用具有特定的吞吐量要求，而弹性（elastic）应用没有。

- 安全性（encryption，data integrity）。可能由应用层实现而非运输层。


可靠性：保证送到对面；安全性：送的过程不让别人顺便获取信息。

### 传输层提供的服务：TCP & UDP

- TCP：
  - 面向连接服务：3次握手后，连接在两个socket之间建立，全双工（同时进行报文收发）。
  - 可靠的数据传输服务：无差错、无丢失冗余、交付顺序相同。
  - 流量控制，拥塞控制（如果网络拥塞，给发送方信号，发送方调整。不一定给通信进程带来好处，但能给internet带来整体好处）。
  - 不管：及时性、最小吞吐量、安全性。
- UDP：
  - 无连接，进程通信前不握手。
  - 只提供不可靠的数据传送，不保证送过去，不保证送到的信息正确，可能乱序送到。剩下什么也不提供。
  - 没有那么麻烦，报文头小。上来就数据传输，很直接。

应用层和传输层的协议对应：

- 邮件、telnet、网页、文件都是TCP；
- YouTube、网络电话有可能是UDP。

TCP的安全增强：SSL，secure socket layer。

==问题：需要提供安全的应用层服务，通过什么来实现？应用层自己增加安全性，或者SSL。==

## Web 和 HTTP

万维网：world wide web，一个新型应用。

web的核心：应用层协议，超文本传输协议，HyperText Transfer Protocol。

### 背景知识

HTTP由两个程序实现：客户端+服务器。

- 服务器：
  - 提供Web页面（HTML文档）：包含多种对象和链接。网页由base HTML-file组成，里面有referenced objects。
  - 存储Web对象（静态/动态对象）：HTML文档、图像、视频、声音、脚本。
  - 对象用URL编址：`协议类型://主机名:端口/路径和文件名`。
    - 常用协议类型：http 超文本，https 安全超文本，ftp 文件传输，file 本地文件 ，mailto 发送邮件，rtsp 流媒体，sip 多媒体呼叫。
- 客户端：
  - 发出请求，接受响应，解释HTML文档并显示。（有些对象需要浏览器安装插件。）

WWW性能提升方法：

- 缓存（caching），如代理技术。
- 多线程（multiple threads）。
- 前端（front end）。

#### Web网页对象

静态对象与静态网页：

- 文本、表格、图片、图像、视频等多媒体类型的信息（实现语言：标记语言，如：HTML，XML，PHP等）。PPT上有一个HTML与XML的比较。
- HTML文档与静态网页：
- 字体、颜色、布局等风格类型的信息（层的样式表CSS）

动态对象与动态网页：（好像是了解即可）

- 通用网关接口（CGI，common gateway interface）。
- 脚本语言+数据库技术。
- 脚本：一个程序，被解释程序（如Web浏览器）解释执行，而不是由计算机的处理器来解释/执行。

- 最好的动态Web开发形式：显示与脚本程序相分离（显示+脚本+CSS3）。


### HTTP概述

HTTP在transport layer通常使用TCP协议，缺省使用TCP的80端口。

#### 无/有状态协议

HTTP是无状态协议，服务器端不保留之前请求的状态信息。

- 无状态协议：效率低，但简单；

- 有状态协议：维护状态相对复杂，需要维护历史信息；在客户端或服务器出现故障时，需要保持状态的一致性等。


#### 发展现状

- HTTP/1.0（1996）
  - 无状态，非持久连接。
  - GET，POST，HEAD。
- HTTP/1.1（1999）
  - 新增 PUT，DELETE。
  - 支持长连接和流水线机制。
  - 缓存策略优化、部分资源请求、断点续传。
- HTTPS：HTTP+TLS（2008）
  - 增加SSL/TLS（TLS 1.2）层，在TCP之上提供安全机制。
- HTTP/2.0（2015、2020）
  - 目标：提高带宽利用率、降低延迟。
  - 增加功能：二进制格式、TCP多路复用、头压缩、服务端推送。

#### 非持久连接 & 持久连接

非持久连接：（非持续性连接）

- HTTP1.0，传一个对象需要三次TCP握手，然后传对象，响应，接着就关闭连接。传一个对象，就需要走一次这个过程。
- 获取每个对象需要两阶段：建立TCP连接、对象请求和传输。每次连接需要经历“TCP慢启动阶段”。
- RTT：
  - 测量：一个小数据包从client到server然后返回client的时间。不要传大文件，否则会把传文件的时延算进去。
  - 传一次，如果是小文件，需要两个RTT：(握手3次+传1次) / (往返的2)。
  - 一般情况：2RTT+file transmission time。
- TCP的三次握手：
  - 我要给你发消息！
  - 我知道你要给我发消息了。
  - 我知道你知道了，那么我要开始发啦~

持久连接：（持续性连接）

- HTTP1.x（如1.1），在相同的TCP连接上，服务器接受请求、响应，响应后仍保持连接。
- 1.1缺省为持久连接，支持流水线机制（可以连发好多请求，而非收到响应后再发下一个请求，响应需要按序），经历较少的慢启动过程，减少往返时间。
- 传一次只需要一个RTT（在连接已建立好时）。
- TCP连接何时关闭？keep alive字段为close时，传完这次就关闭。

#### HTTP报文

报文结构：

- 开始行：
  - 在请求报文中：也被叫做请求行，request line。
  - 方法+所请求资源的URL+版本+CRLF（回车换行，非常重要）。（这个+是空格，各部分用空格连接。）
    - 方法：对【所请求的对象】的操作，也就是命令。
    - GET：请求读取。
    - POST：添加信息（如注释）
    - PUT：（1.1）上传文件。
    - HEAD（1.0），DELETE（1.1）。
  - 在响应报文中：也被叫做状态行，status line。
  - 版本+状态码+短语+CRLF。
    - 状态码：200 ok，301 moved permanently（资源永久性移动到别的地方了，会给出新url），400 bad request（server看不懂的request），404 not found，505 HTTP版本不支持。
- 首部行，header lines。date，server，last-modifide，keep-alive，connection。
- 实体主体，entity body。

示例报文详见ppt。

### cookies

记录用户状态信息。（不需要注册账号）

- 请求报文的cookie首部行。
- 响应报文的cookie首部行。
- 用户端系统存放cookie文件，浏览器进行管理。
- web站点维护后端数据库。

应用：认证 authorization、购物车、推荐 recommendations、会话 user session state（接着上次继续）。

隐私泄露的危险：拿到别人的cookie，假装成那个人，翻遍那个人的浏览记录。https是加密的，可以有效避免风险。

- 对：
  - cookie保存的只是文本串，没有可执行程序。
  - cookie可以跟踪用户网络浏览痕迹，收集用户信息。
  - 可以通过cookie攻击用户。
- 错：
  - cookie容易嵌入间谍程序。
  - cookie只存在于服务器端，在客户端没有任何信息。

### web caches

proxy server，web服务的代理，提供缓存机制。

cache通常由ISP来部署（大学、公司、residential ISP），其实也可以运行在client和server端。

节省带宽，减少响应时间。

conditional GET：

- 在HTTP request msg中，If-modified-since:\<date>。
- 未修改：返回状态是304（Not Modified）。
- 修改过：正常，200。

#### 加速比：定量计算

total delay=internet delay（2s）+access delay（受带宽限制，瓶颈）+LAN delay（超快，微秒级）。

加入cache，命中率为0.4。

total delay=0.6*(internet delay+access delay（变小）)+LAN delay。

访问网络的结构，见ppt页72。

## electronic mail

三个组成部分：

- user agent，用户代理。
  - composing, editing, reading mail messages。
  - e.g., Outlook, Thunderbird, iPhone mail client。
- mail server，邮件服务器。
  - 出去进来的邮件都保存在服务器上。
  - mailbox 保存 incoming messages。
  - message queue 保存 outgoing (to be sent) messages。
  - mail servers 之间：SMTP 协议。
    -  sending mail server 对应 client。
    - receiving mail server 对应 server。
- SMTP，simple mail transfer protocol，简单邮件传输协议。
  - 使用【TCP】可靠传输。使用端口25。
  - 传输的三个阶段：握手，传输，关闭。
  - 持久连接。
  - 格式：
    - commands是ASCII文本，response是状态码+简短说明。
    - 消息全部由7-bit ASCII字符组成。
    - `CRLF.CRLF`，回车换行.回车换行，表示邮件结束。
  - SMTP一般不使用intermeiate mail server。

发送邮件的过程：

- dolce用 user agent 写了一份信给cresc。
- dolce的 user agent 通过SMTP把信送给 mail server，server把它放在message queue里。
- 跑在dolce的mail server 的 SMTP客户端 的 client side 与 cresc 的 mail server建立TCP连接。
- SMTP的client送出邮件，cresc 的 mail server 收到之后，将邮件放在 mailbox里。
- 几分钟后，cresc用 user agent 读邮件。

SMTP client与SMTP server交流的过程：

- HELO, MAIL FROM, RCPT TO, DATA, QUIT 是一些命令。
- 详见课本页91，和ppt页79。
- 使用`telnet servername 25`，可以不使用user agent发送邮件。

SMTP与HTTP的对比：

- 相同点：
  - 进行文件传送时，都使用持久连接。面向连接：可靠性强。
  - 都使用 ASCII 的 command/response 交互；
  - 都使用状态码（220，service ready，服务就绪）。
- 不同点：
  - HTTP：拉 pull，使用 HTTP 从 web server 拉取信息。SMTP：推 push，sending mail server 把文件推向 receiving mail server，该TCP连接由sending方发起。
  - SMTP要求每个报文（包括报文体）使用 7bit ASCII 码，所以非 7bit 的 ASCII 字符或者二进制数据（如图像），必须按照 7bit ASCII 进行编码。HTTP没有这个限制。
  - 发送文本+图形时，HTTP：每个对象都包含在一个 response message 中；SMTP：多个对象在同一个消息中发送。

### 邮件访问协议

逻辑 & 动机：

- mail server在自己的电脑上跑，为了及时接收邮件，电脑就要一直开机，这是不现实的。
- 因此，我们在PC上跑一个 user agent，它访问【存储在mail server】上的某一用户的邮箱。
- 这个访问不能用SMTP，因为取回报文是一个pull操作，而SMTP是push协议。
- 邮件访问协议：POP3 (post office protocol-version 3)，IMAP (internet mail access protocol)。

协议的比较：

- SMTP: delivery/storage to receiver’s server，发送邮件的协议，只是存到了server上。
- 邮件获取协议： retrieval from server，从server那里取过来，接收邮件的协议。
  - POP：Post Office Protocol [RFC 1939]，认证、下载。
  - IMAP：Internet Mail Access Protocol [RFC 1730]。更多特征，能够在服务器上对邮件操作。
  - HTTP：浏览器收邮件，gmail、Hotmail、Yahoo! Mail 等。

POP3 protocol：

- server的回答有两种：+OK（紧跟数据）命令正常，-ERR 命令错误。
- 两个阶段：
  - 认证 authorization，user agent 以明文形式发送 username 和 password。
  - 事务处理，user agent取回报文。list：list message numbers；retr：retrieve message by number；dele：delete，标记为删除。
  - 更新：user agent 发出quit命令后，删除被标记为删除的报文。
- 和IMAP的区别：
  - download & delete模式，报文下载到一个host（PC）之后，别的host（我的手机）就不能访问报文了。download & keep模式没有这个问题，IMAP也没有这个问题（远程服务器上的层次文件夹，所有消息保留在server上 ，所有用户通过文件夹组织消息）。
  - 是无状态的协议，IMAP的session保留用户状态。

## DNS，domain name system

主机名 hostname 用来记忆，IP address 标识主机位置。域名系统（DNS）提供hostname & IP address 的 mapping。

DNS：

- 由分层的DNS server实现的分布式数据库；
- 一个使得【host能够查询分布式数据库】的应用层协议。基于CS模式，运行在【UDP】上，使用53号端口。
- 其他服务：主机别名 host aliasing，邮件服务器别名 mail server aliasing，负载分配 load distribution（繁忙的website被冗余分布在多台server上，每个都有不同的 IP address，因此 主机名 <-> IP地址集合。DNS server 每次响应会回复整个集合，但次序是循环队列的样子）。

user host 获得IP地址的过程：

- 同一台 user host 上，跑着DNS应用的client。
- 浏览器从URL中抽取主机名，将主机名传给 DNS client。
- DNS client 向 DNS server 发送请求，一段时间后收到响应。
- 浏览器向该 IP地址 的80端口 HTTP server 进程发起TCP连接。

### DNS的工作原理

DNS client 向 DNS server 发送请求。

如果internet里面只有一个server，问题：

- 单点故障，a single point of failure。server崩溃，整个internet就瘫痪了。
- 流量负载，traffic volume。单个服务器处理所有查询。
- distant centralized database，单个server不可能邻近所有用户。
- maintenance，server为所有的host保存记录，数据库会很庞大并且频繁更新。

#### 分布式层次数据库

- 3种server：根DNS server、 顶级域（Top-Level Domain，TLD）DNS server，权威DNS server。
- 查询 www.abc.com 的IP地址：
  - 迭代查询：
    - user（通过local DNS server）与root server之一联系，返回顶级域名com的TLD server的IP地址。
    - 与TLD服务器之一联系，返回abc.com的authoritative server。
    - 与authoritative server之一联系，得到IP地址。
  - 递归查询：
    - user（通过local DNS server）与 root server 之一联系；
    - root server 知道【顶级域名com】的 TLD server 的IP地址，亲自去访问 TLD server。
    - TLD server 得知了【root希望知道 www.abc.com 的IP地址】，它也知道【知道这个域名对应的IP地址】的authoritative server是谁，亲自去访问 authoritative server，得到IP地址。
    - TLD server 响应 root。
    - root响应 local server，local server 响应user。
    - 整个 hierarchy 的 upper level server 的负载会更大，因为要互相通信。
- 根服务器：13个根节点，标号A到M，主要部署在美国。每台“root server”是一个冗余服务器网络，不是单个服务器。
- 顶级域名服务器：com、org、net、edu、gov；uk、fr、ca、cn。Educause 公司维护 edu。
- 权威服务器：组织机构维护。
- 本地DNS服务器，local DNS server：
  - 不属于DNS层次架构。
  - 每一个ISP（residential ISP、company、university）都有一个，也被叫做 default name server。
  - user 发送 DNS 请求时，查询被发到 local server。local server 就像代理一样，如果本地cache没有查询内容，则访问根服务器、TLD server、authoritative server…查询。

#### DNS缓存

当任一 DNS server 接收到一个DNS回答，它能把该回答的所有信息都caching下来。在一段时间后（TTL，通常两天）丢弃cached information。

TLD servers 被存在本地 DNS server 中，通常不会访问 root server 来查询 TLD server 的 IP 地址。

#### DNS记录

4元组：(name, value, type, TTL)。

- TTL：生存时间，决定 record 应当从 cache 中 remove 的时间。
- type=A：name 是 hostname，value 是 IP address；
- type=NS：name 是 domain (e.g., foo.com)，value 是 domain 的 authoritative server 的 hostname；
- type=CNAME：name 是某台主机的别名，value 是 规范主机名。
- type=MX：value 是别名为 name 的 mailserver 的 规范主机名。一个公司的邮件服务器和其他服务器可以使用相同别名，一个通过CNAME查规范主机名，一个通过MX查。

#### DNS报文

请求/响应报文的format是一样的。

- header：12字节。
  - identification: 2字节，16 bit number，用于标识该查询，匹配发送的请求和接收到的回答，因此请求/响应的identification相同。
  - flags：2字节，QR 1 + opcode 4 + AA 1 + TC 1 + RD 1 + RA 1 + (zero) 3 +  rcode 4。
    - QR(1比特)：查询/响应的标志位，1为响应，0为查询。
    - opcode(4比特)：定义查询或响应的类型。0是标准的，1是反向的，2是服务器状态请求。
    - AA(1比特)：授权回答的标志位。该位在响应报文中有效 ，1表示 name server 是 authoritative server。
    - TC(1比特)：截断标志位。1表示响应已超过512字节并已被截断（这个截断和UDP有关）。
    - RD(1比特)：1表示客户端希望得到递归回答。
    - RA(1比特)：只能在响应报文中置为1，表示可以得到递归响应。
    -  zero(3比特)：不说也知道都是0了，保留字段。
    - rcode(4比特)：返回码，表示响应的差错状态，通常为0和3。
  - \# questions，# answer RRs，# authority RRs，# additional RRs，各2字节。接下来4类数据区域的数量。
- questions：正在进行的查询信息，名字（要知道IP地址的hostname）+类型（hostname是服务器A 还是 邮件服务器MX）。
- answers：多条RR，一个hostname可以有多个IP地址（冗余服务器）。
- authority：其他authoritative server的记录。
- additional：其他有帮助的记录，不是很懂。

#### 在DNS中插入记录

注册登记机构 registrar：商业实体，验证域名的唯一性，加入数据库，收一点点费用）。

需要提供：基本primary  & 辅助secondary 的 authoritative server 的 hostname & IP地址。对每一个authoritative server，新建一个NS一个A记录。

```
(networkutopia.com, dns1.networkutopia.com, NS)
(dns1.networkutopia.com, 212.212.212.1, A)
```

### DNS使用TCP的场景

区域传输：辅域名服务器会定时（一般3小时）向主域名服务器进行查询，以便了解数据是否有变动。如有变动，会执行一次区域传送，进行数据同步。区域传送使用TCP而非UDP，因为数据同步传送的数据量比一个请求应答的数据量要多得多。



到此为止，所有的协议都是基于CS的。



## P2P

适合P2P架构的应用：文件分发，大型对等方社区中的数据库（分布式散列表，好像没讲）。

### 文件分发

CS：单一 server 向每个 host 分发一个大文件。

P2P：每个对等方能够重新分发【它所有的】该文件的任何部分。

分发时间 distribution time 的比较：

- $D_{CS} \ge \max\{NF/u_s, F/d_{min}\}$，u是server上载速率，d是client最小下载速率。随client的数目线性增加。
- $D_{P2P}\ge \max\{F/u_s,F/d_{min},NF/(u_s+\sum_{i=1}^N u_i)\}$。
  - 第一项：最初的文件在服务器那里，至少把文件的每个bit发一次吧。
  - 第二项：最菜的peer至少把文件的每个bit收一次吧。
  - 第三项：系统整体upload能力=server+每个peer，即分母。NF个bit都得被upload一次吧。
- P2P效率更高。

### 流行的P2P协议：BitTorrent

（了解即可，就是最多考判断题的意思。）

- 所有peer的集合：torrent。
- tracker：记录提供服务的节点。
- peer：获得第一个chunk后，向tracker注册自己，之后周期性地通知tracker我仍然在torrent里。
- tracker：一个peer加入时，随便给它发一个torrent的子集，peer试图与子集列表上的人创建TCP连接，周期性询问它们拥有的chunk列表，对peer还没有的chunk发出请求。
- 应当从哪里要我没有的chunk，先要哪一个chunk，又应当答应给谁发chunk？
  - rarest first：首先请求最稀缺的chunk，一通分发后该chunk就不再稀缺了。
  - 对换算法 tit-for-tat：确定【以最高速率给我发chunk】的4个邻居（疏通 unchoked），给它们发数据。

## CDN，内容分发网络（了解即可）

video 的逐帧 digital image 的 coding（编码方式）：空间上，可能一大块像素是同一个颜色，只记录颜色+区域；时间上，这一帧相比前一帧变化不大，只传输变化。

MPEG4：< 1 Mbps时也可以工作的很好。

流媒体协议：DASH，Dynamic Adaptive Streaming over HTTP（了解即可），为了视频流的分发，智能的客户端（带宽不够用时的优化，带宽够用就没这么多事了）。把video file分为多个块。

如何实现 content distribution networks：

- 超级大的服务器，mega-server。单点失效，congestion，长距离传输（中国到美国）distant。
- CDN：把mega-server的功能分散到全世界各处的小服务器。
  - 找到离我最近的服务器。
  - OTT，over the top，在网络运营商之上的。

## socket编程

- 一个 socket 只能 bind 一个端口，端口与 socket 应该是一一对应的吧。
- 一个进程可以有多个端口号。比如一个 client，可以声明多个 socket 描述符，然后全部向同一个或者不同的 server 发起连接。这个时候，connection 会对每一个 socket 绑定一个端口，那么这个 client 进程便会有多个端口。
- 一个端口可以被多个进程共享。比如一个 server 去 fork 一个子进程，这个时候，父进程和子进程都共享自己 bind 的那个端口，他们也共享同一个TCP连接。这个时候，从 client 那边传过来的信息，一会儿会给子进程，一会儿会给父进程。
- 一个 (源IP地址, 源端口号, 目的IP地址, 目的端口号) 四元组，可以唯一确定一个交付的进程。

### 应用进程通信方式

应用进程：为解决具体应用问题而彼此通信的进程。

#### CS

- 客户（client）和服务器（server）是指通信中所涉及的2个应用进程；
- CS方式描述的是应用进程之间 服务 和 被服务 的关系；
- client是服务请求方（主动请求服务，被服务）。
  - 在进行通信时临时成为client，它也可在本地进行其它的计算；
  - 用户计算机上运行，在打算通信时主动向远地服务器发起通信；
  - 需要知道 server 进程所在 host 的IP地址才能发出服务请求；
  - 一个 client 进程可以与多个server进行通信。
- server是服务提供方（被动接受服务请求，提供服务）。
  - 可同时处理多个远程或本地客户的请求；（本地不需要调协议栈，直接 shared memory / message passing）
  - 必须始终处于运行状态；通常当系统启动时，就自动调用并一直运行着。某些服务器程序也可以由用户或其它进程在通信前启动；
  - 通信开始之前，服务器进程不需要知道客户进程所在主机的IP地址，被动等待并接受来自多个客户的通信请求即可；
- 可以面向连接，也可以无连接（UDP是无连接的，一次握手都不需要）。

#### BS

- 可以看做C/S方式的特例，client改为browser；
- B/S方式采取浏览器请求、服务器响应的工作模式；
- 在B/S方式下，用户界面完全通过Web浏览器实现，一部分事务逻辑在前端实现，主要的事务逻辑在服务器端实现。
- 3层架构：
  - 数据层：由 database server 承担数据处理逻辑。其任务是，接受 web server 提出的数据操作请求，database server 进行数据处理，并把处理结果返回给 web server；也可以把 database server 和 web server 部署在同一台设备上。
  - 处理层：由 web server 承担业务处理逻辑和页面存储管理，接受 client browser 的任务请求，执行相应的事务处理；
  - 表现层：browser 仅承担网页信息的浏览功能, 以超文本格式实现信息的输入和浏览。
- 特点：
  - 界面统一，使用简单。user只需要安装浏览器软件，不用安装客户端软件；
  - 易于维护。系统升级时，只需更新 server端的软件，减轻了系统维护和升级的成本；
  - 可扩展性好。采用【标准的TCP/IP和HTTP协议】（==选择题==），具有良好的扩展性；
  - 信息共享度高。HTML是数据格式的一个开放标准，目前大多数流行的软件均支持HTML；
  - 需要注意的是，在一种 browser 的环境下开发的界面，在另一种 browser 下可能有不完全适配的情况，这时需要安装对应的 browser。

#### P2P

- 指两个process在通信时，并不区分服务的请求方和提供方。
- 从本质上看仍是C/S方式。但是，强调的是通信过程中的对等，每一个P2P进程既是client也是server。
- 落实到具体的一次通信，仍存在client方和server方。
- 音频/视频应用推动了P2P的发展，音频/视频流量已占主要比例。

### server process 工作方式

- 循环方式（iterative mode）：
  - 一次只运行一个服务进程；
  - 当有多个客户进程请求服务时，服务进程就按请求的先后顺序依次做出响应（阻塞方式，或许是等一个请求处理完再处理下一个）。
- 并发方式（concurrent mode）：
  - 可以同时运行多个服务进程；
  - 每一个服务进程都对某个特定的客户进程做出响应（非阻塞方式）。
- 无连接【循环方式】服务：
  - 使用无连接的 UDP 服务进程，通常都工作在循环方式；即，一个服务进程在同一时间，只能向一个客户进程提供服务（顺序服务）；
    - 收到客户进程请求，就发送UDP数据报去响应；
    - 其他发来的请求，被塞到服务端的队列中，排队；
    - 处理完一个请求后，就从队列中读取下一个请求，继续处理。
  - 服务进程只使用一个 server socket，每个客户则使用自己的 client socket。
- 面向连接的【并发方式】服务：（多进程）
  - 面向连接的 TCP 服务进程，通常都工作在并发方式，服务进程可以 在同一时间 同时向多个客户进程提供服务（并发服务）；
  - 在TCP服务进程与多个客户进程之间，必须建立多条TCP连接；每条TCP连接，都在数据传送完毕后释放；
  - 一个TCP连接对应一个（熟知）服务端口；
    - 主服务进程（父服务进程）在【某个熟知端口】等待客户进程发出的请求。
    - 一旦收到请求，就创建一个从属服务进程（子服务进程），并指明从属服务进程使用【临时socket】与该客户建立TCP连接；所有【临时socket】都使用 server 的熟知端口号。
    - 然后，主服务进程继续守在原来的熟知端口，等待向其他客户提供服务。
  - 就是说，访问原来的socket，老socket根据某些标识将client引导到新socket，听说这是os负责的。

### socket编程

TCP时，server 比 client 先运行。

不知道UDP是否如此。好像并不是这样。

09019316 范之闽 2021/11/3 12:53:39
严格来说程序运行的顺序不重要，重要的是client发送的报文到达传输层时，对应的端口是否已经被server监听。如果没有，这个报文不会被server收到，即使server在很短的时间之后开始监听。换言之，就是传输层不为这种事情提供buffer。















