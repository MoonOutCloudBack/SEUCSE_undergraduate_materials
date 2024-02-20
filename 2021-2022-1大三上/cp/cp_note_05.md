# cp 05 syntax-directed translation

进入语义分析，semantic analysis。

SDT的任务：实现semantic analysis，生成中间代码程序。

- SDD，syntax-directed definitions：定义。
- SDT，syntax-directed translation：实现。
- bottom-up & top-down translation。
- implementing methods of SDT。

常见的中间代码形式：

- 逆波兰表示、树形表示、三元式、四元式。

总之，这一章难过的考察内容是 annotated parsing tree。

## attribute grammar

### 回顾：semantic analysis

- 审查语法结构的静态语义，验证语法结构合法的程序 是否真的有意义。
- 如果静态语义正确，执行真正的翻译，生成中间代码 / 实际的目标代码。

### attribute grammar，属性文法

attribute：

- 给 grammar 的每个符号配备一些 attribute ，如类型、值、符号表内容等。
- 分类：（好像不是 synthesized 就是 inherited，并且不能两个都是）
  - synthesized：值来源于 parsing tree 的子树节点，比如 c←a+b。
  - inherited：值来源于父节点或兄弟节点，比如【int a, b】，a 的 type 是前面的 int（其实是根节点），b 的 type和 a 一样是 int。
- 终结符只有 synthesized attribute，由 lexical analyzer 提供，因为终结符不会有子树节点。
- 文法开始符号可以有 inherited attribute，需要初始化。

attribute grammar：

- 包含一个【context-free】grammar 和 一系列 semantic rules。
- 一条 production 配备一系列 semantic rule（可能是多条），计算和传递 attribute。
- 有些 semantic rule 没有显示赋值（比如调用一个返回值为 void 的过程），就给根节点引入一个 virtual synthesized attribute。

### annotated parse tree，带注释的语法树

可恶，貌似这个反而是考试重点！

把【这个位置的非终结符】替换成【该非终结符 attribute 的赋值语句】：

![image-20211113183412939](.\..\..\typora-user-images\image-20211113183412939.png)

![image-20211118103026936](.\..\..\typora-user-images\image-20211118103026936.png)

### 基于 attribute grammar 的 semantic analysis

- dependency graph，依赖图：
  - 用依赖图（有向图）来描述相互依赖关系。
  - A→B，表示 A 把它的值给 B。
  - 建图的算法：
    - for parsing tree 中每个节点，for 节点上文法符号的每个 attribute，为 attribute（而非文法符号）建立一个节点。
    - for parsing tree 中每个节点，for 节点所用产生式的每个 semantic rules，构建有向边的依赖关系。
  - 良定义：没有循环依赖关系，即 topological sort 可以给出 semantic rule 计算的有效顺序。
  - 输入串 → parsing tree → dependency graph →topo calculation order。
  
- 树遍历：
  - 不停地遍历 parsing tree，直到计算出所有 attributes。
  - 比如深度优先，从左到右的遍历。
  
- 一遍扫描【重点】：
  - 为每一个产生式配上一组 semantic rules，并在 syntactic analysis 的同时，进行 semantic analysis（执行 semantic rules，计算 attribute）。
  
  - 时机：
  
    - top-down，产生式匹配成功时。
    - bottom-up，产生式规约成功时。
  
  - abstract syntax tree，抽象语法树，AST（ppt上好像没有，但网课上有）：
  
    - 更精简有效的源程序中间表示。
  
    - 三种操作：
  
      - `mknode(op, left, right)`：建立一个 operator 节点，left right 指向左右子树。
      - `mkleaf(id, entry)`：建立一个标识符节点，变量名为 id，entry 是在符号表中的入口。
      - `mkleaf(num, val)`：建立一个树节点，名字是 num，val 用来存放值。
  
    - 每个 parsing tree 的节点都有一个指针，指向一个上面所述节点。看图就知道了：
  
      ![image-20211113191617428](.\..\..\typora-user-images\image-20211113191617428.png)

## S-attributed SDD 的 bottom-up 计算

S-attributed SDD：只有 synthesized attribute。

一遍扫描的bottom-up计算。

在 LR parsing 时维护3个栈：状态栈，符号栈，属性栈。

## L-attributed SDD 的 top-down 计算

L-attributed SDD：对每个产生式 A→X1X2...Xn，对应semantic rule中的attribute，要不是synthesized attribute，要不是Xi的一个inherited attribute，并且仅依赖于【Xi左边符号X1X2…Xi-1的属性】和【A的inherited attribute】。

一遍扫描的top-down（LL parsing）计算。

### translate scheme

语义规则 & 翻译模式：

- 语义规则，semantic rule：给出了属性计算的定义，没有计算次序等实现细节。
- 翻译模式，translate scheme：给出使用语义规则进行计算的次序等实现细节。

建立翻译模式：

- 仅综合属性时，为每一个产生式，把【语义规则说明的赋值动作】放在产生式末尾。就是直接把 semantic rule 贴到产生式后面。
- 有继承属性时，我要算 A→BC，BC的 inherited attribute 需要已经算出来；A不能引用BC的 synthesized attrbute（本来希望属性值来源于你，你却要引用，这样不行）；只有被引用的所有 attribute 都算出来，这一个 semantic rule 才能执行。
- 但我并不知道怎么建立。

消除翻译模式中的左递归：

- 魔改semantic rule，详见mooc 15讲 ppt页29。

### recursive-descent parser

递归下降分析器。

分析程序由一组递归子程序（函数）组成，每个非终结符对应一个子程序（函数）。

设计：

- 对每个非终结符A，构造一个函数。
- A的继承属性：形参；综合属性：返回值。
- A的产生式中出现的属性：函数里的局部变量。

但仍然不知道怎么做。





















