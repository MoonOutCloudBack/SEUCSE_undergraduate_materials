# 13 I/O system





20210611 67

讲完io我们就来复习！



只讲最重要的部分。

ordinary device：

hardware interface+internal structure。

3个reg：

- status：别人读它就可以知道设备现在处于什么状态。
- command：告诉device做什么。
- data：传递数据。



polling：inefficient但是好实现。

一个一个问，轮询。

interrupt-driven：

让calling process sleep然后context switch去别的task。

而不是polling the device repeatedly。



I/O interrupt是什么？hardware signal，from device to os。



对快的设备，polling或许不错。如果慢，interrupt比较合适。

hybrid：先polling一会，然后就interrupt。

可能会造成livelock。解决：coalescing：合并很多interrupt到一个interrupt。



interrupt-driven：中断太多，可能导致CPU很烦。

第三种技术DMA：

与device交流：I/O指令。Memory mapped I/O：如果已经MEM mapped，那么设备把自己置成available。



Application I/O Interface：

见ppt页10/15，一些概念性的东西，可能会考判断？

Clocks and Timers。Nonblocking and Asynchronous I/O。好像也是概念性的。



subsystem，子系统：如果很复杂。可以选择是否load，不应该在kernel里。

spooling：假脱机：操作系统截取所有输出到一个设备（例如打印机，它不能接受交错的数据流），并且每个应用程序的输出被假脱机（缓冲）到一个单独的磁盘文件。

Device reservation，设备预留：用于在并发应用程序和专用设备之间进行协调的显式设施。

















