# di 01 线性系统基本概念

数学基础 & 信号与系统。

## 数学基础

### 采样函数

$$
f(n)=\sum_{\tau}f(\tau)\delta(n-\tau)
\\
f(t)=\int f(\tau)\delta(t-\tau)d\tau
$$

### 正交基 

#### 函数内积

$$
\langle f(t),g(t)\rangle=\int f(t)\cdot g^*(t)dt
$$

若$\lang f(t),g(t)\rang=0$​​​，则f和g正交。星号是共轭。

#### 正交基

一组基函数$f_1(t),f_2(t),\cdots$​​，满足

$$
\lang f_m(t),f_n(t)\rang =\left\{
\begin{aligned}
1 && (m=n)
\\
0 && (m\ne n)
\end{aligned}
\right.
$$

则这组基函数就是正交基。感觉一堆δ函数就是正交基。

$f_k(t)=e^{jk\omega t},(k=0,\pm 1,\pm 2)$​是常见的正交基。（是说在一个基波周期上积分的值？）

#### 正交展开

把一个函数表示为正交基函数的线性叠加，即
$$
g(t)=\sum_k A_kf_k(t)
$$
求展开系数$A_n$​：
$$
\lang g(t),f_n(t)\rang=\lang \sum_k A_kf_k(t),f_n(t)\rang
\\
=\sum_kA_k\lang f_k(t),f_n(t)\rang=A_n
$$

## 信号与系统

### 系统

从激励到响应。（函数的函数：泛函）

$x(t)$，$f(*)$，$y(t)=f(x(t))$。

我们想：已知f，从x求y；希望从x到y时，设计f。

#### 线性系统

线性是【系统】的特性。

线性系统定义 -> 单位冲激响应/卷积：时间域方法。

特征函数 -> 系统传递函数：频率域方法。

最终要设计线性滤波器。

#### 时域方法：单位冲激响应 & 卷积

单位冲激响应：$h(n)=f(\delta(n))$。

又$x(n)=\sum_{\tau}x(\tau)\delta(n-\tau)$，

则
$$
y(n)=f(x(n))=f(\sum_{\tau}x(\tau)\delta(n-\tau))
\\
=\sum_{\tau}x(\tau)f(\delta(n-\tau))
\\
=\sum_{\tau}x(\tau)h(n-\tau)
$$

卷积：
$$
y(t)=\int x(\tau)h(t-\tau)d\tau=x(t)*h(t)
\\
y(n)=\sum_{\tau}x(\tau)h(n-\tau)=x(n)*h(n)
$$
响应=【激励】与【单位冲激响应】的卷积。

性质：

- 交换律，结合律（级联的线性系统可以等效为一个线性系统）。
- 微分：$\frac{d}{dt}[h(t)*f(t)]=\frac{dh(t)}{dt}*f(t)=h(t)*\frac{df(t)}{dt}$​。
- 离散序列的差分也是如此吧。

#### 频域方法：特征函数

特征函数，eigenfunction，$x(t)=e^{j\omega t}$：
$$
y(t)=h(t)*x(t)=\int h(\tau)x(t-\tau)d\tau
\\
=\int e^{j\omega(t-\tau)}h(\tau)d\tau
=e^{j\omega t}\int_{-\infin}^{\infin} e^{-j\omega \tau}h(\tau)d\tau=x(t)\cdot\lambda
\\
\lambda(\omega)=\int_{-\infin}^{\infin} e^{-j\omega \tau}h(\tau)d\tau
$$
传递函数λ只与ω有关，是一个复常数。

当$x(t)=a_1e^{j\omega_1t}+a_2e^{j\omega_2t}+a_3e^{j\omega_3t}$时，

$y(t)=a_1\lambda(\omega_1)e^{j\omega_1t}+a_2\lambda(\omega_2)e^{j\omega_2t}+a_3\lambda(\omega_3)e^{j\omega_3t}$​，无需再做卷积运算。

### 信号

![image-20210809115109634](.\..\..\typora-user-images\image-20210809115109634.png)

#### 傅里叶级数，fourier series

周期信号$x(t)$，周期是T。

定义一组基函数：$e^{jn\omega_0t},(n=0,1,2,\cdots)$。其中$\omega_0=\frac{2\pi}{T}$。

$x(t)$可以在此基上展开为：
$$
x(t)=\sum_{n=-\infin}^{\infin}A_ne^{jn\omega_0t}
$$
基函数系数计算：
$$
\lang x(t),e^{jn\omega_0t}\rang=\int_0^T x(t)e^{-jn\omega_0t}dt
\\
=\int_0^T (\sum_{k=-\infin}^{\infin}A_ke^{jk\omega_0t})e^{-jn\omega_0t}dt
=T\cdot A_n
\\
A_n=\frac1T \lang x(t),e^{jn\omega_0t}\rang=\frac1T \int_T x(t)e^{-jn\omega_0t}dt
$$

##### 傅里叶级数的物理意义

给出一个线性电路，想通过实验得到阻抗曲线Z(f)，f是频率。

方案1：“每次输入一个单频率的正弦波电流，测量系统的输出电压波形，由此获得该频率点的阻抗幅度和相位。”就是一个一个点测量，然后描点成线。

方案2：输入20个不同频率的正弦波线性叠加的电流，记录系统的输出电压（包含20个不同频率的正弦波形），通过【傅里叶级数】分解获得【20个不同频率电源正弦波的幅度和相位】，得到10个测量点的阻抗。

#### 傅里叶变换，fourier transform

定义连续函数$X(\omega)=\int_{-\infin}^{+\infin}x(t)e^{-j\omega t}dt$​。​（注意与上方$A_n$表达式做对比）

（如果是非周期函数，积分区间就是负无穷到正无穷，否则是一个周期。

$A_n$是$X(\omega)$的采样，$A_n=\frac{1}{T}X(n\omega_0)$。

从【周期函数傅里叶级数】到【傅里叶变换】：
$$
x(t)=\sum_{n=-\infin}^{+\infin}A_ne^{jn\omega_0t}
=\sum_n\frac1T X(n\omega_0)e^{jn\omega_0t}
\\
=\sum_n\frac{\omega_0}{2\pi} X(n\omega_0)e^{jn\omega_0t}
$$
傅里叶变换：
$$
x(t)=\frac{1}{2\pi}\int_{-\infin}^{+\infin}
X(\omega)e^{j\omega t}d\omega
\\
X(\omega)=\int_{-\infin}^{+\infin}x(t)e^{-j\omega t}dt
$$
X(ω)：频率特性。傅里叶变换是【时域】与【频域】间的转换。

|X(ω)|：幅频特性（频谱）。φ(ω)：相频特性。

| 时间域               | 频率域                             |
| -------------------- | ---------------------------------- |
| 周期连续，如余弦函数 | 非周期离散，偶对称的两个冲激函数   |
| 非周期连续           | 非周期连续，频域连续因为时域非周期 |
| 周期离散             | 周期离散，频域周期因为时域离散     |

实信号的频谱：

频率特性X(ω)共轭对称，因为不同频率分量的虚部要两两抵消。（所以画频谱时经常只花x＞0的部分）

##### 傅里叶变换的性质

线性：$ax(t)+by(t)\rightarrow aX(\omega)+bY(\omega)$​；

时延：$x(t-\tau)\rightarrow X(\omega)e^{-j\omega\tau}$​；​

相移：$x(t)e^{j\omega_0t}\rightarrow X(\omega-\omega_0)$​；

微分：$\frac{d^nx(t)}{dt^n}\rightarrow (j\omega)^nX(\omega)$​；​​

卷积：$x(t)*y(t)\rightarrow X(\omega)Y(\omega)$​；时域卷积频域相乘。

#####  傅里叶变换与传递函数

传递函数（transfer function）：$\lambda=\int h(\tau)e^{-j\omega\tau}d\tau$；

满足$x(t)=Ae^{j\omega t}$，$y(t)=\lambda(\omega) x(t)$；

事实上，$\lambda=F\{h(t)\}$​，就是单位冲激响应的傅里叶变换。

#### 线性滤波器

分类：低通、高通、带通、带阻。

指标：

- 截止频率（频谱下降到【-3db】即【0.7、根号2分之一】的频率）（考虑物理可实现性，认为最大频谱为1）。
- 过渡带：截止频率接近的一部分频率，没有说怎么定义的。

理想滤波器的截止频率ω0很重要，但在数字图像处理中，其实并不能准确地定出来ω0，因此过渡平缓的滤波器更常用。

#### 离散傅里叶变换

对周期为T的连续信号x进行采样：$x(n)=x(\frac{nT}{N})$​​，即一个周期内采样N个点。
$$
X(k)=\frac1N\sum_{n=0}^{N-1}x(n)e^{-j2\pi\frac{nk}N}
\\
x(n)=\sum_{k=0}^{N-1}X(k)e^{j2\pi\frac{nk}N}
$$
对比傅里叶级数：
$$
A_k=\frac1T\int_{0}^{T}x(t)e^{-jn\omega_0t}dt
\\
x(n)=\sum_{k=-\infin}^{\infin}A_ke^{jk\omega_0t}
$$

#### 拉普拉斯变换，laplacian transform

$$
X(\sigma,\omega)=\int_{-\infin}^{+\infin}x(t)e^{-\sigma t}e^{-j\omega t}dt
\\
X(s)=\int_{-\infin}^{+\infin}x(t)e^{-st}dt
\\
s=\sigma+j\omega
\\
L\{x(t)\}=F\{x(t)e^{-\sigma t}\},~
L\{x(t)\}|_{\sigma=0}=F\{x(t)\}
$$

σ=0的拉普拉斯变换是连续傅里叶变换。

#### Z变换，z transform

$$
X(\sigma,\omega)=\sum_{n=-\infin}^{\infin}x(n)e^{-\sigma n}e^{-j\omega n}
\\
X(z)=\sum_{n=-\infin}^{\infin}x(n)z^{-n}
\\
z=e^{\sigma+j\omega}
\\
X(e^{\sigma+j\omega})|_{\sigma=0}=X(\omega)
$$

单位圆上的z变换是离散傅里叶变换。







作业：

两个实序列$x(n)y(n)$，长度相等。仅调用FFT模块一次，完成两个序列的DFT，即得到$X(k)Y(k)$。



解答：

构造$q(n)=x(n)+jy(n)$，对$q(n)$进行FFT得到$Q(k)$，即$Q(k)=X(k)+jY(k)$。

$Q(k)$的共轭对称为$Q^*(N-k)$，也是$x(n)-jy(n)$的DFT，即$Q^*(N-k)=X(k)-jY(k)$。

由此可得：

$$
X(k)=\frac{Q(k)+Q^*(N-k)}{2}
\\
Y(k)=\frac{Q(k)-Q^*(N-k))}{2j}
$$













