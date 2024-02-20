听说今年会加强对第六章的考察，这个事情就很危，因为往年貌似从来没有考过第六章。

## 首先我们建立概念

### 课程框架

对用户输入的高级语言，首先进行分析（analysis），然后进行合成（synthesis）。

分析：

- lexical，词法分析，识别单词类型是什么（int x = 3 → int, x, =, 3)
- syntax，语法分析，int x = 3 符合语法规则吗，fit 哪一条语法规则。
- semantic，语义分析，这个语法规则代表什么意思，我们为语法规则配上什么样的动作。
  - type checking，类型检查，（不知道在干什么）。

合成：

- run-time env，为变量分配内存，侧重考察递归过程的内存分配。
- 生成中间代码，intermediate code，为最终生成代码过渡一下。
- optimize，优化中间代码，让中间代码效率更高。
- 生成真正的代码。

### lexical

- language：字符串集合，比如 L = {ω | ω∈(a,b)*} 。
- grammar 是表达 language 构成规则的形式化方法。
- grammar： $G=(V_N,V_T,S,P)$，非终结符集，终结符集，文法开始符号，产生式。
- grammar 的分类，没事这不重要：chomsky 0，1（产生式左边长度 ≤ 右边长度，要考虑上下文），2（context free），3（A -> αB / A -> Bα，B在左边左线性，B在右边右线性）。
- 【重点考察】由 language 反推 grammar。
- re ↔ FA ↔ grammar。
- NFA → DFA：二进制状态法，n state 的 NFA，变成DFA 最多可能有 2^n 个 state。
- DFA → minimum DFA：集合划分法，把所有状态认为是一个集合，读入字符，根据集合状态转移的分歧，切割集合。
- 画FA从1开始标状态。
- 【重点考察】grammar → re → NFA → DFA → minimum DFA。

这样，就可以通过匹配DFA，完成 一串高级语言 → 一串 token 了。

### syntax

- first集：本非终结符 生成的第一个终结符，对每个产生式写一个集合。
- follow集：紧跟 本非终结符 的第一个终结符，一个非终结符一个集合。
- 左递归：先非直接左递归 → 直接左递归，再消除左递归。
- LL(1) 的 parsing table：
  - 行是非终结符，列是终结符。
  - A→α，a∈FIRST(α)，则在A行a列填上A→α。
  - 有A→ε，且a∈FOLLOW(A)，则在A行a列填上A→ε。
- 【重点考察】消除左递归 → 提左公共因子 → 构造LL(1) parsing table。
- LR 要先改写文法，S' → S 。
- LR(1) 的 parsing table：
  - 行是状态编号，列是终结符 action 非终结符 goto。
- 【重点考察】写自动机 → 构造LR(1) parsing table。
- （最后19级考了 SLR；19级题目小改，可能20级题目会大改）

### semantic

- 基本pattern：
- ![image-20211113183412939](.\..\..\typora-user-images\image-20211113183412939.png)
- ![image-20211118103026936](.\..\..\typora-user-images\image-20211118103026936.png)
- 【重点考察】annotated parse tree，带注释的语法树。

### runtime env

- 活动记录：过程（函数、方法）的每次执行，称为该过程的一个活动；给活动分配一块连续存储区，称为活动记录。
- 活动树：程序串行执行，父节点调用子节点，兄弟节点从左到右执行，我的左兄弟执行完我才能执行。
- 注意，main函数没有形参个数、形参值、返回地址。
- 【重点考察】画内存分配。
- 建议：看答案，看完就会了。

### intermediate code

- 三地址码：
  - ** 运算符：x = y op z，x = op y。
  - ** 直接赋值：x = y。
  - ** 跳转：goto L，if x relation_op y goto L，if a goto L。
  - 传参、转子：param x，call p,n。
  - ** 返回语句：return y。
  - ** 索引赋值：x := y[i]，x[i] := y。
- 【重点考察】高级语言 → 三地址码。
- 四元式：op，arg1，arg2，result。arg12不能颠倒。
- ![image-20211218174757569](.\..\..\typora-user-images\image-20211218174757569.png)
- 【重点考察】高级语言 → 四元式。
- 建议：看ch8笔记，笔记末尾整理了pattern。

### optimization

- 基本pattern：
- ![image-20211219054034358](.\..\..\typora-user-images\image-20211219054034358.png)
- ![image-20211219054054235](.\..\..\typora-user-images\image-20211219054054235.png)
- 每个圈标号：n1 n2 n3 n4。
- 最初的变量标在圆圈下方，算出来/赋值后出现的变量标在圆圈侧边。
- 叶节点要按照opd顺序排！
- 根据 出基本块活跃的符号 及其依赖的符号，删代码。
- 【重点考察】构造DAG，重写代码。

