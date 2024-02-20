# db 02 data model

## evolution

### hierarchical data model

基本想法：用tree structure建模现实世界。

record & field：

- 学生：record；
- id、选课信息等：学生record的field，

基本unit是PCR：parent-child relationship，1:N的关系。每一个record只能有一个parent。

无法建模：M:N的关系（3个学生同时参加两门课），multi parents，三方的关系。

#### 补救方法：virtual record

或者叫做pointer。

![image-20210813101429571](.\..\..\typora-user-images\image-20210813101429571.png)

M:N：两个方向都建模，用指针互相找到。

multi parents：另一个parent的child是virtual record，可以通过它的pointer找到真的record。

第三个没听懂。

### network data model

基本数据结构：set，一个1:N的关系。owner对应N个member。用循环链表实现。

允许一个record称为多个set的member，同时作为多个set的owner。

record & data items：data items和hierarchical的field类似，但是可以是向量。（ws：表中套表）

LINK record type：可以描述hierarchical受限的3种情况。

![image-20210813104224142](.\..\..\typora-user-images\image-20210813104224142.png)

## relational data model

基本数据结构：table，或relation。用表建模世间万物。

- 基于set theory，高度抽象。
- 屏蔽底层细节，把操作变成清晰简单的表的计算。
- 建立新的algebra system：relational algebra。
- non procedure query language（非过程性查询语言）：SQL。标准的5大运算。
- soft link：与以前data models的核心区别。

entity的features：relation的attributes。

attribute的value scope：domain，数据范围。数据是原子的，不能是vector（表中套表）。

一个relation是一个N方关系。

![image-20210813111156349](.\..\..\typora-user-images\image-20210813111156349.png)

这就是R的schema。R的一个instance value可以被写成r(R)，是n-tuple的集合。

tuple：可以理解为表的一行。r可以理解为表行的集合。

每一个tuple可以被写成：ti=<v1, v2, ..., vn>, vi∈Di, 1≤i≤n。

因此 ti∈D1×D2×...×Dn, 1≤i≤n；

有 $r\subseteq D1×D2×...×Dn$​。

### primary key

一个attributes的集合是candidate key，需要满足2个条件：

- 不存在两个distinct tuples有这些attributes的same value。
- 对于我们考察的attribute set的任意subset，不满足第一个条件。也就是我们考察的set是最小的set。
  - 不满足第二个条件：superkey。
  - relation有＞1个key的时候，选一个primary key，剩下的叫做alternate key。
  - 如果primary key包含了relation的所有attribute，把它叫做all key。

### foreign key

一个attribute集合，可以用来索引另一个relation里的tuple。

ppt：correspond to另一个relation的主键，是【就是另一张表的主键】的意思吗？

感觉是两张表之间的关系。可以理解为一个pointer。

- referential integrity constraint：出现在该relation的foreign key都必须出现在另一个relation里，即没有空指针。integrity就是没有空指针。

- domain integrity constraint：域完整性约束，数据的值要在定义域里。
- entity integrity constraint：每一个表都需要有主键。主键的值不能为NULL。

### relational algebra

basic operations：

- selection（σ）：筛选出一个row的子集。
- projection（$\pi$）：删除不想要的column。
- cross-product（×）：结合两个relation。
- set-difference（-）：找到在relation1但是不在relation2里的tuple。
- union（∪）：把两个relation并起来。

$\{\sigma,\pi,\times,-,\cup\}$​是完整的操作算子，其他的操作都可以由它们推出来。

还有其他的操作：intersection、join、outer join、division等。

表和表之间的运算是封闭的，运算的结果仍然是表。

#### projection

删除不在projection list里的attributes。

这样写：$\pi_{sname,rating}(S2)$，sname rating是projection list的内容，S2是个relation。

操作完了之后，schema就只包含projection list里的fields了。

操作完了之后，出现重复的row该怎么办？real system不会主动删除重复的row（考虑算平均值或统计数量），除非显式要求这样做。

#### selection

选出满足selection condition的row。

这样写：$\sigma_{rating\gt8}(S2)$，selection condition以下标的形式写。

这次选出的都是row，schema没有变，不会duplication了。

#### union, intersection, set-difference

这些都是二元操作。要求两个操作数（relation）union-compatible。

union-compatible：field的数量相同 && 对应的attribute的type相同，好像就是schema相同。

schema没有变。

#### cross-product

relation1的每一行与relation2的每一行pair一下，形成一个新表。

renaming operator，重命名操作：$\rho(C(1\rightarrow sid1,5\rightarrow sid2),S1\times R1)$​。​​

#### join

condition join：$R \bowtie S=\sigma_C(R\times S)$​​，先叉乘再condition筛选，有时被叫做theta-join。

结果的schema和叉乘的schema一样。

equi-join：一种特殊的condition join，condition只包含【对重复attribute拥有相同值】的行。

equi-join这样写：$S1\bowtie_{sid}R1$​​，sid是要equal的attribute。

natural-join：一种特殊的equi-join，condition是所有common attributes都equal。

natural-join就是通常所说的join，即$\bowtie$​。

#### division

对这一类应用非常有效：哪个水手把5艘船全预定过一遍。

假设A有2个fields分别是x和y，B只有一个field y。

则$A/B=\{\lang x\rang|~\exist\lang x,y\rang \in A~\forall\lang y\rang\in B\}$​。

也就是说，A/B包含所有的x tuple，使得对于每一个B的y tuple，能从A里找到一个xy tuple。满足B中所有y的A中的x，才能被A/B挑选出来。

schema就是xy-y，即x。

用basic operators来描述A/B：$\pi_x(A)-\pi_x((\pi_x(A)\times B)-A)$​​​​。就是用【A中所有x】减去【A中不全满足y的x】。后者可以表示为【假设A所有x都满足B（用一个叉乘得到）】减去【真实的A】（即梦想与现实的差距，这就是我们要减掉的部分），别忘了对整个xy tuple取column x。

#### outer joins

- left outer joins，$^*\bowtie$：根据左边这张表来，行数和左表相同，没有的地方补null。
- right outer joins，$\bowtie^*$：根据右边这张表来，行数和右表相同，没有的地方补null。
- full outer joins，$^*\bowtie^*$​：兼顾左右。

![image-20210816203703027](.\..\..\typora-user-images\image-20210816203703027.png)

#### outer unions

对两个不union-compatible的relation做union。强行union，不同时出现在两张表的attribute赋为null。

### relational calculus

relational algebra：需要保证运算顺序，即对查询过程给出确定性描述。

保证运算顺序：就像2+3\*5和(2+3)\*5。

relational calculus：一个不需要知道运算过程和运算符顺序的数学，它是non-procedure的。

calculus：variables, constants, comparison oprators, logical connectives, quantifiers。

关系型数据库的重大优势：non-procedure，不需要保证顺序。

两种方式：

- TRC：variables range over (i.e., get bound to) tuples。变量定义在tuple上，元组关系计算。tuple relational calculus。
- DRC：variables range over domain elements (attribute values)。变量定义在attribute values上。domain relational calculus。

calculus中的表达式称为formulas。answer tuples（即我们想要的查询结果）让formulars为true。

#### domain RC

query的形式：{<x1,x2,...,xn> | P(x1,x2,...,xn,xn+1,...,xn+m)}；P是希望为true的条件；这n+m个x称为domain variables。

我们可以把atomic formular连起来，构造更复杂更强的expression。

atomic formula：

- <x1,x2,..,xn>∈Rname，或者X op Y，或者X op constant。
- op：＜、＞、＝、≤、≥、≠。

formular：

- 一个atomic formular。
- $\lnot p$，$p\land q$，$p\lor q$。
- $\exist X(p(X))$或 $\forall X(p(X))$，X需要在p(X)中free。

| 左边的variables必须是 | 右边formular中唯一的变量。

变量的bound与free：被 $\exist$ 和 $\forall$ 限定的变量是bound的，不bound的变量就是free的。

除法，R1(ABCD) / R2(CD)：
$$
\{\lang A,B\rang~|~\forall~\lang C,D\rang\in R2~(\lang A,B,C,D\rang\in R1)~\}
$$

#### tuple RC

query的形式：{t[\<attribute list\>] | P(t)}；

attribute list是我们想要查找出来的attribute，相当于一个projection。

在条件P里，用“.”索引tuple的domain。

举个例子：$\{t[Name]|(t\in Sailors)\lor (t.T>7)\lor (t.Age<50)\}$；

![image-20211224173819736](.\..\..\typora-user-images\image-20211224173819736.png)

除法，R1(ABCD) / R2(CD)：
$$
\{P~|~\forall~t2\in R2~\exist~t1\in R1
\\
(t1.A=P.A~\and~t1.B=P.B~\and~t1.C=t2.C~\and~t1.D=t2.D)\}
$$

#### 一些细节

unsafe queries：有无穷多个answers的query。语法上正确，但物理意义不对。比如：$\{S|\lnot (S\in Sailors)\}$；

relational completeness：relational calculus是【关系完备】的。

证明：relational algebra的5个基本运算可以用expression描述，而algebra是relational completeness的。

SQL就是基于relational calculus的。

### 评价

- suitable for Online Transaction Processing (OLTP) applications，比如说一些类似于登记成绩/交易记录的场景；
- 基于records，不能更好地面向users和applications。
- 不能自然地描述entities之间的关系。
- 缺少semantic information：面向对象……
- 数据类型太少了，很难满足现实世界的应用。

## 其他data model

### ER data model

entity：用来表示一个real-word object，它有一些attributes，和relational data model的定义类似。

entity set：一个包含相似entities的collection，如employees。

- 在一个entity set里的所有entities都有相同的attributes set。
- 每一个entity set都有一个key。
- 每一个attribute都有一个domain。
- 允许联合或多值的属性。比如多个手机号这样的。

是relational data model的extension。

relationship：两个或多个entities之间的关系。允许关系带attributes，允许多方关系，允许relationship set（collection of similar relationship），允许一个entity参与多个relation。

#### ER diagram【考过！】

![image-20210823144328935](.\..\..\typora-user-images\image-20210823144328935.png)

#### 一些概念

- cardinality ratio constraints：分为1:1（配偶）、1:N（雇员在多个部门工作）、N:1（多个雇员）、M:N（考虑前述两种雇员情况）。
- participation constraints：entity参与relationship的min/max数量。
- 如果一个department必须有一个manager，那么department是manage relationship的【total participation】。反之叫做partial。


advanced topic：更高级的功能，为了尽量自然地建模现实世界。

- weak entity：临时描述不太重要的entity，比如教师亲属看病优惠时建模教师亲属。
- specialization & generalization：有点像继承，比如研究生之于学生。
- aggregation：将一个relationship set当作entity set，然后拿它们去participate其他relationships。
- category：由不同种类entity组成的entity set，如私有账户/公有账户都在账户category。

### object-oriented data model

弥补了relation data model的shortage，Object-Relation DBMS。

### 还有一些data models

- logic-based data model (deductive DBMS)：
  - 扩展的query功能（尤其是递归的query）。
  - DBMS有推导能力。
- temporal data model，时间上的；
- spatial data model，空间上的；
- XML data model：
  - 在internet上存储数据
  - common data exchange standard，常用数据交换标准。
  - information systems integration。
  - 半组织的数据，expressionof semi-structured data。

## 总结

- data model是DBMS的核心。
- data model是在database中simulate real world的methodology。
- 每一个DBMS都实现了一个data model。





