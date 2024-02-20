## 从 langrage 产生 [ε-free] grammar

- 第一步：画状态图，写 grammar (非终结符, 终结符, 文法开始符号, 产生式)。
- 第二步：消除 ε-production。

入门：第二单元补充练习，exer2。

## 从 re 产生 [minimum state] DFA

- re → NFA；
- NFA → DFA：【DFA的新状态】是【NFA的老状态的集合】，注意空串边的状态 closure；
- DFA → reduced DFA：按转移情况划分等价类。

## 对一个 grammar，消除左递归 / 左公共因子，判断是否 LL(1)



## 为 [ambiguous，自定义优先级] grammar 构造LR(1) parsing table



## 证明：如果 G 是 LL(1) grammar，那么 G 是 LR(1)grammar



## 根据 syntax-directed definition，为一个 string 建立 annotated parse tree

标准做法：先画 parsing tree，再逐节点改成 semantic rule。

省力做法：对类似于算术运算的grammar，先算出最终结果，然后直接写。

入门：第五章ppt的两个例题。

## 在活动记录 [第n次] 达到最多时，画出此时的 run-time stack map

- 第一步：画 activation tree，找活动记录 [第n次] 最多的时候。
- 第二步：【未完待续】

## 将一段程序翻译成 TAC（three-address code）



## 将一段程序翻译成 quadruple

