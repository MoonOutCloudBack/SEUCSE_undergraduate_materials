# 计算机网络 第二章 实验

<p align='right'>09019106 牟倪</p>

## 一、测试 tracert 命令，并解析其过程

Tracert 命令（trace router的缩写）是路由跟踪实用程序，用于确定 IP 数据包访问目标所采取的路径。命令用 IP 生存时间 (TTL) 字段和 ICMP 错误消息来确定从一个主机到网络上其他主机的路由。

### 实验过程与结果

打开 cmd，输入`tracert www.baidu.com`，执行结果如下图所示：

![image-20211016164837153](.\..\..\..\typora-user-images\image-20211016164837153.png)

- 最左侧一列：数据包经过路由节点的标号；
- 中间三列：连接到每个路由节点的时间、返回时间、多次连接反馈的平均值。时间越小，连接速度越快。
- 最后一列：路由节点的 IP 地址。

第7个路由节点的结果是“*”，表示 ICMP 包返回时间超时，无法联通。可能的原因有很多种，如“该路由节点禁止ping”等。

### tracert 的执行过程与原理

tracert 利用 IP 生存时间字段（TTL）来确定数据包的路径：

- 首先，tracert 向目的地送出一个 TTL 为1的数据包。当路径上的第一个路由节点收到这个数据包时，它将TTL减1，此时 TTL 变成0，所以该路由器会将此数据包丢掉，并送回一个“ICMP time exceeded”消息（包括发 IP 包的源地址、IP 包的所有内容、路由器的 IP 地址）。tracert 收到这个消息后，便得到了路由节点的地址。
- 接着，tracert 再送出另一个 TTL 是2的数据包，发现第2个路由器，再送出TTL为3的数据包……
- tracert 每次将TTL加1来发现下一个路由器，直到数据包到达目的地为止。由于tracert通过UDP数据包向不常见端口（30000以上）发送数据包，因此数据包到达目的地后，会收到“ICMP port unreachable”消息，可以根据这个消息判断是否到达目的地。

## 二、测试 curl 命令，访问一个 web 页面

curl（Command-line Uniform Resource Locator，命令行统一资源定位器）是使用命令行访问网页URL的工具 。Linux系统自带curl工具。

### 实验过程与结果

打开linux虚拟机，打开命令行，输入`curl https://www.baidu.com`，执行结果如下图所示：

![image-20211016171553928](.\..\..\..\typora-user-images\image-20211016171553928.png)

curl 命令得到了网页的源代码。

## 三、利用 telnet 测试 GET 命令，访问 www.baidu.com

Telnet 协议是 TCP/IP 协议家族中的一员，是 Internet 远程登陆服务的标准协议和主要方式，为用户提供了在本地计算机上完成远程主机工作的能力。Telnet 是常用的远程控制Web服务器的方法。

在我的电脑上用 telnet 连接到服务器，在 telnet 程序中输入命令，这些命令就会在服务器上运行，就像直接在服务器的控制台上输入一样。换一个视角，一旦入侵者与远程主机建立了 Telnet 连接，入侵者便可以使用目标主机上的软、硬件资源，而入侵者的本地机相当于一个只有键盘和显示器的终端。

### 实验过程与结果

打开cmd，输入`telnet www.baidu.com 80`。回车后，屏幕为全黑。此时我们用快捷键`Ctrl+]`打开本地回显功能，这样就可以看见我们所键入的内容了。

![image-20211016175007529](.\..\..\..\typora-user-images\image-20211016175007529.png)

单击回车，进行编辑状态。输入`GET / HTTP/1.1`后回车，接着输入`HOST:`然后按回车，一个简单的HTTP请求就完成了；只要再次按下回车，便可以向服务器递交这个请求了。

`GET`表示请求方式，`/`表示请求的根目录下的文件，`HTTP/1.1`表示HTTP协议版本，`HOST:`是一个消息头。`GET HTTP`必须大写，否则请求无法发送。

服务器的返回内容如图所示：

![image-20211016174727356](.\..\..\..\typora-user-images\image-20211016174727356.png)

## 四、利用 telnet 命令测试 SMTP 服务

SMTP，simple mail transfer protocol，简单邮件传输协议，定义了将邮件在 mail server 之间推送的服务。

我们利用telnet，远程登陆QQ邮箱的SMTP服务器，测试SMTP服务。

### 实验过程与结果

- 打开cmd，输入 `telnet smtp.qq.com 587`；
- 输入 `helo qq.com`，向服务器表明身份；
- 输入 `auth login`；
- 输入邮箱的 base64 编码（3469959471@qq.com的base64编码）；
- 输入开启 IMAP/SMPT 时授权码的 base64 编码（无空格）；
- 输入发送人邮箱 `mail from:<xxxxxxxxxx@qq.com>`；
- 输入收信人邮箱 `rcpt to:<xxxxxxxxxx@qq.com>`；
- 输入 data+回车，成功后就可以进行邮件正文编辑；
- 邮件输入格式：
  - Subject: 主题文本
  - （空一行）
  - 邮件内容
  - . （句点单独一行，表示邮件结束）
- 输入`quit`退出。

实验结果：

![](.\..\..\..\typora-user-images\QQ图片20211102220413.png)

对方收到的邮件：

![image-20211102220629828](.\..\..\..\typora-user-images\image-20211102220629828.png)

## 五、使用 nslookup 查询域名信息

### 实验过程与结果

打开cmd，输入`nslookup baidu.com`。

![image-20211217190819942](.\..\..\..\typora-user-images\image-20211217190819942.png)

成功查询到了域名对应到的IP地址。

发现两次应答中，IP地址顺序不同，或许与负载均衡算法中“将一串IP地址以循环队列的顺序发送”有关。













