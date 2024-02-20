# cp 03 lexical analysis

词法分析。得到每个单词的类型，形成一个token序列。

lexeme指token的实例化。

主要内容：

- regular expression：正则表达式，描述语言的pattern。
- finite automata：有限自动机，识别语言的模型。【NFA→DFA→reduced DFA】，感觉常考。
- regular grammar：正规文法。分为左线性右线性（看B在哪边）。

## regular expression

由regular grammar生成的language的集合，可以用regular expression来写。一个regular expression表示一个language，即句子的集合，叫做regular set。

### 生成re的规则

- ε是一个re，表示集合{ε}。
- 如果a属于字母表Σ，那么re里的a代表集合{a}。
- α和β都是正则表达式，则
  - α | β，L(α)∪L(β)，可以是α也可以是β；
  - αβ，L(α)L(β)，concatenation，笛卡尔积。
  - α\*，(L(α))*，kleene闭包，重复0或正整数次。
  - (α)，就是L(α)。
- 生成一个re，只能运用以上3条规则有限次。

运算符优先级：*＞concatenation＞| 。

### equality of re

若两个re生成的language是一样的，则认为它们equivalent，记作α=β。

### algebraic laws of re

- | 满足commutative（交换律）associative（结合律）。
- concatenation满足associative，并且distribute over |（对|有分配律）。α(β|γ)=αβ|αγ，(α|β)γ=αγ|βγ。
- ε是concatenation的单位元。
- α\*=(α|ε)\*。
- (α\*)\*=α*，重复做kleene闭包和做一次一样；
- (α | β)\*= (α\* | β\*)\*= (α\*β\*)\*，就是由α和β生成的任意长的串。
- α=β|γα <=> α=γ*β，递归re规则的化简。
- α=β|αγ <=> α=βγ*；

### extentions of re

- 一个或多个：α+；
- 0或1个：α？
- [a-z]，[A-Za-z]，[A-Za-z0-9]；

## finite automata

### transition diagram

圈：状态。双圈：accepting state，终态。

边：状态转换。边上写着字符/单字符的pattern，表示接收到一个字符/pattern后转换到另一个状态。

### 描述

deterministic FA：读入一个单字符的pattern后，状态转换方向确定；DFA。

non-deterministic FA：可能往多个状态去转换。

#### DFA

五元组，M(S,Σ,move,s0,F)：

- S：状态集合；
- Σ：输入字符的symbol alphabet。
- move：transition function，一个从s×Σ到s的mapping，move(s,a)=s'。
- s0：initial state，s0∈S。
- F：final states set。

DFA的特点：

- 没有空串边；就是指向别人/自己的写着ε的边。
- 读入一个字符后，只有一个确定的状态转换方向。
- DFA所定义的language：DFA接受一个串 当且仅当 这个串能构造出一个【从初态到终态的path】。

构造DFA：见ppt。感觉上还要根据DFA写language。

#### NFA

move与DFA不同：从S×Σ到S的mapping，move(s,a)=$2^S$，$2^S\subseteq$S。

识别：多个转移方向，空串边；NFA里，ε是合法的输入字符。

根据NFA写language：见ppt。感觉有点难。

### NFA→DFA

子集法，subset：把我同时可能进入的两个状态合起来，当作一个新状态。

从NFA(S,Σ,f,S0,Z) 生成 等价的DFA(Q,Σ,δ,I0,F)：

- I0={S0}，Q初始化为{I0}；DFA的初态就是NFA的初态，DFA的状态集合初始化为NFA的初态。
- 对状态集合Q内的每一个没被考察过的状态，考察【接受Σ中每个字符】后的状态；如果出现了新的状态，那么就再把这个新状态添加到Q里。
  - 比如说，若s1接受1后可以转换到s0和s1，则我们设状态q2={s0,s1}，这就是一个新状态了。
  - 考察q2接受1后的状态，就看s0接受1后变成什么，s1接受1后变成什么，把【所有可能变成的东西】再求并集得到转移状态，看看Q里面有没有这个状态。
- 重复第2步，直到没有新状态能被加到Q里。
- 终态集合：F={I | I∈Q and I∩Z!=空集}；只要包含原来的终态，这个状态就可以作为新终态。

例题：见ppt。

#### 空串边的情况

通过空串边能到达的状态，都合并成一个新状态，即求空串的闭包。

一个空串闭包内的状态互相等价，这句话是错的。一个ε-closure里的状态不一定能互相到达，只有是强连通图才互相等价。

procedure：

- Q初始化为I0=ε-closure(S0)；DFA的状态集合初始化为NFA初态的ε-closure。
- 对状态集合Q内的每一个没被考察过的状态，考察【分别接受Σ中每个字符】后的状态，对接受一个字符能转移到的状态求ε-closure，最后把它们并起来；如果ε-closure是新的状态，那么就把这个新状态添加到Q里。
- 重复第2步，直到没有新状态能被加到Q里。
- 终态集合：F={I | I∈Q and I∩Z!=空集}；只要包含原来的终态，这个状态就可以作为新终态。和无空串的NFA一样。

例题：见ppt。

### reduced DFA

DFA状态数目最小化。

思路：

- 把states划分为非终态集和终态集。假定所有终态集/非终态集是等价的。
- 接下来验证划分是否合理，考察接受一个字符后转换方向是否相同，集合内状态的行为是否等价。
- 如果不合理（行为不等价）就继续划分，直到划分都合理了。

算法：

- input：DFA M={S,Σ,move,s0,F}；
- output：DFA M'，与M accept一样的language，states数量尽可能少。
- procedure：
  1. 构造一个初始partition Π0，分为两个group：非终态集（包含dead state）和终态集。（DFA图中，缺边的状态连到dead state上，用虚边连。）
  2. 对每一个partition里的group，如果读入一个字符后group的行为不同（转移到不同的group），就把I按不同的行为划分为subgroup。
  3. 如果$\Pi_{i+1}=\Pi_i$，即partition没有变化，那么$\Pi_{final}=\Pi_i$，迭代结束，去第4步。否则继续重复第2步。
  4. 在每一个group里，选择一个状态作为代表。
  5. 如果M'含有dead state（不是终态，无论输入是什么都转移到它自己，即完全出不去）或者根本unreachable的state，remove这些state。

例题：见ppt。

## re ↔ FA

### re→FA

thompson's construction rules，简化版本：

![image-20210826101546346](.\..\..\typora-user-images\image-20210826101546346.png)

#### 一个【描述→DFA】的思路：状态划分

01的二进制数，被5整除。

分为5个状态：模5后余01234，构造DFA。

### FA→re

逆过程，合并。

如果初态和终态相同，首先分离增加一个初态一个终态，用ε相连。

去掉节点，重新构造通过节点的各种路径。

## FA ↔ grammar

### right-linear regular grammar→NFA

右线性正规文法（RRG）：A→aB，A→a。是我们常写的格式。

对每一个RRG，都存在一个NFA，它们生成的language相同。

对每一个NFA，都存在一个RRG一个LLG，生成的language相同。

- input：G=(V_N, V_T, P, S)；分别是非终结符、终结符、产生式、文法开始符号。
- output：FA M=(Q, Σ, move, q0, Z)；分别是状态集合、symbol alphabet、move函数、初态集合、终态集合。
- procedure：
  - 把每一个非终结符作为一个状态，再增加一个终态T；
  - Q=V_N∪{T}，Σ=V_T，q0=S。如果有S→epsilon，那么Z={S,T}，否则Z={T}。
  - 对A→aB，构造move(A,a)=B；
  - 对A→a，构造move(A,a)=T；

### FA→right-linear regular grammar

- input：M=(Q, Σ, f, S, Z))；
- output：Rg=(V_N, V_T, P, S)；
- procedure：
  - 把每一个非终结符作为一个状态，再增加一个终态T；
  - Q=V_N∪{T}，Σ=V_T，q0=S。如果有S→epsilon，那么Z={S,T}，否则Z={T}。
  - 对move(A,a)=B，构造A→aB；
  - 如果A∈Z，构造A→ε。如果S∈Z，构造S→ε。

### left-linear regular grammar→FA

左线性正规文法。

- 初始化：
  - LLG的文法开始符号S，对应FA M中的终态Z。
  - 给M新增加一个初态q0，取M的状态集合Q=V_N∪{q0}。
  - 若LLG的产生式P中有S→ε，则终态集合Z={S, q0}，否则Z={S}。
  - Σ=V_T，symbol alphabet=终结符集合。
- 对于P中每个A→Ba，在M中构造move(B,a)=A。B读入a之后可以变成A。
- 对于P中每个A→a，在M中构造move(q0,a)=A。





















