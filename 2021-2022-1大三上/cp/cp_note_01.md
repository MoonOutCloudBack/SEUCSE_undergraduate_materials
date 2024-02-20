# cp 01 introduction

## 课程介绍

如何学习编译原理：

- 听课。自学很困难；
- 刷题。练习是非常重要的。好多概念可能看着简单，其实考的奇难无比。
- 和同学讨论。不要一个人闷头看书。

### 分数占比

作业：7-8次，10%。

实验：2个，20%。

期末考试：70%。

## what is compilation

转换，从高级预言转换成汇编语言。

高级语言->中间代码（三地址码）->汇编代码。

### 两个阶段

analysis phrase：分析。

- lexical analyzer：词法分析，分析每一个词，每一个词的性质。
- syntax analyzer：语法分析，是不是符合语法规则。此时得到了语法树。（符号表：symbol table）
- sematic analyzer：语义分析，是否符合上下文环境，比如相加的两个东西类型是否一致。

synthesis phrase：合成。

- intermediate code generator
- code optimizer
- code generator

### lexical analysis

识别单词类型是什么。

- 关键词，keywords。
- 标识符，identifier，（好像就是变量名）。
- 运算符，operator。
- 常量，constant。
- 分隔符，separators，空格tab换行。
- 界限符，delimiters，逗号冒号括号等。

然后我们把它们放在表里。

### syntax analysis

得到层次化的结构：语法树，syntax tree。

![image-20210805103817801](.\..\..\typora-user-images\image-20210805103817801.png)

### sematic analysis

是否可行，是否能做。类型检查是重要的一部分。

### intermediate code generator

转换成中间代码。一条指令只有3个地址，和低级语言比较接近。

### code optimizer

效率更高的等价代码。

### code generator

生成汇编代码。

## 课程框架

- 语言和文法，programming language & grammar definition。什么是文法/语言，语法树，语言和文法的互相转换。4种类型的文法，对应的识别模型。
- 词法分析，lexical analysis：regular grammer， finite automation lex。
- 语法分析，syntactic analysis：context-free grammer and pish-down automation，LL(1,top down) grammer，LR(botton up) grammer，使用YACC。
- 中间代码生成：intermediate code generation and suntax-directed translation，三地址码，scanner，翻译的模式。
- type checking and run-time environment，分配内存等。
- 代码优化：code optimization，基本块内的优化（block optimization），循环优化（loop optimization），global optimization。
- 目标代码生成：target code generation。

















