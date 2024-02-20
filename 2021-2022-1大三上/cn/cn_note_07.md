# cn 07 wireless & mobile networks

## 无线网络的组成部分

基站（802.11 AP）：

一般与有线网络相连，负责区域内有线/无线设备的中继。

基础设施模式（见基站） & 自组织的ad hoc（节点覆盖特定范围内的相邻节点，进行中继；多个节点组织成网络，互相通信）。

## 3G4G5G

1G：FDMA的语音通话；2G：TDMA的数字消息。

3G：CDMA。

4G：集3G与WLAN于一体。

5G：更高的速率，更宽的带宽。频段更高，容易受建筑物影响。

Bluetooth：最初只有10m，现在5.0版本300m。

## 无线链路特征

来自其他源的干扰：标准化的无线电频率由许多设备共享 (e.g., phone)，devices (motors) 互相干扰。（CDMA：给每个用户特定且唯一的编码方案）

多路径传播：无线信号会在物体表面反射，最终在不同时间到达目的地。

隐藏终端：A和B、B和C互相听到，AC离得太远不知道彼此的存在，于是A和C一起跟B说话，B哪个也听不清。

信号衰减。

## IEEE 802.11 wireless LANs： Wi-Fi

802.11 LAN的架构：基础服务集（BSS，basic service set）称为cell。

CSMA/CA：

先听后发，边听边发。

### IEEE 802.11 MAC protocol： CSMA/CA

可以看这个视频：https://www.bilibili.com/video/BV11E411y7Ln

MAC 是 media access control。

sender：

- 如果现在信道空闲，等一个DIFS（分布式帧间间隔），然后发送entire frame。
- 如果现在信道繁忙，搞一个随机回退值，然后倒计数，计数器=0时再看信道是否ok。

receiver：

- 如果接收成功，等一个SIFS（短帧间间隔，因为隐藏终端问题），然后发一个ACK。
- 但是仍然不能解决接收方冲突的隐藏终端问题。

RTS CTS的预约版本：

- sender先基于CDMA（监听）发一个request-to-send（RTS）。
- BS（基站）觉得可，广播一个clear-to-send（CTS），表示自己被预约了。
- sender收到CTS就开始发送，其他节点收到CTS延迟发送。
- 几乎避免所有冲突。

![image-20211223174334551](.\..\..\typora-user-images\image-20211223174334551.png)

思想：

- 检测冲突：没法听信号强弱了，所以用ACK确认。
- 用时间间隔把帧分开。
- 计时器机制，应答机制。
- CD：检测到冲突就赶快回退。CA：甚至还可以预约呢。

CA只需要了解重点。



链路层最重要的东西就是多路访问协议。这是链路层帧的传输协议啊。

交换机需要自学习，这个MAC地址和哪个AP连。
