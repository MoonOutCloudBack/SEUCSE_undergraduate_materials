# db 01 introduction

## basic concepts

### database

database：a very large, integrated collection of data，是数据的集合，对real-world enterprise（现实世界的应用）进行建模。两方面：entities，relationships。

database的好处：

在主存与secondary storage之间管理数据的方式，方便快捷，数据的完整性安全性，管理权限，concurrency（同时取钱取成负数）。

database与file的比较：

- Application must stage large datasets between main memory and secondary storage (e.g., buffering, page-oriented access, 32-bit addressing, etc.) 
- Special code for different queries
- Must protect data from inconsistency due to multiple concurrent users
- database提供crash recovery功能。
- database有更好的security，提供access control功能。

### DBMS

DBMS：a software package designed to store and manage databases，管理数据，听说在os和user之间，是系统软件。

DBMS的好处：data independence, efficient access, reduced application development time, integrity & security, concurrency。

database system：包括DBMS、application、data本身、DBMS的开发工具、DBA、用户，是一整套数据库的集合。

一些玄学：

- 文件系统中的文件面向应用，一个文件对应一个application，文件之间不存在联系，冗余大，共享性差，独立性差；数据库系统中的文件面向整个应用系统，文件之间相互联系，冗余小，共享性好，独立性好。
- 数据独立性：数据和程序的依赖程度。物理独立性：应用程序对数据存储结构的依赖程度。逻辑独立性：应用程序对数据全局逻辑结构的依赖程度。

### data model & schema

data model：a collection of concepts and definitions for describing data。

schema：a description of a particular collection of data, using a given data model，具体数据模型的实例。

relational model of data：目前使用最广泛的data model。relation就是a table with rows and columns。每一个relation都有一个schema。

### ANSI-SPARC

设计思想理念：

- external schema/view：how user see the data，我们如何展示数据给用户看（和用户权限有关）；最接近用户的应用层。
- conceptual schema：logical structure，数据结构；
- physical schema：file & index；如何在磁盘具体存储，如何建立索引（建在学生id还是课程id）；

![image-20210806113122904](.\..\..\typora-user-images\image-20210806113122904.png)

![image-20210806112608685](.\..\..\typora-user-images\image-20210806112608685.png)

data independence：DBMS最大的好处之一。

- conceptual schema独立于数据组织和存储的方式（physical schema）。
- 感觉上是在说：约定好层间的接口，然后互不影响。
- 数据物理独立性，数据逻辑独立性。

## history & classification

根据data model的发展：人工管理→文件系统→数据库系统。

- no management，before 1960：没有data model；
- file system：只能进行simple data management；
- DBMS：
  - 1964，第一个DBMS（美国）：IDS，network；network data model。
  - 1969，第一个商用DBMS（IBM）：hierarchical data model。
  - 1970，E. F. Codd(IBM)，relation data model。

根据DBMS architectures的发展：

- centralized database systems；集中式。
- parallel database systems；并行化。single computer，multi CPU，parallel computing。
- distributed database systems；分布式。multi computer。
- mobile database systems；移动。mobile phones，移动智能机。

根据architectures of application systems based on databases的发展：

- centralized structure：host+terminal。
- distributed structure。
- client / server structure；客户端装软件。
- three tier / multi-tier structure；在CS架构里加了一个新的层，以网页的方式（互联网）把操作传给web server，然后web server连到application server，最后连到database server。简化了客户端。
- mobile computing。
- grid & cloud computing。

根据expanding of application fields，应用场景：

- on-line transaction prcessing（OLTP），在线交易。
- engineering database；
- multimedia database；
- temporal & spatial database；
- data warehouse，on-line analytical processing（OLAP），data mining。
- deductive database, knowledge management。

## database system的life cycle

database system：allpications+DMBS+database+DBA（数据管理员）。

DBMS是database system的核心。

- database system planning；
- database designing；
- database establishing and loading；
- database running, manageing and maintaining；
- database extending and restructuring。

