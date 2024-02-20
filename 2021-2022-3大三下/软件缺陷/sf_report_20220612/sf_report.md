## 主要思路

最初动机：想用 DL（尤其是 RL）做 infix。

- infix 在做什么？假设一个程序是正确的，给定报错的输入，通过 5 种 templates 5 种 mutation，将其变成正确的。
- RL 怎么用：
  - observation：我的 state（input）+ 外部 env（program + 报错）。
    - program 用 transformer。（可能要稍微说说 transformer）
    - 报错硬编码，one-hot。
    - state（input）也用 transformer。
  - action：
    - 在某一行末尾 添加 & 删除 token。
    - 在末尾 添加一行 删除一行。
    - 将某一行某一 token 变成 -1 到 10 的 int。
    - 将某一行某一 token 变成 -1 到 10 的 float。
  - reward & done：-1 仍报错 0 正确 吧。
  - 训练：RL + behavior cloning + self imitation。

框架：

- RLInFix: A reinforcement learning approach for Automatically Repairing Beginners' Program Inputs
- intro。
- 介绍 RL，1. RL 在干什么，2. liter review
- 介绍 infix。
- method 给出怎么做，画炫酷图。
- possible limitation：容易 “自我放弃” （第一步全清空，第二步加0），语义信息失去。
- conclusion。

















## abstract

RLInFix 被 infix 启发，也是专注于 fix input。

This paper presents RLInFix, a framework for automatically fixing program inputs for beginner programmers using Reinforcement Learning and Transformer. RLInFix is inspired by InFix[1], which also repairs the input data to make programs run successfully, instead of focusing on the way of debugging program codes as most works do.

用 RL 因为整个迭代 fix 过程跟 RL 很像，RL 的 action 被 infix 的 5 templates 5 启发，并做了 generalization 6 种带参的通用 action，RL 的 reward 用是否 correct 来表征。

Our framework is based on Reinforcement Learning (RL), because the whole iterative repairing process is very similar to a typical RL trajectory. Inspired by the five templates and five mutations for input repairing in InFix[1],  six parameterized actions are used in our framework, which is strong and generalizable. The reward for RL is determined by the correctness of the input.

用 transformer 因为程序具有 context 和 自然语言很像。

The popular Transformer model is used to encode the program and error messages, for programs are similar to natural language which has context and needs the "attention" of human.

我们提出了完整的框架，旨在启发软件故障研究的思路，但没有给出代码实现，鼓励之后的工作做代码实现。

This novel framework is proposed to broaden the idea of software fault research, as well as provide methodology for related code implementation. Thus, implementation for RLInFix is not provided in this work.

关键词。

keywords: software faults, input repair, Reinforcement Learning, Transforme

## intro

初学者的挫败。有意义。

尽管在线教育资源越来越丰富，各种编程语言的初学者，仍然经常在入门过程中感到挫败。debug 单个错误的时间和 beginners 的挫败感成正相关。目前的大部分工作都 focus on 如何 debug user 的代码。然而，拿 python 语言举例，根据 python tutor 的数据，a surprisingly-large portion are input-related (e.g., entering 1,2 instead of 1.2) 。因此，从 input 角度入手，为 beginners 的 debuging 过程 提供修改建议或 hints，对于帮助编程技术普及，是很有意义的。

Though online education resources are becoming more and more abundant, beginners of various programming languages still often feel frustrated in the process of getting started. The time of debugging a single error is positively related to the frustration of beginners. Currently, most of the current works focus on how to debug user's code. However, take Python as an example: according to the data from Python Tutor\cite{pythontutor}, a surprisingly large proportion of errors is input related (e.g., entering 1,2 instead of 1.2). Therefore, for the popularity of programming technique, it is significant to provide support or hints for the beginners' debugging process, from the perspective of input.

InFix 一系列有意义的探索。

xx 等人在这个方向做了一系列有意义的探索：

Endres et al. made some meaningful exploration in this direction. They present InFix, a randomized template-based approach for automatically fixing erroneous inputs for beginner programmers. Taking advantage of input error patterns, InFix can iteratively repair the inputs. InFix was evaluated on 25,995 unique input-related scenarios and repaired 94.5\% of input errors with average time of 1.2s per input.

### 文献综述

我们尝试用 ML RL 来 repair program inputs，希望 ML 的强大能力能带来更好的效果、更强的泛化性。在 softward fault localization and repair 方面的 数据挖掘、机器学习 工作从 2010 年开始变多 \cite{MLreview}，常用的技术有 决策树 (DT)，朴素贝叶斯(NB)，支持向量机（SVM）等。进两年来，深度学习技术开始应用到 softward fault localization and repair \cite{MLreview}，常用的技术有 卷积神经网络(CNN)，多层感知器（MLP），长短期记忆(LSTM) 等。

We try to utilize Machine Learning and Reinforcement Learning to repair program inputs, hoping they can lead to better results and generalization. Since 2010, in software fault localization and repair, Data Mining and Machine Learning works increase, and commonly-used technologies include Decision Tree, Naive Bayes, Support Vector Machine, etc\cite{MLreview}. In the past two years, Deep Learning techniques have been applied to software fault localization and repair\cite{MLreview}. The commonly used technologies include Convolutional Neural Network (CNN), Multi-Layer Perceptron (MLP), Long Short-Term Memory (LSTM), etc.

这些工作的思路大多为 tokenization 和 neural embedding \cite{MLreview}。通用的机器学习、深度学习技术 相当于更强大的函数拟合器，因此很适合做这些工作，而 RL 则是提出了一套与环境交互、解决困难任务的 framework，需要任务的结构契合 RL framework \cite{MLreview2}。也因此，RL 的工作数量非常少。

Most of these works base on the idea of tokenization or neural embedding\cite{MLreview}, where Machine Learning and Deep Learning technologies act like powerful function fitters. Thus, Machine Learning and Deep Learning are suitable for these tasks. Nevertheless, Reinforcement Learning is a framework for interacting with the environment and solving difficult tasks, which requires corresponding task structures\cite{mlreview2}. Therefore, Reinforcement Learning works in software fault localization and repair are rarely observed.

我们找到了 两类 RL 工作的典型案例，都在软件测试领域。其中一种\cite{RLandroid2018}\cite{RLandroid2020} 用好奇心驱动的 RL 建模测试者（agent）和应用程序（env）的交互，有效提高了测试的代码覆盖率。另一种 \cite{RLtestcase2017}\cite{RLtestcase2020} 使用 RL 对软件测试的 test cases 进行长时间周期内的自动排序，将 历史执行信息 作为 obs，使用 RL 的奖励优化机制 不断改善排序算法。

We found two typical cases of Reinforcement Learning work, both in the field of software testing. One of them \cite{RLandroid2018}\cite{RLandroid2020} use curiosity driven Reinforcement Learning to model the interaction between the tester (agent) and the application (environment), effectively improving the code coverage of the test. The other one \cite{RLtestcase2017}\cite{RLtestcase2020} use Reinforcement Learning to automatically sort the test cases of software testing over a long period of time, take the historical execution information as the observation, and uses reward mechanism of Reinforcement Learning to continuously optimize the sorting algorithm.

然而，没有工作尝试用 ML RL 来 repair program，尤其是 repair program inputs。

However, no work is found to utilize Machine Learning and Reinforcement Learning to repair program, especially to repair program inputs.









我们 present RLInFix，1. RL 建模，2. action，3. transformer。

This paper presents RLInFix, a Reinforcement Learning framework for automatically fixing erroneous program inputs for beginners. RLInFix repairs input data rather than source code, requires no test cases, and requires no special annotations. Based on the work of InFix\cite{infix}, we 1. model the iterative fixing process of InFix as an RL process, 2. take advantage of beginners' input patterns to automatically create powerful and generalizable input repairs, and 3. use Transformer to efficiently encode both the program codes and error messages. 



我们的贡献：1. 提出 RLInFix 框架，2. 对 Python 语言给出了比较具体的实现思路。但这个框架并不仅适用于 Python，action pattern 和 transformer encoding 思想是通用的，只不过在 python 的合理性已经由 InFix 证明了。我们希望能启发思路，因此没有给代码实现，也没有做实验。

In summary, our main contributions are as follows:

\item Propose RLInFix framework, 

\item Provide a more specific implementation idea of RLInFix for Python.

Note that this framework is not only applicable to python. The ideas of the action patterns and the Transformer encoding are universal, and the rationality of Python has been proved by InFix\cite{infix}. Our novel framework is proposed to broaden the idea of software fault research, as well as provide methodology for related code implementation. Thus, implementation for RLInFix is not provided in this work.



全文组织如下：sec 2 是一些 preliminaries，包括 infix，RL transformer，sec3 是方法，sec4 是对 possible limitations 的讨论（因为并没有做实验验证），sec 5 是 conclusion。

## preliminaries

### infix

infix 假设程序正确，要把引起错误的输入改对。而不是专注于 debug 程序语句。

With the assumption that the program is correct, InFix\cite{infix} focuses on repairing the inputs that cause errors, rather than debuging program codes as most works do.

观察发现，很多错误输入有相似的原因，可以被总结为 特定pattern。以python为例，根据 python tutor 数据集，int 输入格式 34.42%，pack 9.09%，float 8.62，index 相关 18.56%。

An essential observation is that, many inputs that cause errors have similar reasons, and these reasons can be generalized to specific patterns. Taking Python as an example, according to the dataset from Python Tutor\cite{pythontutor}, the errors related to integer input format take 34.42% in total, the list packing format 9.09%, the float input format 8.62%, and the list index problem 18.56%.

同时，一个错误输出可能包含很多 pattern，需要迭代地 一个一个 fix 它们。

Also, an erroneous input may contain more than one patterns, which need to be fixed iteratively one by one.

因此，提出了一种随机搜索的 fixing algo。ppt 5 英文表述。

Therefore, a fixing algorithm using random search is proposed. For an error input $I$, we first check that if any template $T$ can be fitted. If so, $I ← T(I)$. Else, take a random mutation $M$, that is, $I ← M(I)$. This step is done repeatedly until $I$ goes correct.

它 surprisingly 有效 迅捷，ppt 9 英文表述。

InFix is surprisingly effective. It can successfully repair 94.5% of input-related errors in Python Tutor dataset, with high quality rated by humans. It is also efficient, spending a median time of 0.88s and an average time of 1.23s per error.

然而，它可能丢失 test case 的语义信息。这涉及到 前述 随机搜索迭代 fixing 过程中，correctness 的定义。比如，我们想输入 int 66，但输入成了带引号的 “66”。将 “66” fix 成 66 或者 0 都可以跑通，但只有 前者不会丢失语义信息。

However, InFix may lose the semantics of test cases. This involves the definition of correctness in the random search iterative fixing process. For example, a beginner wants to enter an integer 66, but he enters "66" with quotation marks. Fixing "66" to 66 or fixing it to 0 can be both executed successfully, but only the former one doesn't lose the semantics.

作者提出假设，认为新手只想成功跑通程序，认为问题不大。

The authors of InFix puts forward a related assumption: beginners only want to successfully run the program, and they do not care about the semantics.

### RL

RL 是一种 general idea 或者 framework，where agent 通过与外界环境交互 学习完成任务。具体说，agent 得到当前 env 的 obs，根据 obs 对env做出 action，得到 env 的反馈 reward 和更新后的 obs。

Reinforcement Learning (RL) is a kind of general idea or framework, where an agent learns to complete tasks through interaction with the external environment. Specifically, in each round of interaction, the agent obtains the observation of the current environment, takes an action according to the observation, and obtains the feedback reward of the environment and the updated observation.

agent 一直与 env 交互，直到任务成功完成 或 确定失败，这段过程称为一个 episode。RL 的目标函数是 最大化整个 episode 中 agent 获得的 reward 之和。

The agent interacts with the environment, until we can make sure the task is completed successfully or it fails. This process is called an episode. The objective function of Reinforcement Learning is to maximize the total rewards in the whole episode.



### transformer

transformer 是一种神经网络结构，最早由 Vaswani 等人提出\cite{attention}，适用于建模序列。因为其独特的注意力机制，符合人们理解长段文本、理解图像的机理，因此被广泛应用在 计算机视觉 和 自然语言处理中。

Transformer is a kind ofneural network structure, which was first proposed by Vaswani et al. \cite{attention}, and is applied to sequence modeling. It is widely used in Computer Vision and Natural Language Processing because of its unique attention mechanism, which is similar to the mechanism of people understanding long texts as well as images.

## method

### RL framework

The RLInFix framework is based on Reinforcement Learning (RL), because the whole iterative repairing process is very similar to a typical RL trajectory. The basic elements of RL are as follows:

Environment:

我们需要把整个系统划分为 env 和 agent。在 RL 中，一般来说，我们能掌控、命令它做出动作的 被划分为 agent，其余我们未知、并且可能具有随机性的部分为 env。在 RLInFix，程序、编译器/解释器被视为 env，编译器 / 解释器输出的结果（如 error message）是环境给我们的反馈。

The whole system is needed to be divided into environment and agent. Generally, in RL, the part we can control through commands is divided into agents, and the other part we do not know and may work with randomness is the environment. In RLInFix, programs and compilers or interpreters are regarded as the environment. The output of compilers or interpreters (such as error messages) is feedback from the environment.

Agent:

在 RLInFix 中，user inputs被视为 agent。我们需要根据 env 的反馈，不断改变 input，以求尽快让程序 run successfully。

In RLInFix, the part of user inputs is treated as agents. The action we take is to change the inputs according to environment feedback, in order to make the program run successfully as soon as possible.

Observation:

当我们 humans 在修改 错误 input 时，我们会读源代码、当前的 input、error message。RLInFix 的 obs 也同样包含这几部分。因为 我们 humans 读 programs 时也会应用到注意力机制，比如观察一个变量被哪几段 code 影响，根据 error message 精准定位到 the buggy line of code。因此，我们采用 Transformer 模块来编码 obs。Section \ref{sec:encoder} 有以 python 为例的详细介绍。

When we humans are modifying the error input, we will read through the source code, current input, and error message. The observation of RLInFix also includes these parts. Note that we humans also apply some kinds of attention mechanisms when reading programs, such as tracing which paragraphs of codes affect a variable, and locating the buggy line of code according to error messages. Therefore, the Transformer architecture is used to encode the observation. Section \ref{sec:encoder} provides a detailed description in terms of repairing beginners' Python inputs.

Action:

被 InFix\cite{infix} 中对 input 实施的 5种templates 5种mutations启发，我们定义了 6 个参数化的 action，用来微改 当前 inputs。Section \ref{sec:action} 有以 python 为例的详细介绍。

Inspired by the 5 templates and 5 mutations in InFix\cite{infix}, we define 6 parameterized actions to modify the current inputs. Section \ref{sec:action} provides a detailed description in terms of repairing beginners' Python inputs.

Reward:

RL 的目标函数是 最大化整个episode 的 reward，因此，如果当前 input 不 correct，则令 reward 为一个 -1 的 penalty，否则为 0。

The objective of RL is to maximize the total reward for the whole episode. Therefore, if the current input is not correct, we give the agent a penalty of -1 as the reward. Otherwise, the reward is 0.

Done (episode end):

当 input 是 correct 时，episode 结束（同时 reward 也 =0）。

When the current input is correct, the episode ends (and the reward is 0).

### state encoder using transformer

我们想通过 attention model 来提取和 error message 有关的信息，包括 error 发生的位置，error 的类型和详细信息，哪些输入引起了 error，引起 error 的相关变量在前面的 program 被如何处理。在 python 场景下，一条典型的 error message 如下所示：

The Attention model is utilized to extract the information related to the error, including 1. the location of error occurence, 2. the type and details of the error, 3. which inputs caused the error, and 4. how the error-relevant variables were processed in the previous program sentences.

In the Python scenario, a typical error message is as follows:

```
Traceback (most recent call last):
    File "E:\path\to\the\program\codes.py", line 11
ValueError: invalid literal for int() with base 10: ’3.3’
```

It includes 1. the line number of the error, 2. eror type, 3 details of the error, which can be all extracted through a hard-coded process. After parsing the error message, the Attention model is used to distill the relationship between the error line of code and the previous program sentences. At the same time, the Attention model is also applied to encode user inputs. The whole procedure is shown in Figure \ref{fig:encoder}.

其中包含了：1. 发生错误的行数，2. 错误类型，3. 错误详细信息。这些信息可以通过硬编码流程提取。解析 error message 后，利用 attention model，追溯 出错 line of code 与之前 program sentences 的关系。同时，也使用 attention model 编码 user inputs。整个流程如下图所示：

【图】

### actions

We define 6 parameterized actions as Tabel \ref{tab:action}.

```
\begin{table}[htbp]
	\centering  % 显示位置为中间
	\caption{6 actions of RLInFix}  % 表格标题
	\label{action}  % 用于索引表格的标签
	% 字母的个数对应列数，|代表分割线
	% l代表左对齐，c代表居中，r代表右对齐
	\begin{tabular}{|c|c|c|}  
		\hline  % 表格的横线
		Action & Description & Parameters\\  % 表格中的内容，用&分开，\\表示下一行
		\hline
		Insert a line & Insert a new line at the end of the input & /  \\
		\hline
		Delete a line & Delete the last line of the input & /  \\
		\hline
		Insert a token & Insert a new token at the end of a specific line of the input & the line number  \\
		\hline
		Delete a token & Delete the last token of a specific line of the input & the line number  \\
		\hline
		Modify a token to integer & Modify a a specific token to a random integer of [-1, 10] of a specific line of the input & the order of the token, the line number  \\
		\hline
		Modify a token to float & Modify a a specific token to a random float of [-1.0, 10.0] of a specific line of the input & the order of the token, the line number  \\
		\hline
	\end{tabular}
\end{table}
```



### training

When training the RL model, 可以采用 behavior cloning 和 self imitation 的 tricks 作为辅助。具体的，我们可以搜集用户成功修改 inputs 的案例，拿来做监督学习（即 behavior cloning），同时让 agent 反复学习自己曾经的成功案例（self imitation）。

When training the RL model, the tricks of behavior cloning and self imitation may help. Specifically, we can collect cases of users' successful repairing processes as dataset of behavior cloning, and also let agents repeatedly learn their own successful cases (self imitation).

## possibie limitations

correctness and the loss of semantics

无论是 infix 还是 rlinfix，语义都可能在 fixing process 丢失。就像 ref{sec:infix} 所说，这跟 correctness 的定义有关。在 correctness 加入与原 input 的相似性判定，或许可以缓解这一情况。

Whether for InFix or RLInFix, semantics may be lost in the fixing process. As \ref{sec:infix} shows, the loss of semantics is related to the definition of correctness. Adding a similarity term to the correctness judgment with the original input may alleviate this kind of loss.

suicide shortcut for the agent

在训练过程中，agent 可能会采取 “自杀” 行为，如把所有 input 一行一行消掉。当 input 为空时，程序可能恰好能正确执行，这鼓励 agent 放弃寻找 pattern，采取自杀的捷径。同样，在 correctness 加入与原 input 的相似性判定，或许可以缓解这一情况。

During the training process, the agent may take some kinds of "suicide" behaviors, such as erasing all input lines one by one. When the input is empty, the program may just run successfully, which encourages the agent to give up mining patterns and take the "suicide" shortcut. Similarly, adding a similarity term to the correctness judgment with the original input may alleviate this kind of loss.

## conclusion

This paper presents RLInFix, a Reinforcement Learning framework for automatically fixing erroneous program inputs for beginners. RLInFix repairs input data rather than source code, requires no test cases, and requires no special annotations. Based on the work of InFix\cite{infix}, we 1. model the iterative fixing process of InFix as an RL process, 2. take advantage of beginners' input patterns to automatically create powerful and generalizable input repairs, and 3. use Transformer to efficiently encode both the program codes and error messages. 







## ref

```
```































