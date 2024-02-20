# db 06 database design

数据依赖关系：

- 函数依赖：比如说，我的其他信息都依赖于一卡通号，可以根据一卡通号确定我所有的信息，不会出现 一个一卡通号 → 多个信息。【重点考察】
- 多值依赖：一个老师只会讲coca，或者一个老师只在cs专业讲课（不太明白），很少见。
- 连接依赖：(S, P, J) = (S, P) $\bowtie$ (P, J) $\bowtie$​ (S, J)，RHS是projection结果，这种依赖叫做无损连接，很少见。

## relational DB's 5 normal form

- 1NF：每一个 attribute 必须是 atomic 的，不可再分，不允许表中套表。
- 2NF：1NF，且no partially function dependency exists between attributes，不存在部分函数依赖。比如 (sid, sname, address, cid, grade)，sid+cid 是主键，grade 依赖于 sid+cid，但 sname 和 address 只依赖于 sid，这就是部分依赖，不符合2NF。
  - insert / delete abnormity：如果一个学生一门课都没选 / 把课全退了，主键是sid+cid，就没法记录该学生的信息。
  - redundancy / hard to update：不必要的数据冗余，一个学生选了500门课，就冗余499份 sname 和 address；一旦搬家，就要改500份 address。
  - 解决方案：一事一地，one fact in one place。
- 3NF【常用】：2NF，且no transfer function dependency exists between attributes，不存在属性对主键的传递依赖。比如 (employee#, salary_level, salary)，employee编号确定，工资level就确定了，具体工资也就确定了。其实salary只依赖于salary_level，这就是传递依赖，不符合2NF。
  - insert abnormity：如果一个打工人的级别暂时没确定，就不能录入level→salary mapping 信息。（没太听懂）
  - delete abnormity：如果一个级别只有一个打工人，当这个人辞职了，这个级别的 level→salary mapping 信息就丢失了。
  - redundancy / hard to update：不必要的数据冗余，三级打工人有500个，就冗余499份 level→salary mapping；一旦涨工资，就要改500份 level→salary mapping。
  - 有一个地位相当的范式BCNF，稍微严格一点点。是3NF但不是BCNF的反例：(城市, 街道, 邮政编码)，城市+街道→邮政编码，但给定邮政编码就能知道城市。但这种情况非常少，一般认为3NF<=>BCNF。
- 4NF：3NF，且消除可能存在的多值依赖。
- 5NF：4NF，且消除可能存在的连接依赖。

## ER graph

看到 57讲 17min了。













