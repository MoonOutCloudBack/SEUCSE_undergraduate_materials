## 列个计划

- [x] embedded SQL
- [x] recovery
- [x] query optimization
- [x] trigger
- [x] concurrency
- [x] relational algebra & calculus

 



## 复习重点

### high-level 复习重点

- primary / candidate / foreign key。relational algebra & calculus。
- SQL查询。
- query optimization（索引，B+树）。
- concurrency。
- recovery。
- embedded SQL。
- trigger。
- integrity constraint，NF。

### low-level 复习重点

- 用 algebra TRC DRC 表示除法。

- recovery 的两个 rule 以及为什么。

  ![image-20211226130256704](.\..\..\typora-user-images\image-20211226130256704.png)

- 

### 简答押题



## 原始材料

quiz3：

1. B树和B+树的区别，为什么DBMS用B+树。
2. 写SQL查询。

quiz5：

1. 数据独立性的意义和实现方法，范式越高越好吗。
2. primary / candidate / foreign key。
3. 写SQL。
4. query optimization，relational DB 比 hierarchical / network 更需要优化。
5. concurrency，模拟运行加锁过程。
6. trigger。

quiz6：

1. 数据独立性的意义和实现方法。
2. 关系代数，relational algebra & domain relational calculus & tuple relational calculus。
3. 写SQL。
4. query optimization，为什么树型索引是主流索引，玄学题。
5. concurrency，死锁，相容矩阵的合理性。
6. recovery。
6. primary / candidate / foreign key，schema的范式与问题。

quiz7（15-16）：

1. concurrency，为什么可串行化即正确。
2. primary / candidate / foreign key。
3. 写SQL。
4. query optimization，为什么稠密索引不一定提高效率。
5. 数据库设计，完整性约束+规范化（范式），设计schema。
6. query optimization，relational DB 比 hierarchical / network 更需要优化。
7. recovery。
8. trigger。

20-21：

1. recovery，数据冗余的优缺点。
2. primary / candidate / foreign key。
3. 写SQL语句。
4. embedded SQL。
5. trigger。
6. recovery，checkpoint的作用，什么时候清空。
7. concurrency，serializable，相容矩阵合理性，SX与SUX的比较。
8. ER图。

17-18：

1. 网状数据模型怎么表达 m:n 关系，relational model 和 network model 的本质区别。
2. relational algebra。
3. 写SQL。
4. embedded SQL。
5. query optimization，主键索引，查＞20% 簇集索引比非簇集索引号。
6. concurrency，SUX比SX提高并发度。
7. 数据库设计，ER图，写schema，primary / candidate / foreign key，范式。
8. trigger。

14-15：

1. 玄学题：现代数据库怎么管理数据模式的？数据模型怎么影响系统性能？什么是结构化数据， 半结构化数据，非结构化数据？
2. SQL和 relational algebra 的异同。
3. primary / candidate / foreign key，写SQL。
4. 【可忽略】query optimization，给定索引，执行代价。
5. recovery。
6. concurrency，相容矩阵合理性。

















