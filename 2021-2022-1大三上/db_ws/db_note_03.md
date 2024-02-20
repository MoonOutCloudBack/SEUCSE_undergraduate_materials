# db 03 user interface & SQL language

DBMS的用户界面（user interface）需要提供：

- query languages，data的retrieval和manipulation；
- GUI，与用户交互；
- APIS；
- class library，听起来像是模板；

query language：allow manipulation and retrieval of data from a database，支持在数据库里检索+操纵数据。

query language的4个类别：

- formal query language，正式；
- tabular query language，表格；
- graphic query language，图形；
- limited natural language query language，自然语言；

query language不是programming language：不是图灵完备（turing complete）的；

## SQL概述

SQL：structured query language → standard query language。

SQL的4个部分：

- data definition language (DDL)，define/alter/delete data schema，操纵schema。
- query language (QL)，retrieve data，查询数据。【重点】
- data manipulation language (DML)，insert/update/delete data，操纵数据。【重点】
- data control language (DCL)，control user's access authority to data，控制用户访问数据的权限。

### important terms & concepts

base table：基表，真实存在的表。

view：视图，how users see the data。

data type supported：我们支持的数据类型。

## basic SQL query

```sql
SELECT [DISTINCT] target_list FROM relation_list WHERE qualification;
```

- target_list：我们想要的attributes的lists。
- DISTINCT：去掉重复的值。
- relation_list：我们想查的表，【可以写多个表】，用逗号隔开。
- qualification：查询条件，用AND OR NOT连接。

例子：

```sql
SELECT S.sname 
FROM Sailors S, Reserves R 
WHERE S.sid=R.sid AND R.bid=103;
```

用“.”的形式索引attribute，涉及多张表时最好写清楚，虽然只要不出现重复attribute就可以跑。

### 实现query：conceptual evaluation strategy

电脑如何处理 query。

- 计算所有 relation_list 的 cross-product。
- discard 不满足 qualification 的 tuples。
- 删掉不在 target_list 里的 attributes。
- 如果 DISTINCT，eliminated duplicate rows。

一般性：每一个query都可以这样做。完备性：先把所有表cross-product起来，答案肯定在这个里面，不会漏掉。

但这样的遍历大概是最低效的方法了。我们需要optimizer，后面会讲。

### expressions & strings

```sql
SELECT S.age, age1=S.age-5, 2*S.age AS age2
FROM Sailors S
WHERE S.sname LIKE ‘B_%B’;
```

在SELECT clause里，用`age1=S.age-5`、`2*S.age+3 AS age2`命名新的想要得到的attribute。

在WHERE clause里，用`LIKE`做字符串匹配操作。_代表任意一个字符，%代表任意多个任意字符（听说可以0个，但access里是≥1个）。

## medium SQL query

### INTERSECT

- 如果我们想查【借过红色船也借过绿色船的水手id】，不能直接`B.color='red' AND B.color='green'`（怎么会有船又红又绿呢）。

- 我们可以先查红色再查绿色，然后对两个结果`INTERSECT`（取交集）。

- ```sql
  SELECT S.sid
  FROM Sailors S, Boats B, Reserves R
  WHERE S.sid=R.sid AND R.bid=B.bid
  AND B.color=‘red’
  INTERSECT
  SELECT S.sid
  FROM Sailors S, Boats B, Reserves R
  WHERE S.sid=R.sid AND R.bid=B.bid
  AND B.color=‘green’;
  ```

- `UNION`是对结果取交集，与`INTERSECT`类似。

- 还有一种方案是，复制一份借船记录表和船信息表，同时查两份表，一份查红船一份查绿船。

- ```sql
  SELECT S.sid
  FROM Sailors S, Boats B1, Reserves R1,
  Boats B2, Reserves R2
  WHERE S.sid=R1.sid AND R1.bid=B1.bid
  AND S.sid=R2.sid AND R2.bid=B2.bid
  AND (B1.color=‘red’ AND
  B2.color=‘green’)
  ```

- 一个技术细节：如果要查【借红船+绿船的水手名字】，单单把【借红船水手名字】和【借绿船水手名字】取交集是不行的，因为名字不是主键，可能会有重复。

### nested queries：IN

使用`IN`实现嵌套的查询，在一个查询结果的表里继续查询。

比如说，查询【借过船#103的水手的姓名】：

```sql
SELECT S.sname
From Sailors S
WHERE S.sid IN (SELECT R.sid
               FROM Reserves R
               WHERE R.bid=103);
```

如果我们要查【没有借船#103的水手的姓名】，直接把IN改成NOT IN：

```sql
SELECT S.sname
From Sailors S
WHERE S.sid NOT IN (SELECT R.sid
                    FROM Reserves R
                    WHERE R.bid=103);
```

甚至可以在nested query里面引用外面的S表：查询【借过#103的水手名字】

```sql
SELECT S.sname
FROM Sailors S
WHERE EXISTS (SELECT *
              FROM Reserves R
              WHERE R.bid=103 AND S.sid=R.sid);
```

练习：查询【只被一个水手预定的船的id】：

```sql
SELECT bid
FROM Reserves R1
WHERE bid NOT IN (SELECT bid
                 FROM Reserves R2
                 WHERE R2.sid<>R1.sid)
```

- 嵌套的意思是，除了这个水手以外，其他水手定的船的集合。如果这个水手定的这条船不在【其他水手订船集合】里，那么这条船就只被这个水手定过。
- 就是说，嵌套那里首先对两张reserve表做了cross-product，留下`R2.sid<>R1.sid`的记录（也就是被很多个水手预定的船），project出那些船的id。只要bid不在这个嵌套查询的结果里，它就是只被一个人预定的船。

### EXISTS

EXISTS：存在这样的元组。

查询【借过#103的水手名字】：存在这样一条借船记录，使得被借的船是#103，借船水手的id也恰好是我的id，那么我就是借过#103的。

```sql
SELECT S.sname
FROM Sailors S
WHERE EXISTS (SELECT *
              FROM Reserves R
              WHERE R.bid=103 AND S.sid=R.sid);
```

查询【只借过一次船#103的水手的名字】：

```sql
SELECT S.sname
FROM Sailors S
WHERE EXISTS (SELECT *
              FROM Reserves R
              WHERE R.bid=103 AND S.sid=R.sid
                    AND NOT EXISTS(SELECT *
                                   FROM Reserves R2
                                   WHERE R2.bid=103 AND S.sid=R2.sid AND R2.day<>R.day));
```

### 其他operators

NOT (IN | EXISTS | UNIQUE)、ANY、ALL。（虽然不知道UNIQUE是什么）

比如【找我们专业的rank1】：也在cs专业，绩点 ≥ 任一cs同学的绩点。

```sql
SELECT S.sid, S.name
FROM Student S
WHERE S.major='cs' AND 
      S.grade>=ANY (SELECT S.grade
                    FROM Student S
                    WHERE S.major='cs');
```

用SQL实现division，查【订过所有船的水手名字】：

思路1：双重否定，不存在这样的船：所有的船 - 我没订过的船。

```sql
SELECT S.sname FROM Sailors S
WHERE NOT EXISTS((SELECT B.bid FROM Boats B)
                 EXCEPT
                 (SELECT R.bid FROM Reserves R
                   WHERE R.sid=S.sid);
```

EXCEPT就是一个集合减（set-difference）的操作。

不使用EXCEPT：

```sql
SELECT S.sname
FROM Sailors S
WHERE NOT EXISTS (SELECT B.bid
                  FROM Boats B
                  WHERE NOT EXISTS (SELECT R.bid
                                    FROM Reserves R
                                    WHERE R.bid=B.bid
                                    AND R.sid=S.sid));
```

- 我们要找水手的名字，对于这样的水手，不存在这样的船，使得对于这样的船，不存在一条借船记录，显示本水手借过它。
- 也就是说，对于这样的水手，不存在【没有一条借船记录来见证“我借过它”】的船。也就是说，不存在我没借过的船。

### aggregate operators

COUNT，SUM，AVG，MAX，MIN。

```sql
SELECT AVG(S.age)
FROM Sailors S
WHERE S.rating=10;
```

好奇【年龄最大的水手】的名字和年龄，`SELECT S.sname, MAX(S.age) FROM Sailors S`，这样写是illegal的（一边是列，一边是列的max值）。

以下的写法是错的，因为aggregate operation不能嵌套使用，也就是不能`MIN(AVG(S2.age))`：

```sql
SELECT S.rating
FROM Sailors S
WHERE S.age = (SELECT MIN(AVG(S2.age))
               FROM Sailors S2)
```

### grouping

想对每一个分组进行查询，比如寻找【每一个rating组】的最小年龄。但我们不知道有多少个rating，SQL也不支持for循环。

```sql
SELECT		[DISTINCT] target-list
FROM		relation-list
WHERE		qualification
GROUP BY	grouping-list
HAVING		group-qualification
```

比如，查询【每个年龄的平均rating】：

```sql
SELECT S.age, AVG(S.rating)
FROM Sailors S
GROUP BY S.age;
```

### HAVING

`HAVING`是来筛选分组的。

`SELECT`和`HAVING`的属性，需要是`GROUP BY`的子集。

对【每一个红船】，查询【它被预定过多少次】：

```sql
SELECT B.bid, COUNT(*) AS rcount
FROM Reserve R, Boats B
WHERE R.bid=B.bid AND B.color='red'
GROUP BY B.bid;
```

不能把`B.color='red'`放在`HAVING`里。因为虽然我们知道，一个bid对应一个唯一color，我们筛选color就是筛选分组，但是机器不知道啊。机器用HAVING筛选整个分组，在它看来，一个组内可能有些tuple满足、有些不满足。

可以强行这样写：

```sql
SELECT B.bid, COUNT(*) AS rcount
FROM Reserve R, Boats B
WHERE R.bid=B.bid
GROUP BY B.bid, B.color='red'
HAVING B.color='red';
```

### 多约束的复杂查询

对【每个至少有3个水手的rating】，查询【该rating下 年龄＞18 水手】的【最小年龄】：（对这3个水手年龄没有要求，因此不希望被`WHERE`刷下去，因此用一个新表作为`HAVING`条件）

```sql
SELECT S.rating, MIN(S.age)
FROM Sailors S
WHERE S.age > 18
GROUP BY S.rating
HAVING 2 < (SELECT COUNT(*)
            FROM Sailors S2
            WHERE S2.rating = S.rating);
```

高级写法：`EVERY`、`ANY`。

对【每个至少有3个年龄＞18 水手、且每个人年龄都＜60的rating】，查询【该rating下 大于18岁水手】的【最小年龄】：

```sql
SELECT S.rating, MIN(S.age)
FROM Sailors S
WHERE S.age > 18
GROUP BY S.rating
HAVING 2 < COUNT(*) AND EVERY(S.age<60);
```

### 思想：建立中间查询表

查询【平均年龄在各rating中最小的rating】：（注意平均年龄最小的rating可能不止一个）

```sql
SELECT Temp.rating
FROM (SELECT S.rating, AVG(S.age) AS avgage
      FROM Sailors S
      GROUP BY S.rating) AS Temp
WHERE Temp.avgage = (SELECT MIN(Temp.avgage)
                     FROM Temp);
```

## advanced SQL query

### NULL

三值逻辑：能，不能，现在还不知道。

### CAST expression

强制类型转换，定义精确度，把NULL转换成其他数据类型。

```sql
substr(string1, CAST(x AS Integer), CAST(y AS Integer))
CAST (elevation AS Decimal (5,0))
```

CAST应用，两张表的outer-union：

![image-20211018145611972](.\..\..\typora-user-images\image-20211018145611972.png)

上面代码中的`CREATE VIEW`是创建一个临时表，和WITH的区别：WITH只是子查询，没有建立临时schema；VIEW建立了临时schema，但结束与sql的连接之后就销毁了。

### CASE expression

详见ppt吧，不是很懂，希望不考（双手合十）。

### sub-query

SELECT和FROM里面都可以有sub-query。

三种类型：

- scalar sub-query，返回一个值。
- table expression，返回一个表。
- common table expression，被多次使用的表；

### WITH

一个temporary table可能被多次用到，可以用`WITH`来定义一下，事实上会建立一个temporary view。

比如，查询【总薪酬最高的department】：

```sql
WITH payroll (deptno, totalpay) AS
	(SELECT deptno, sum(salary)+sum(bonus)
    FROM emp
    GROUP BY deptno)
SELECT deptno
FROM payroll
WHERE totalpay=(SELECT max(totalpay)
               FROM payroll);
```

查询所有的【department pair】，其中【第一个department的平均工资】≥ 第二个的两倍。

```sql
WITH deptavg (deptno, avgsal) AS
	(SELECT deptno, avg(salary)
    FROM emp
    GROUP BY deptno)
SELECT d1.deptno, d1.avgsal, d2.deptno, d2.avgsal
FROM deptavg AS d1, deptavg AS d2
WHERE d1.avgsal>2*d2.avgsal;
```

### outer join

把 Teacher (name, rank) 和 Course (subject, enrollment, quarter, teacher)做outer-join。

```sql
WITH
	innerjoin(name, rank, subject, enrollment) AS
		(SELECT t.name, t.rank, c.subject, c.enrollment
		FROM teachers AS t, courses AS c
		WHERE t.name=c.teacher AND c.quarter=‘Fall 19’) ,
	teacher-only(name, rank) AS
		(SELECT name, rank
		FROM teachers
		EXCEPT ALL
		SELECT name, rank
		FROM innerjoin) ,
	course-only(subject, enrollment) AS
		(SELECT subject, enrollment
		FROM courses
		EXCEPT ALL
		SELECT subject, enrollment
		FROM innerjoin)
SELECT name, rank, subject, enrollment
FROM innerjoin
UNION ALL
SELECT name, rank,
	CAST (NULL AS Varchar(20)) AS subject,
	CAST (NULL AS Integer) AS enrollment
FROM teacher-only
UNION ALL
SELECT CAST (NULL AS Varchar(20)) AS name,
	CAST (NULL AS Varchar(20)) AS rank,
	subject, enrollment
FROM course-only;
```

为什么UNION 后面要加ALL？这是语言细节，和系统有关系。

### recursion

比如，查询【Hoover直接/间接领导的employee】中【薪资＞1e5】的。

```sql
WITH agents(name, salary) AS
	((SELECT name, salary
     FROM FedEmp
     WHERE manager='Hoover')
    UNION ALL
    (SELECT f.name, f.salary
    FROM agents AS a, FedEmp AS f
    WHERE f.manager=a.name))
SELECT name
FROM agents
WHERE salary>100000;
```

三部分：initial query 初始化集合、recursive query 集合自己生成自己、final query 最后筛选。

比如，查询【一个wing需要多少个rivet】。

```sql
WITH wingpart(subpart, qty) AS
	((SELECT subpart, qty
     FROM components
     WHERE part='wing')
    UNION ALL
    (SELECT c.subpart, w.qty*c.qty
    FROM wingpart w, components c
    WHERE w.subpart=c.part))
SELECT sum(qty) AS qty
FROM wingpart
WHERE subpart='rivet';
```

有环的查询，递归的终止条件：比如，查询【SFO到JFK的cost最小路径】。

```sql
WITH trips(destination, route, nsegs, totalcost) AS
	((SELECT destination, CAST(destination AS varchar(20)), 1, cost
     FROM filghts
     WHERE origin='SFO')
    UNION ALL
    (SELECT f.destination,
    		CAST(t.route||','||f,desination AS varchar(20)),
    		t.nsegs+1, t.totalcost+f.cost
    FROM trips t, flights f
    WHERE t.destination=f.origin
    	  AND f.destination<>'SFO'
    	  AND f.origin<>'JFK'
    	  AND t.nsegs<=3))
SELECT route, totalcost
FROM trips
WHERE destination='JFK' AND totalcost=
	(SELECT MIN(totalcost)
    FROM trips
    WHERE destination='JFK');
```

## 查询之外的SQL

### data manipulation language (DML)

insert & delete & update：

```sql
INSERT INTO EMPLOYEES VALUES ('Smith', 'John', '1980-06-10', 'Los Angles', 16, 45000);
DELETE FROM Preson WHERE LastName='Rasmussen';
UPDATE Person SET Address='Zhongsan 23', city='Nanjing' WHERE LastName='Wilson';
```

### view in SQL

general view：

- base table derive出来的 virtual table。
- logical data indipendence，security。
- 更新的问题：对view的更新要不要反馈到base table里。仅当一一对应才能更改，不能强行修改平均分之类的。

temporary view，rucursive query：WITH。

## Embedded SQL

把SQL嵌入到programming language（C++、java、python）里。

解决方案：

- embedded SQL：是SQL与编程语言交互的最基本原理，把 SQL statements 转换成 library function 来 access database。
- programming APIs：封装好的数据库访问。
- class library：OOP，programming APIs 封装成类。

在C里面直接跑SQL：

- 以 `EXEC SQL` 开始，以 `;` 结束。
- 通过宿主变量（host variables）与SQL交互，要用 `EXEC SQL` 定义，不能定义成结构体。用冒号来区分，用INTO来赋值。
- 特殊host variable：SQLCA（SQL communication area），`EXEC SQL INCLUDE SQLCA `，用`SQLCA.SQLCODE`来确定语句执行的状态（是否成功）。＜0是失败。
- 如果返回结果不是一个tuple，而是一系列tuple（一张表），使用cursor（游标）。用FOR定义，用OPEN打开（像文件一样），用FETCH遍历，遍历完时`SQLCA.SQLCODE`返回100，用CLOSE关闭。

```c
// 定义宿主变量
EXEC SQL BEGIN DECLARE SECTION;
char SNO[7];
char GIVENSNO[7];
char CNO[6];
char GIVENCNO[6];
float GRADE;
short GRADEI; /*indicator of GRADE*/
EXEC SQL END DECLARE SECTION;

// 连接数据库
EXEC SQL CONNECT :uid IDENTIFIED BY :pwd;
// DDL 操纵schema，DML 操纵数据
EXEC SQL INSERT INTO SC(SNO,CNO,GRADE)
    VALUES(:SNO, :CNO, :GRADE);
// SQL 查询
EXEC SQL SELECT GRADE
	INTO :GRADE :GRADEI
    FROM SC
    WHERE SNO=:GIVENSNO AND
        CNO=:GIVENCNO;
// 定义cursor
EXEC SQL DECLARE C1 CURSOR FOR
	SELECT SNO, GRADE
	FROM SC
	WHERE CNO = :GIVENCNO;
EXEC SQL OPEN C1;
if (SQLCA.SQLCODE<0) exit(1); /* ＜0，查询发生错误*/
while (1) {
	EXEC SQL FETCH C1 INTO :SNO, :GRADE :GRADEI;
	if (SQLCA.SQLCODE==100) break;
	/* treat data fetched from cursor, omitted*/
}
EXEC SQL CLOSE C1;
```

变量和交互的内容会事先编译好。但如果需要动态交互，比如动态输入username和password，该怎么办？

### dynamic SQL

#### dynamic SQL executed directly

只能执行 non query SQL statements。

```c
EXEC SQL BEGIN DECLARE SECTION;
char sqlstring[200];
EXEC SQL END DECLARE SECTION;
char cond[150];
strcpy(sqlstring, "DELETE FROM STUDENT WHERE ");
printf("Enter search condition: ");
scanf("%s", cond);
strcat(sqlstring, cond);
EXEC SQL EXECUTE IMMEDIATE :sqlstring;
// 直接执行一句话
```

#### dynamic SQL with dynamic parameters

也是只能用在 non query statements。

```c
EXEC SQL BEGIN DECLARE SECTION;
char sqlstring[200];
int birth_year;
EXEC SQL END DECLARE SECTION;
strcpy(sqlstring, "DELETE FROM STUDENT WHERE YEAR(BDATE) <= :y;");
printf("Enter birth year for delete: ");
scanf("%d", &birth_year);
EXEC SQL PREPARE purge FROM :sqlstring;
EXEC SQL EXECUTE purge USING :birth_year;
```

:y 是占位符。

出现多个占位符：USING后，依此写出所有占位符，逗号隔开；或者写很多行，每一行依次赋值一个占位符。汤雯婧说是前者，我也觉得是前者。

#### dynamic SQL for query

```c
EXEC SQL BEGIN DECLARE SECTION;
char sqlstring[200];
char SNO[7];
float GRADE;
short GRADEI;
char GIVENCNO[6];
EXEC SQL END DECLARE SECTION;
char orderby[150];
strcpy(sqlstring, "SELECT SNO, GRADE FROM SC WHERE CNO= :c");
// ORDER BY
printf("Enter the ORDER BY clause: ");
scanf("%s", orderby);
strcat(sqlstring, orderby);
// :c
printf("Enter the course number: ");
scanf("%s", GIVENCNO);
EXEC SQL PREPARE query FROM :sqlstring;
EXEC SQL DECLARE grade_cursor CURSOR FOR query;
EXEC SQL OPEN grade_cursor USING :GIVENCNO;
// traverse
if (SQLCA.SQLCODE<0) exit(1);
while (1) {
	EXEC SQL FETCH grade_cursor INTO :SNO, :GRADE :GRADEI
	if (SQLCA.SQLCODE==100) break;
	/* treat data fetched from cursor, omitted*/
}
EXEC SQL CLOSE grade_cursor;
```

#### stored procedure

就是把一串经常执行的操作，预先编译好，并存储在database里。

已经编译好了，reusable，不再需要写代码了；但是需要存储，空间换时间。

expand function of DBMS，DBMS的扩展功能，可以写脚本。

```c
EXEC SQL
	CREATE PROCEDURE drop_student
		(IN student_no CHAR(7),
		OUT message CHAR(30))
	BEGIN ATOMIC
		DELETE FROM STUDENT WHERE SNO=student_no;
	DELETE FROM SC
		WHERE SNO=student_no;
	SET message=student_no || ’droped’;
END;
EXEC SQL
...
CALL drop_student(...); // 可以call它了
```



























