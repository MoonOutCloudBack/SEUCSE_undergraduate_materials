# cp 08 intermediate code generation

中间代码独立于具体语言。

## 常见分类

- 后缀式（逆波兰表示法）：
  - 操作符在操作数后面。
  - 只需要知道操作符的目数（传进多少个参数），不需要括号。
  - 用栈实现。
- 图表示法：
  - 抽象语法树AST。
  - 有向无环图DAG。子树共享利于优化代码。
- 三地址码，three address code。
  - 运算符：x := y op z，x := op y。
  - 直接赋值：x := y。
  - 跳转：goto L，if x relation_op y goto L，if a goto L。
  - 传参、转子：param x，call p,n。
  - 返回语句：return y。
  - 索引赋值：x := y[i]，x[i] := y。
- 三元式：op，arg1，arg2，用语句位置作为结果域（优化困难，因此出现简介三元式，三元式+简介码表表示计算顺序）。复杂语句需要连续两条三元式。比如索引赋值，变址+赋值。
- 四元式 quadruple 是三地址语句的实现。
  - 带有四个域的记录结构：op，arg1，arg2，result。arg12不能颠倒。
  - ![image-20211218174757569](.\..\..\typora-user-images\image-20211218174757569.png)

## 赋值语句的翻译

![image-20211218113318226](.\..\..\typora-user-images\image-20211218113318226.png)

我们应该不需要翻译模式吧，只要自己能somehow写出来结果就ok了。

## 带数组元素引用的赋值语句

Elist 代表（从左到右已经分析完的）index 列表。考虑了 index 多维的情况。

- Elist.ndim：下表个数计数器。
- Elist.place：保存临时变量的名字，这些临时变量存放【已形成的Elist中】的【下标表达式计算出来的值】。
- Elist.array：数组名。

E：表达式。E.type：类型属性。

L：左值？变量

- L.place：简单变量的符号表入口，下标变量的存放【base的临时变量】的名字。（名字和符号表入口混用，因为可以互相检索）
- L.offset：简单变量=null，下标变量：寻址用。

![image-20211218133107442](.\..\..\typora-user-images\image-20211218133107442.png)

生成三地址码：

![image-20211218141021119](.\..\..\typora-user-images\image-20211218141021119.png)

![image-20211218141227199](.\..\..\typora-user-images\image-20211218141227199.png)

![image-20211218141542396](.\..\..\typora-user-images\image-20211218141542396.png)

![image-20211218141738854](.\..\..\typora-user-images\image-20211218141738854.png)

![image-20211218142055085](.\..\..\typora-user-images\image-20211218142055085.png)

### 类型转换

大概就是，判一下两个操作数是不是都是int/real，如果不都是就进行 inttoreal 操作。

## boolean表达式的翻译

### 按数值表示法翻译

![image-20211218140520168](.\..\..\typora-user-images\image-20211218140520168.png)

### 优化

A or B → if A then true else B。

A and B → if A then B else false。

not A → if A then false else true。

表达式新增两个属性：

E.true：E为真跳转到的代码。E.false：E为假跳转到的代码。

![image-20211218170136051](.\..\..\typora-user-images\image-20211218170136051.png)

（三地址码要多遍扫描，四元式可以一遍扫描）

truelist，falselist。同一个链表的四元式要去同一个地方，都需要回填。

这个技术叫做backpatch，要考。

链表的链尾=0。

![image-20211218170856423](.\..\..\typora-user-images\image-20211218170856423.png)

## 控制流语句的翻译

if A then B 和 if A then B else C 都ok。

while A do B：

![image-20211218173630979](.\..\..\typora-user-images\image-20211218173630979.png)

事实上感觉问题不大。

## 下面我们以题代学

### 四元式数组

![image-20211218175238588](.\..\..\typora-user-images\image-20211218175238588.png)

第三步是算出真正的基址base。

### 三地址码数组

题目和上面一样。

```
T1 := I * 20
T2 := J + T1
T3 := a - 21
T4 := T3[T2]
X := T4
```

### 三地址码bool表达式

![image-20211218175409816](.\..\..\typora-user-images\image-20211218175409816.png)

【注意】or 和 and 在一起，要先算 and。

### 三地址码控制流

![image-20211218175438183](.\..\..\typora-user-images\image-20211218175438183.png)

这种赋值要分成两步写，t = x + y，z = t。

### 四元式bool表达式（纯计算）

![image-20211218175752075](.\..\..\typora-user-images\image-20211218175752075.png)

### 四元式bool表达式（short circuit）

![image-20211218175842956](.\..\..\typora-user-images\image-20211218175842956.png)

jnz：若为真就跳转。

### 四元式控制流 backpatch

if then

![image-20211218180458971](.\..\..\typora-user-images\image-20211218180458971.png)

while：

![image-20211218180649555](.\..\..\typora-user-images\image-20211218180649555.png)















