# cp 04 syntax analysis

## background

词法分析：得到一串token，把token交给语法分析。

语法分析：得到语法树parsing tree。下推自动机PDA。

parser：

- 从lexical analyzer那边获得token序列。
- 确定这个token序列是否能被我们的grammar生成。
- 报告syntax errors。
- 自动修复常见的syntax error，这样就可以继续向下parse了。
- 生成parsing tree。

top-down method：

- 从root到leaves。
- 推导（derive）和匹配（match）。
- 只能分析LL(1) grammars。Left-to-right-scan Leftmost-derivation。

bottom-up method：

- 规约（reduce），从下而上一直规约到文法开始符号。
- 应用范围更广，LR grammars。Left-to-right-scan Rightmost-derivation。

syntax errors：

- lexical：拼错identifier、keywords、operator。
- grammar：括号匹配不上。
- semantic：类型不兼容的opd。
- logical：无限递归调用。

syntax errors handling的目标：报告错误，recover（这样就能继续检测后面的错误了），不能严重降低速度。

strategies：panic mode（抛弃），phrase level（替换）。

## top-down，LL parsing

### basic ideas

- left-most derivation (top-down)，match。
- backtracking，retry。

递归下降，recursive-descent parsing。

每个非终结符对应一个子程序，当出现mismatch时，返回上一层，选择下一个来匹配。

inefficient：最坏情况下指数级复杂性。因此不希望回溯，non-backtracking，predictive。

影响 top-down parsing 的因素：

- 产生式的公共前缀，左公共因子。则不知道应该使用哪一个产生式。
- 左递归，如L→LD|D，那就无穷推导L了。要把左递归换成右递归。
- ambiguity grammar，二义性的文法，一个句子对应多个parsing tree。

### 准备工作

#### eliminating ambiguity

消除二义性：重写语法，rewriting the grammar。

经典例子：

- $$
  E\rightarrow E+E~|~E*E~|~(E)~|~id
  \\
  \downarrow
  \\
  \left.\begin{array}{}
  E\rightarrow E+T~|~T\\
  T\rightarrow T*F~|~F\\
  F\rightarrow (E)~|~id
  \end{array}
  \right.
  $$

- ![image-20211019103049812](.\..\..\typora-user-images\image-20211019103049812.png)

#### 左递归【重要】

- 直接左递归，immediate left recursion：P→Pa|b；
- 间接左递归。Indirect left recursion：P→Aa，A→Pb。将A代入得到P→Pba，可以化成直接左递归。

消掉 immediate left recursion：

- simple：P→Pa|b。
  - 观察形式：b，ba，baa，…
  - 化成：P→bP'，P'→aP'|ε。
- general：P→Pa1|Pa2|…|Pam|b1|b2|…|bn。
  - 化成：P→b1P'|…|bnP'，P'→a1P'|…|amP'|ε。

indirect → immediate left recursion（见ppt页34/76）：

- 将所有的非终结符随便定一个顺序：P1…n。
- 对 i 从1到n，
  - 对 k 从1到 i-1（k＜i）：
    - 如果有Pi→Pk γ，Pk→d1|…|dm，变成Pi→d1 γ|…|dm γ。
  - 然后消除left recursion。
- 最后重新整理一下grammar。

### non-recursive predictive parser

通过查parsing table实现predictive。

#### 使用parsing table

- 行是非终结符，列是终结符。
- 我们进行left derivation，根据目前要derive的非终结符确定行，根据下一个终结符确定列，使用那个格子里的产生式。
- 目前要derive的非终结符怎么确定？
  - （用栈实现parsing tree的前序遍历（VLR））。
  - 取出栈顶，使用产生式，把得到的【终结符+非终结符】都逆序压进栈，就是右边先进、左边后进、最左边在栈顶。
  - 如果栈顶是匹配的终结符，stack.pop()。如果栈顶是非终结符，继续查parsing table使用产生式。
  - 技术细节：最开始我们把“#”压进栈，所以当stack.top()=='#'的时候，证明解析成功。

#### 构造parsing table

- first集（我生成的第一个终结符）的计算：
  - 形式化定义：FIRST(A)={a|A→a…, a∈V*}。就是对每个A生成的串，串首的终结符。
  - 计算：把α展开为α1α2…αn。
    - 如果ε !∈ FIRST(α1)，则FIRST(α)=FIRST(α1)。
    - FIRST(α)=(FIRST(α1)-{ε})∪…∪(FIRST(αk)-{ε})∪FIRST(αk)，前k-1个FIRST都包含ε，但FIRST(αk)不包含。
    - 如果一直到FIRST(αn)都包含ε，那么最后执行FIRST(α)∪={ε}。
  - 计算是自底而上的。
- follow集（紧跟我的第一个终结符）的计算：
  - 形式化定义：FOLLOW(A)={a|S→…Aa…, a∈V*}。就是【跟在非终结符A后面】的终结符都是什么。如果S→…A，那么 #∈FOLLOW(A)。
  - 计算：
    - 对每个 N→αAβ，FOLLOW(A)∪=FIRST(β)-{ε}。
    - 如果ε属于FIRST(β)，或者有N→αA，则 FOLLOW(A)∪=FOLLOW(N)。
    - #∈FOLLOW(S)，但不一定 FOLLOW(S)={#}，可能有别人生成S。
  - 计算是自顶而下的。
- 计算predictive parsing table：
  - A→α，a∈FIRST(α)，则在A行a列填上A→α。
  - ε∈FIRST(β)，B→β，b属于FOLLOW(B)，则在B行b列填上B→ε。

### LL(1) grammar

重要题型：给一个grammar，判断是否LL(1)。

构造分析表时，是否产生冲突？

G是LL(1)的，当且仅当：parsing table的一个格子最多填一个产生式；

或者说，对G中每一个 A->α|β ：

- FIRST(α) ∩ FIRST(β)=Φ，FIRST集不冲突。
- αβ至多一个推导出ε。如果β→ε，则FIRST(α) ∩ FOLLOW(A)=Φ。
- （Left recursion, common prefixes, ambiguity. 左递归，共同前缀，二义性。一个格子里面，不能填两条产生式。）

## bottom-up，LR parsing

### 概述

LR(k) parsing：（一般都是LR(1)）

- L：left-to-right scan，从左到右扫描。
- R：construct a rightmost derivation in reverse，逆着最右推导，即最左规约（leftmost reduction）。
- k：the number of input symbols of look ahead，往前看字符的数目。
- 不适用于 ambiguous grammar。

基本做法：

- 维护一个栈，栈底放一个#。
- 顺序读字符，压栈；如果与某个产生式右部（产生式的结果）match，则弹出产生式右部，压栈产生式的左部。
- 如果栈顶是S，则成功。

几个名词：

- handle：句柄，RHS，完整的产生式右部。
- shift：移进，就是压栈。
- reduce：pop handle, push LHS。就是拿左部替换右部。

优缺点：

- advantages：最通用，非回溯，几乎可以处理所有context-free grammars，比predictive top-down能分析更多的愈发，更快探测syntactic error，可以用自动化分析器生成。
- drawbacks：手工构造工作量太大，我们需要YACC。

### 使用 parsing table

![image-20211026111226454](.\..\..\typora-user-images\image-20211026111226454.png)

两个栈：状态栈（初始0），符号栈（初始#）。

s：shift 移进；r：reduce 归约；列：状态；后面的数字：状态转移。

读入非终结符：状态转移按右边的表。

弹出一个字符，弹出一个状态。字符和状态是同步的。

power：LR(0)<SLR(1)<LALR<LR(1) 。

### LR(0)

#### item

用一个dot来表示边界，dot左边表示已经进栈，右边表示期待它进栈。

感觉像光标一样。当dot在最右边的时候，就可以reduce了。

item：状态的意思，产生式右部+一个dot。

A→ε 用 A→· 来表示。

#### 构造NFA

不管原来如何，增加S'→S，S'作为新的文法开始符号。这样不会有产生式产生S'，并且看到S'就知道accept了。

构造一个NFA。注意空串边：dot后面有非终结符。

![image-20211028103015559](.\..\..\typora-user-images\image-20211028103015559.png)

#### 构造DFA

合并空串边，构造新的状态。

规约项目 & 移进项目。

冲突：有些项目，可能规约可能移进 & 可能规约成不同的非终结符，影响分析表构造。出现这种问题，就不是LR0。

#### 根据DFA构造parsing table

- 对项目集（CLOSURE）之间的边，移进，状态转移。
- 对规约项目，规约，弹出 |RHS| 个状态，压入 LHS。
- 若 S'→S· 属于 Ik，则ACTION[Ik, #]=acc，接受。

### SLR(1)

加了一个简单的规则，往后看一位。

构造 parsing table 时，对含有【移进-规约、规约-规约】冲突的项目集向前查看一个符号。

- 假设项目集I中，有m个移进项目 A1→α1·a1β1、A2→α2·a2β2、…、Am→αm·amβm；有n个规约项目 B1→γ1·、B2→γ2·、…、Bn→γn· 
- 只要集合{a1, a2, ..., am}和 FOLLOW(B1), FOLLOW(B2), ..., FOLLOW(Bn)，两两交集均为空集，则是SLR(1)。
- 下列规则解决冲突：
  - 若当前符号 a∈{a1, a2, ..., am}，则移进。
  - 若 a∈FOLLOW(Bi)，则使用 Bi→γi 规约。
  - 否则报错。

### LR(1)

有一个问题：FOLLOW集太宽泛了。就是对于一个字符，某些情景下的我 是可以让它FOLLOW我的，但是这个情景下 我并不能做到，因此不用考虑它。但SLR(1)还是考虑了整个FOLLOW集，容易误伤，我们需要更细化。

LR1，改进了LR0的item的结构：加一个【向前搜索符 a】，a只对规约的项目有意义，在其他项目中只起传递的作用。

- 它定义规约的条件：对 [A→α·, a]，若下一字符为a，则可以规约，否则不规约。即，细化到产生式的FOLLOW集。
- 如何得到item：
  - 最开始：[S'→·S, #]。
  - 如果 [A→α·Bβ, a] 属于 CLOSURE(I)，有产生式B→γ，b∈FIRST(βa)，则可以加入[B→·γ, b]。
- 如何得到 parsing table：
  - 对于移进项，正常移进，状态转移；
  - 对于项目集 Ik 中的规约项 [A→a·, b]，ACTION[k, b]=rj（rj是那个产生式）。不是无条件规约了。

### LALR(1)

嫌LR1项目太多，合并同心集（having the same core）。如果合并后不产生新的冲突，则是LALR1。

- 同心集：很多状态中的项目集，除了向前搜索符不同外，其余部分完全相同。
- 合并：指向前搜索符的合并。若不冲突，按照新状态建立 parsing table。
- 冲突的情况：
  - GOTO表合并，不会转向冲突，因为同心。
  - ACTION表合并：
    - 出错&出错 / 移进&移进，不会冲突。
    - 规约&规约，按同一产生式则不冲突，按不同产生式则冲突。
    - 移进&出错 / 移进&规约：不会的，因为移进&出错 / 移进&规约不同心。
    - 规约&出错：人为规定成规约，放松报错条件。最晚在【下一个符号移进前】报告出错信息，因此不影响分析过程。

### ambiguous grammar

定义优先级：

例如，【E'→E, E→ E+E | E*E | (E) | i】。

规定\*的优先级高于+，在解析 i+i\*i 时，发现下一个字符为\*，就移进\*而非规约 i+i。





