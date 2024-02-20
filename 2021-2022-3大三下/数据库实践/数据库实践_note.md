## 20220508

oracle server：

- 两部分组成：instance（例程） + database（文件的集合）。
- instance：
  - access database 的 路径，
  - memory + process。
  - 对一个database，有且只有一个 open instance。
  - memory：
    - SGA： system global area 系统全局区，共享内存，每个会话都共享。
    - PGA： program global area 程序全局区，
      - 共享池：代码（db 运行优化生成的中间代码，库缓存）+ 系统数据（数据字典，就是 metadata 的意思）
      - 数据库高速缓存：用户数据。
      - 重做日志缓存。

事务 ACID：

- consistency：一致性。`summit;` independence：事务运行过程不可观测，用 lock。
- backup 备份，restore 还原备份，recover 恢复。
- 数据文件（备份 baseline）+日志文件（log 记录 delta）。
- 操作：
  - 闪回：ctrl Z 操作，比如刚刚 drop 了 表 emp，`flashback table emp to before drop;` 
  - flashback 解决不了 物理错误（比如磁盘坏了）。
  - 回滚：对冲操作（insert 对于 delete）`rollback`。
  - 闪回 和 回滚的区别：一个是修手机，一个是给我一个新手机（insert 新数据）。

## 20220516

1. 数据字典视图。
   - 内容来自具体数据文件。
   - user_tables 开头的，属于自己的对象。当前用户所有。
   - all_tables 当前用户有权限的。
   - dba_tables dba 有权限的。`table does not exist` 
   - 统计数据（求和 max min）的滞后（如果从静态表查 如 `numrows`），要及时更新。
2. 动态性能视图。
   - 虚表，不是存储结构。内容来自 内存、控制文件。
   - 访问视图时，执行 构成视图的语句。
   - V$

用户进程 将请求 提交给 服务进程。

- 专用服务器进程：爸爸妈妈做饭。200 以下。
- 共享服务器进程：名称解析 找到饭店地址，调度进程 服务员，点菜 请求，响应队列。200 以上。10000+ 要配中间电脑，连接池。

后台进程：

- PMON 监视用户进程，处理异常终止，释放资源等。
- SMON 监视系统进程。
- DBWR 数据库书写器，从 cache 写 db。
- LGWR 日志书写器，写 redo log files。
- CKPT：检查点进程。IOBJ 事件（没听清不 sure）
  - scn system change number 系统改变号，随时间单调递增序列。
  - 和 instance 有关操作，用 `alter system`；和数据库静态存储文件相关，用 `alter database`。
  - 检查点左右：同步所有数据 + 控制文件。
- 其他：`desc v$bgprocess` 查后台进程。

oracle 外部文件，比如 parameter file password file，等于操作系统文件。

oracle 重做日志体系：

- server p 把 log 塞在 redo log buffer cache（commit），LGWR 把 log 从 redo log buffer cache 写到 redo log files。
- 以 组 方式组织。
- 数据库日志模式：
  - 非归档模式。图书馆空间不够（组不够了），旧书扔掉。
  - 归档模式。旧书重新归类，归档日志 archived log files 外部文件 用户/os 管理。

SQL：

- 查询语句 QL（select）
- DML用户数据增删改（insert，update，delete，merge）
- DDL 系统数据增删改（create，alter，drop，truncate，raname，cooent）
- DCL。
- TCL 数据控制语句（commit，rollback，savepoint）。

`select 3+4 from dual;` dual 表示查一些和表没关系的东西。oracle 一定要 from，所以不得不 dual。

null 的 三值现象，既不是 true 又不是 false。不能查询 xxx=null。

- `fetch first 2 rows only with ties;` fetch只取前两行，with ties 和你一样的我也要知道。
- `offset 3 rows;` 前三个舍掉。 这是 Oracle。 mysql 用 limit。

利用提交变量 和用户交互：

- define undefine & && 
- `define gh=7369` 
- mysql 不用 define 用 set。

函数：

- 字符串：substr，concat，length，instr 定位字符用的 左边右边开始 第几次出现。

组织结构 缩进 图：

```sql
select  lpad(' ', (level-1)*2)||ename ename from emp
start with empno=7839
connect by prior empno=mgr;
```

## 20220518

查询hr.employees表，列出2份疗养名单，显示last_name,years,months。马尔代夫（5个名额），云南（20个名额）。

```sql
conn hr/hr
use hr;

select last_name, 
floor(months_between(sysdate, hire_date)/12) as year,
round(month_between(sysdate, hire_date), 1) as month
from hr.employees order by hire_date fetch first 5 fows only;

select last_name, ceil(months_between(SYSDATE, HIRE_DATE)/12) as years,
mod(ceil(months_between(SYSDATE, HIRE_DATE), 12) as months from employees
order by years*12 + months desc
fetch next 5 rows only;
    
select last_name, trunc(months_between(sysdate, hire_date)/12) as years,
mod(trunc(months_between(sysdate, hire_date)), 12) as months from employees
order by hire_date offset 5 rows fetch next 20 rows only;
    
select last_name, trunc(months_between(sysdate, hire_date)/12) years,
trunc(mod(months_between(sysdate, hire_date), 12)) months from employees
order by 2 desc, 3 desc
fetch first 5 rows with ties;
```

中学生考试成绩：

```sql
select name,
sum(case subject when 'chinese' then score else 0 end) chinese,
sum(case subject when 'math' then score else 0 end) math,
sum(case subject when 'english' then score else 0 end) english
from scott.st group by name;

select name,
sum(decode(subject, 'chinese', score, null)) as CHINESE,
sum(decode(subject, 'math', score, null)) as MATH,
sum(decode(subject, 'english', score, null)) as ENGLISH
from scott.st group by name order by name;

# decode(condition,key1,value1,key2,value2,key3,value3,...,default)
```

table1 join table2 on condition;

```sql
use hr;
select e.last_name, e.department_id, e.salary, r.salavg
from employees as e join
(select department_id, avg(salary) as salavg
from employees group by department_id) as r
on e.department_id = r.department_id and e.salary > r.salavg;
```

## 20220524

```sql
create table emp_hz as select empno, ename, sal from emp where deptno=30;
create table emp_gz as select * from emp_hz where 1=2;
insert into emp_gz values(1111, 'Tom', '3000');
insert into emp_gz values(7900, 'JAMES', '2900');
commit;

merge into emp_hz hz using emp_gz gz
on (hz.empno=gz.empno) when matched
then update set hz.sal=gz.sal when not matched
then insert values(gz. empno,gz.ename,gz.sal);
```

TCL（commit rollback）显式事务结束。或者 DDL DCL 语句。

事务没结束时，我们这边看的是最新的，但别人（比如一个新 mobaxterm session）看的是原来的。

权限：grant，收回：revoke。

一个事务可以包含多条语句，要成功全成功，要失败全失败。即使中间存了多个 checkpoint，也会直接 rollback 到事务之前。除非显式 `rollback to savepoint xxx;` 

锁：

- 从 数据字典 查 会话信息
- alter system kill 掉 session。

DDL：数据定义语句。

- create，alter，drop，truncate，rename，comment。
- 增加字段，修改字段，删除字段。

char 完全分配这些字符的空间 提高性能，varchar 最多分配这么多 事实上可能少一些 节省空间。

例程管理：

- 关闭 4 个模式：
  - 正常关闭
  - 事务性关闭
  - 立即关闭
  - 终止退出。





## 20220614 期末实验

选择题，写语句，简答题问答题，大题。备份恢复，日志挖掘器 logminer 查看历史操作。

2 题 1 h。

RMAN 备份。

机试实验 1：rman 全库备份。

- 准备测试数据（创建新表）
- 检查数据库情况：备份 spfile，归档
- 修改 RMAN 配置：
  - 先 `show all`，
  - 一个和 spfile 有关的操作（好像是自动备份）
  - 指定备份目录，自动备份目录？放 spfile 和 备份文件。
- 全库备份：
  - delete backup。list backup。
  - `backup database plus archivelog;` 控制文件，数据文件，归档的
- 增加新数据：
  - 进行 insert。别忘了 commit。
- 复制联机日志文件（因为数据在这个里面）：
  - 先建了一个 log 目录。
  - 进行一个 cp 操作。`cp 原path ./` 
- 摧毁数据库：
  - vim /ect/oratab，先改一下，这样才能看到 sales 库。
  - 在 外面 dbca，打开一个窗口
  - 第一步选择 删数据库，第二步删 sales（我备份的数据库），然后一直yes。
- 启动例程（因为 RMAN 启动才能做）。
  - `rman target /` 
  - `startup nomount ` 
  - 还原 spfile：`restore spfile from '前面那个 spfile 的路径'` 
- 因为删数据库 目录都丢掉了，所以操作和建库差不多，要建目录。
  - cat 
  - mkdir -p adump 和 oradata/sales，fast_recovery。
- 还原控制文件 `restore controlfile from` 
- 加载数据库。`alter database mount`.
- `resotore database;` 
- 把联机日志文件复制回去。
  - select member from v$logfile；出来一堆原来的地方。
  - 在原先复制的路径，进行一个反向 cp。
- 恢复数据库 recover database；
- 打开数据库
  - alter database open;
  - alter database open resetlogs;
- 检查数据
  - selsect * from 。



课程回顾：书面考试 1.5 h。

- 环境搭建：知道装环境的过程。
- oracle 架构：instance database。问答题 * 1，详细描述 oracle 架构，就是某一张图。
  - 分两个部分，
  - instance 数据处理：instance 启动的初始化参数文件里面的 control file，mount 需要 控制文件。
    - 共享内存：SGA 共享池，缓存，数据恢复（存储日志）
    - 进程的内存：PGA
    - 用户进程（UI）→ 服务进程，各种黄色圆圈。
  - database 数据存储。文件用来 存储数据 控制文件 恢复。（日志组织方式：组，归档 非归档 模式）
- sql 语句：
  - select 连接 子查询，马尔代夫 云南 疗养 前xx个人（这种）
  - merge 工作表并到总表。
  - 工资高于平均工资。
  - 看群里大家写的语句。
- 管理（选择题，简答题）：
  - 例程管理，初始化参数，怎么去改。
  - 控制文件，目录复用 多个控制文件，出问题怎么重新创建。
    - 关闭的 4 个模式，启动的 3 个阶段。
  - 日志文件：怎么加成员 改名字 删掉 出问题怎么处理。
  - 数据移动：导入导出dsp mp。
  - 备份，闪回。







![image-20220614170236266](.\..\..\typora-user-images\image-20220614170236266.png)





得分点：

全局备份，加新数据 破坏 恢复。

挖掘：要看 timestamp。









