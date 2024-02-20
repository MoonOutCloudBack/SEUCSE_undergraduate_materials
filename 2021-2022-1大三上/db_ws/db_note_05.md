# db 05 security & integrity constrains

## security 

4个database destruction的原因：

- system failure & concurrent access造成的inconsistency（上一章）。
- man-caused destruction。（误操作/有意攻击）
- input data is incorrect / updating transaction didn't obey rules of consistency preservation。（输入数据不正确，运行程序不符合保持db一致性的原则，用户程序不对）

阻止accessed illegally的方法：

- 视图 view。不支持视图的db：查询修改 query rewriting，强行改写你的查询。

- access control： general user 普通用户，user with resource privilege 特权用户（可以新建表、可以控制新建表的权限），DBA 管理员。

- identification & authentication： password，key，IC card，fingerprint。

- authorization： GRANT语句。

  ```SQL
  GRANT CONNECT TO MOONOUT IDENTIFIED BY moonout;
  -- 把连接权限给moonout
  GRANT SELECT ON TABLE Student TO uran WITH GRANT OPTION;
  -- 把select权限给uran，并允许uran继续把select权限grant给别人
  ```

- 但为成百上千的user授权，太麻烦了：提出 role 角色。

- 操作系统层面的文件权限，只允许DBMS访问，user没法绕过DBMS；还是担心绕过DBMS，把磁盘的数据加密。

- audit trail 审计追踪，使用AUDIT语句，结合log信息。

  ```SQL
  AUDIT SELECT, INSERT, DELETE, UPDATE on Student WHENEVER SUCCESSFUL;
  ```

### 统计数据库的安全

- 这样说吧，我想查男性程序员wang的工资，但我只能做SELECT的统计查询，不能查询个体。
- 我count了一下男性程序员的数量，发现=1，我就断定那一个人是wang了；然后我sum一下男性程序员的工资，其实就是wang一个人的工资，就得到了。
- 为了避免这种情况，DBA要求只能对一组数量 ≥ 2 的tuple做统计查询。
- 那么，我count一下所有人的数量，发现=10，然后我count一下 不是男性 或者 不是程序员的人的数量，发现=9。然后我用所有人工资的sum - 后者的sum，就得到了wang的工资。
- DBA感到很无语，于是限制只能对一组数量 ≤ 8 的tuple做统计查询。此时，要求【b ≤ |SET(T)| ≤ (n-b), b=2】。
- 接下来，我count一下男性的数量，发现=5，count一下男性非程序员的数量，发现=4，然后 sum前者 - sum后者，又查到了。
- 事实上，只要有这样一个集合，【2b ≤ |SET(T)| ≤ (n-2b), b＜n/4】，就可以查到任意集合：
  - SET(p) = SET(p or T) ∪ SET(p or not T) - SET(T) - SET(not T)。
- 我们把满足这种性质的一个tuple集合，叫做通用追踪器，general tracker。
- 只要我足够耐心，总会找到一个通用追踪器的。然后，只要知道定位个体的条件，就可以查询个体信息了。
- 为了统计数据库的安全，DBA要检查所有的谓词组合，限制满足条件的所有组合，还不能影响数据库的正常访问。此时DBA终于意识到，统计数据库的安全是很难的问题。

## integrity constrains

用来检查替代前值BI的后值AI，如果不满足条件就报错，保证进入数据库的数据都干净正确。

### static constraints

- inherent constraint：固有的约束，不允许表中套表。

- implicit constraints：在schema里定义的。

  - domain constraint（学生绩点不是字符串）。
  - primary key constraint（主键不能重复不能为NULL）。
  - foreign key constraint（外键不能荡空，引用完整性）。具体实现：报错 / 级联删除 级联更新（修改主键时）。（外键是关联表的attribute，而非信息表的primary key）

- explicit constraint：断言，check语句。

  - 在程序里定义：不合法的事情直接报错，不会送给数据库。

  - 用ASSERT断言语句。

    ```sql
    ASSERT balanceCons ON account: balance>=0;
    ```

  - 在CREATE TABLE时使用CHECK语句。

    ```sql
    CREATE TABLE Sailors
    (sid INTEGER, sname CHAR(10), rating INTEGER, age REAL,
    PRIMARY KEY (sid),
    CHECK (rating >= 1 AND rating <= 10)
    );
    
    -- Interlake不能借
    CREATE TABLE Reserves
    (sname CHAR(10), bid INTEGER, day DATE,
    PRIMARY KEY (bid, day),
    CONSTRAINT noInterlakeRes
    CHECK (‘Interlake’ <>
    (SELECT B.bname FROM Boats B WHERE B.bid=bid)
    ));
    ```
  
  - 多张表上的constraints：创建断言。
  
    ```sql
    CREATE ASSERTION smallClub
    CHECK
    ((SELECT COUNT(S.sid) FROM Sailors S) +(SELECT COUNT(B.bid) FROM Boats B) < 100);
    ```

### dynamic constraints：trigger

与触发器trigger结合起来，状态转换中的约束规则。

trigger：

- 如果DBMS出现某些特定changes，自动开始的procedure。或者将 ECA rules 称为 trigger。

- 三部分：event（激活 trigger），condition（check 去 run 哪个 trigger），action（trigger 具体做什么）。

- active database（不同于传统被动数据库的主动数据库）rules（ECA rules）。

- 定义语法：

  ![image-20211224171053718](.\..\..\typora-user-images\image-20211224171053718.png)
  
- 示例：

  ```sql
  CREATE TRIGGER youngSailorUpdate
  AFTER INSERT ON SAILORS
  REFERENCING NEW TABLE AS NewSailors
  FOR EACH STATEMENT -- (也可能是row)
  INSERT
  	INTO YoungSailors(sid, name, age, rating)
  	SELECT sid, name, age, rating
  	FROM NewSailors N
  	WHERE N.age <= 18; -- (也可以用WHEN)
  ```

  ```sql
  CREATE TRIGGER 库存控制
  AFTER UPDATE OF 库存量 ON 库存
  REFERENCING NEW AS N
  FOR EACH ROW
  WHEN (N.库存量 < N.库存下限 AND NOT EXISTS(
  		SELECT * FROM 在购订单 WHERE 零件号=N.零件号)
       )
  INSERT INTO 在购订单
  	VALUES(N.零件号, N.订购量, SYSDATE);
  ```

  ```sql
  CREATE TRIGGER students_SNO_update
  BEFORE UPDATE OF SNO ON STUDENT
  REFERENCING OLD AS O
  FOR EACH ROW
  WHEN (EXISTS(SELECT * FROM SC WHERE SNO=O.SNO))
  ROLLBACK;
  ```

- 如何执行：

  - immediate execution，立即执行。
  - deferred execution，延迟执行，事务提交时统一来 check 哪些 tuple 触发 trigger。
  - decoupled or detached mode，分离执行。将 trigger 作为一个新事务。若原事务 rollback，则它也 rollback。

- cascading trigger，连锁触发。如果成环怎么办：触发图 trigger graph（os deadlock的思想），或对 cascading 次数加以限制（16-64）。

- 实现方法：

  - loosely coupling，松耦合：在DBMS上包一层，拦截用户语句。
  - tightly coupling，紧耦合：DB2 Oracle。
  - nested method：介于前两者之间，比如改解析 statement 的语法树。

王能赋老师课本的trigger代码：

![image-20211224155147900](.\..\..\typora-user-images\image-20211224155147900.png)

![image-20211224155502023](.\..\..\typora-user-images\image-20211224155502023.png)

![image-20211224155516491](.\..\..\typora-user-images\image-20211224155516491.png)

![image-20211224155535161](.\..\..\typora-user-images\image-20211224155535161.png)

![image-20211224155556277](.\..\..\typora-user-images\image-20211224155556277.png)

![image-20211224155630695](.\..\..\typora-user-images\image-20211224155630695.png)

![image-20211224155649063](.\..\..\typora-user-images\image-20211224155649063.png)

![image-20211224155707630](.\..\..\typora-user-images\image-20211224155707630.png)

![image-20211224155731779](.\..\..\typora-user-images\image-20211224155731779.png)

![image-20211224162137224](.\..\..\typora-user-images\image-20211224162137224.png)

![image-20211224165021322](.\..\..\typora-user-images\image-20211224165021322.png)

```sql
create trigger xxx
before insert on Sailors
referencing NEW as N
for each row
when not (
    exists (
        select * from sailors
        where sailors.sid = N.master)
    and
    	(select count(*) from sailors
        where sailors.master = N.master) < 2
) rollback;
```







