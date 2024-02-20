### 01 数学

线性：叠加性 x+y → f(x)+f(y)，齐次性 ax → af(x)。时不变：x(n-n0) → y(n-n0)。

![image-20211230083355234](.\..\..\typora-user-images\image-20211230083355234.png)

#### 正交基

一组基函数$f_1(t),f_2(t),\cdots$​​，满足（内积 = 1 或 0）

$$
\lang f_m(t),f_n(t)\rang =\left\{
\begin{aligned}
1 && (m=n)
\\
0 && (m\ne n)
\end{aligned}
\right.
$$

则这组基函数就是正交基。比如一堆δ函数，$f_k(t)=e^{jk\omega t},(k=0,\pm 1,\pm 2)$​。（在一个基波周期上积分的值=1？）

#### 正交展开

把一个函数表示为正交基函数的线性叠加，即 $g(t)=\sum_k A_kf_k(t)$。

求展开系数$A_n$：$\lang g(t),f_n(t)\rang=\lang \sum_k A_kf_k(t),f_n(t)\rang
=\sum_kA_k\lang f_k(t),f_n(t)\rang=A_n$ 。

#### 傅里叶级数 & 变换

 $x(t)=\sum_{n=-\infin}^{\infin}A_ne^{jn\omega_0t}$， $A_n=\frac1T \lang x(t),e^{jn\omega_0t}\rang=\frac1T \int_T x(t)e^{-jn\omega_0t}dt$ 。
$$
x(t)=\frac{1}{2\pi}\int_{-\infin}^{+\infin}
X(\omega)e^{j\omega t}d\omega
\\
X(\omega)=\int_{-\infin}^{+\infin}x(t)e^{-j\omega t}dt
$$

| 时间域               | 频率域                                   |
| -------------------- | ---------------------------------------- |
| 周期连续，如余弦函数 | 非周期离散，偶对称的两个冲激函数         |
| 非周期连续           | 非周期连续，频域连续因为时域非周期       |
| 周期离散             | 周期离散，频域周期因为时域离散           |
| 实信号               | X(ω)共轭对称，不同频率分量的虚部两两抵消 |

性质：

- 时延：$x(t-\tau)\rightarrow X(\omega)e^{-j\omega\tau}$​；​

- 相移：$x(t)e^{j\omega_0t}\rightarrow X(\omega-\omega_0)$​；

- 微分：$\frac{d^nx(t)}{dt^n}\rightarrow (j\omega)^nX(\omega)$​；​​

- 卷积：$x(t)*y(t)\rightarrow X(\omega)Y(\omega)$​；时域卷积频域相乘。


传递函数（transfer function）：$\lambda=\int h(\tau)e^{-j\omega\tau}d\tau$，单位冲激响应的FT。

满足$x(t)=Ae^{j\omega t}$，$y(t)=\lambda(\omega) x(t)$。

线性滤波器的截止频率：频谱下降到【-3db】即【0.7、根号2分之一】的频率。

#### 离散傅里叶变换

对周期为T的连续信号x进行采样：$x(n)=x(\frac{nT}{N})$​​，即一个周期内采样N个点。
$$
X(k)=\frac1N\sum_{n=0}^{N-1}x(n)e^{-j2\pi\frac{nk}N}
\\
x(n)=\sum_{k=0}^{N-1}X(k)e^{j2\pi\frac{nk}N}
$$

#### 二维傅里叶变换

从此不再用角频率ω了，用频率u，$ω=2\pi u$。​
$$
F(u,v)=\int\int f(x,y)e^{-j2\pi(ux+vy)}dxdy
\\
f(x,y)=\int\int F(u,v)e^{j2\pi(ux+vy)}dudv
$$

$$
F(u,v)=\frac{1}{MN}\sum_{y=0}^{N-1}\sum_{x=0}^{M-1}
f(x,y)e^{-j2\pi[xu/M+yv/N]}
\\
f(x,y)=\sum_{v=0}^{N-1}\sum_{u=0}^{M-1}
F(u,v)e^{j2\pi[xu/M+yv/N]}
$$

### 03 几何变换 & 插值

几何变换的两个步骤：

- 坐标映射：$\overrightarrow{X'}=T(\overrightarrow X)$，将【原始图像每个像素的坐标】映射到【目标图像的新坐标】。
- 像素赋值：$f(\overrightarrow{X'})\leftarrow f(\overrightarrow{X})$，把【原始图像的像素值】赋给【新坐标处的像素】。

#### 仿射变换

$$
X'=AX
\\
\left[\begin{array}{}
x'\\y'
\end{array}\right]
=
\left[\begin{array}{}
a_{11} & a_{12} & a_{13}\\
a_{21} & a_{22} & a_{23}
\end{array}\right]
\left[\begin{array}{}
x\\y\\1
\end{array}\right]
\\
\left\{\begin{array}{}
x'=a_{11}x+a_{12}y+a_{13}
\\
y'=a_{21}x+a_{22}y+a_{23}
\end{array}\right.
$$

##### 平移，translation

$$
\left\{\begin{array}{}
x'=x+t_x\\
y'=y+t_y
\end{array}\right.
,~
A=\left[\begin{array}{}
1&0&t_x \\
0&1&t_y
\end{array}\right]
$$

##### 缩放，scaling

$$
\left\{\begin{array}{}
x'=s_x\cdot x\\
y'=s_y\cdot y
\end{array}\right.
,~
A=\left[\begin{array}{}
s_x&0&0 \\
0&s_y&0
\end{array}\right]
$$

##### 旋转，rotation

$$
\left\{\begin{array}{}
x'=\cos\theta\cdot x-\sin\theta\cdot y\\
y'=\sin\theta\cdot x+\cos\theta\cdot y
\end{array}\right.
,~
A=\left[\begin{array}{}
\cos\theta & -\sin\theta & 0 \\
\sin\theta & \cos\theta & 0
\end{array}\right]
$$

旋转的正方向定义为【x轴正方向到y轴正方向】。貌似一般是顺时针，因为y轴正方向指向下。

#### 插值

最近邻插值。双线性插值：

<img src=".\..\..\typora-user-images\image-20210823121337145.png" alt="image-20210823121337145" style="zoom:80%;" />

### 04 图像增强

降噪：理想低通滤波器的振铃现象。

增强整体对比度：

- 直方图均衡，gamma校正。不过，它们非线性，会改变频率信息，因此要在均值滤波之后。将灰度映射安排在最后。
- gamma校正：
  - $s=r^{\gamma}$。
  - 看导数：导数＞1就是灰度范围拉伸。。γ=1时，恒等函数；γ＜1时，低灰度范围拉伸，高灰度范围压缩；γ＞1，低灰度范围压缩，高灰度范围拉伸。
  - <img src=".\..\..\typora-user-images\image-20211025111144746.png" alt="image-20211025111144746" style="zoom: 50%;" />
  - 处理之前要先归一化，[0,256) -> [0,1)，最后再调整到[0,256)。
- 直方图均衡，后面有图。灰度窗调整：分段线性映射。

图像锐化：

- unsharp masking：抽离高频分量，进行增强。也可以反向用来降噪。可以分好多频段，每个频段乘一个系数。
- laplacian：梯度的散度，$\grad\cdot\grad f=\grad^2f=\frac{\part^2 f_x}{\part x^2}+\frac{\part^2 f_y}{\part y^2}$。DFT：$-(u^2+v^2)F(u,v)$。拉普拉斯过零点，可以作为精确边界。把二阶导数乘一个＜0 的权重，加到原来函数上去，就是unsharp masking。
- 同态滤波器，retinex，整体对比度增强。I → log 变成 log L+log R → unsharp masking → exp → I hat。对log结果进行高通滤波。

### 05 图像复原

两个影响图像的因素：降质函数 + 噪声。

线性模糊模型：搞一个用来图像复原的卷积核。
$$
g(x,y)=h(x,y)*f(x,y)+\eta(x,y)
\\
G(u,v)=H(u,v)\cdot F(u,v)+N(u,v)
\\
\hat f(x,y)=\hat h(x,y)*g(x,y)=\hat h(x,y)*[h(x,y)*f(x,y)+\eta(x,y)]
\\
\hat F(u,v)=\hat H(u,v)\cdot G(u,v)=\hat H(u,v)\cdot[H(u,v)\cdot F(u,v)+N(u,v)]
$$

#### 逆滤波

观察频域等式，希望 $\hat H$ 和 $H$ 乘积为1。得到 $\hat H(u,v)=\frac 1{H(u,v)}$。
$$
\hat F(u,v)=F(u,v)+\frac{N(u,v)}{H(u,v)}
$$
截止的逆滤波：由于降质卷积核一般是低通滤波，所以频率大到一定程度就 分母→0了，所以我们只对频率小（u²+v²≤R²）的分量进行逆滤波。

#### 维纳滤波，wiener filter

是线性滤波器。

希望： $E\{|f(x,y)-\hat f(x,y)|^2\} \rarr \min$ 。

有：
$$
\hat F(u,v)=\frac{H^*(u,v)}{|H(u,v)|^2+S_n(u,v)/S_f(u,v)}G(u,v)
\\
S_n(u,v)=|N(u,v)|^2, S_f(u,v)=|F(u,v)|^2
$$

- G是实际图像的频谱，H是降值函数的频谱，*是共轭。
- Sn是噪声的功率谱，Sf是信号的功率谱。

如果可以估计出噪声能量和信号能量，就可以计算维纳滤波器的传递函数。

简化：把【噪声功率谱 / 信号功率谱，$S_n(u,v)/S_f(u,v)$】用一个常数 K 来替代，噪声越大K越大。当K=0时，退化成逆滤波。

#### 运动模糊

τ：曝光时间，v：物体运动速度，vτ=a。相机接收到的光线：$g(x)=\int_0^\tau f(x-vt)dt$。某一方向上的均值滤波。

确定方向：

- 引理：直接高通滤波的信号，比先低通滤波再高通滤波的信号，能量更大。
- 我们对各个方向做高通滤波、求信号能量，极小值对应的方向就是运动方向。

确定均值滤波的卷积核大小：

- cannon的解决方法：首先我们进行一波旋转，把运动方向转到x轴。
- 然后对x轴一行一行的像素求和，DFT，用第一个零点（极小点）位置来确定卷积核大小。
- <img src=".\..\..\typora-user-images\image-20211115102850820.png" alt="image-20211115102850820" style="zoom:80%;" />
- 第一个零点的位置是 k，对相位的贡献是 $\pi$ 。因为$H(u)=\sin(\pi ua)/\pi ua$，所以$a=1/k$。

### 06 图像压缩

冗余：编码冗余、像素相关性冗余、视觉冗余。

评价指标：

- 压缩比。$C_R=n_{img}/n_{cod}$，$n_{img}$ 图像数据量，$n_{cod}$ 编码后数据量，压缩比＞1。

- 压缩误差评估：均方误差（rms）：

- $$
  e_{rms}=\bigg[\frac 1{MN}\sum_{x=0}^{M-1}\sum_{y=0}^{N-1}
  [\hat f(x,y)-f(x,y)]^2\bigg]^{1/2}
  $$

- 信噪比：

- $$
  SNR=\frac{\sum\sum[\hat f(x,y)]^2}{[\hat f(x,y)-f(x,y)]^2}
  $$

编码：

- 香农第一定理（Shannon's 1st theorem）：编码最小平均字长，熵（entropy）：$H=-\sum_r p(r)\log_2p(r)$。
- Huffman（熵编码），对bit错误很敏感，一个bit错误/丢失造成全局错误。
- 行程编码，(5,3) (3,2) (4,1) (3,1) (2,5) → 5, 3, 3, 2, 4, 1, 3, 1, 2, 5

#### JPEG

- 预处理：

  - 颜色空间变换：用一个线性变换，将 RGB 转换为 Y Cb Cr（亮度+色差）。兼容灰度图像，Y=灰度，不存在Cb Cr。

    ![image-20211229200328707](.\..\..\typora-user-images\image-20211229200328707.png)

  - 降采样：亮度维持原分辨率，两个色差通道进行 4*4 的降采样【失真】【通过降采样大小调节压缩比】，将3个通道的数据作为各自独立的单通道图像对待。

  - 分块：将图像整齐切割成 8*8 的小块。

- DCT变换：

  - 对每一个小块，进行离散余弦变换。

    ![image-20211229222819755](.\..\..\typora-user-images\image-20211229222819755.png)

- 量化：

  - 通过品质因子调整量化矩阵【调节压缩比】，对每个小块应用量化矩阵（小块对应位置值 / 矩阵对应位置值）【失真】，每个通道的量化矩阵不同。

- 直流分量（DC）编码：

  - 差分预测编码：用差分的思想记录数值，每条记录的格式为 (size(4bit), diff) 二元组，size 表示 diff（该数值与前一数值的差）的位数。
  - 类似 huffman 的编码：对每条记录进行编码，编码表（将 size 进行编码）根据统计数据中不同 size 出现频率、用 huffman tree 生成。

- 交流分量（AC）编码：

  - zig-zag 行程编码：将每个小块剩下 63 个数据按 zig-zag 顺序编排后进行行程编码，每条记录的格式为 (cnt of 0(4bit), size(4bit), diff value) 三元组，cnt of 0 表示该数值后面紧跟0的个数，(size(4bit), diff value) 表示第一个非0像素与该数值的差分。
  - 类似 huffman 的编码：对每条记录进行编码，编码表（将 (cnt of 0, size) 进行编码）同样根据统计数据生成。

### 07 图像分割

特征提取（类间差异 类内相似）+ 分类器（输入特征 输出标签）

特征：灰度特征、边界特征，大概就是灰度域的卷积结果。

分类器：

- 阈值法。确定阈值：
  - 正态分布拟合得交点，谷点阈值。
  - 大津方差：最小化 $\sigma(T)=P_A(T)\sigma_A(T)+P_B(T)\sigma_B(T)$。
  - 最大熵阈值：最大化 $\Psi(T)=H_A(T)+H_B(T)$，$H(T)=-\int _{-\infin}^{\infin}p(t)\ln(p(t))dt$。
- 区域增长。

背景光不均匀：乘性背景光：retinex，取对数 → 高通滤波 → exp。加性背景光：直接高通滤波，用原图减去频率特别低的分量。

形态学补洞，得到完整区域。

### 08 二值图像

d4 = |Δx| + |Δy|。d8 = max(Δx, Δy)。前景背景距离，一个d4，一个d8。

连通，connectness：距离=1。

#### 二值图像运算

非线性，不可逆。

- 腐蚀，erosion $\ominus$ 。扩张，dilation $\oplus$​ 。
- 开运算，opening operator $\circ$ ，$X\circ B=(X\ominus B)\oplus B$，先腐蚀再扩张。光滑目标轮廓，去掉毛刺和孤立点，在纤细点处分离物体。
- 闭运算，closing operator $\bullet$ ，$X\bullet B=(X\oplus B)\ominus B$，先扩张再腐蚀。填充凹陷、弥合孔洞和裂缝。
- 边界提取：$\part X=X-(X\ominus B)$，B是 3*3 填满的卷积核（d8的边界）十字架型的卷积核（d4的边界）。我减去我的腐蚀（腐蚀：把前景刨掉一圈）。

#### 区域和边界

- 区域填充，迭代：

  - $Y_0=P$，P是在前景区域布的种子。
  - $Y_k=(Y_{k-1}\oplus B)\cap X^c,~k=1,2,3,...$ 
  - 当 $Y_k=Y_{k-1}$ 时，terminate，输出 $Y_k\cup X$。
  - （X是边界，$X^c$是X的补集，B是 d4/d8 的卷积核，与运算是把掉到边界里点抹掉。）

- 链码 & 边界跟踪，一步一步沿着边界走，最后会走到起始点。

  - 方向编码，链码 chain code。就像这样（d8的方向编码）：
    $$
    \left[\begin{array}{}
    3 & 2 & 1 \\
    4 & P & 0 \\
    5 & 6 & 7
    \end{array}\right]
    $$

  - 初始化：最东面的边界点，方向朝 0；

  - 依次判断 右前、前、左前 是否存在前景点。

    - 是： 前进到下一边界点，修改初始方向为小于等于前进方向的偶数编号（就是不改变面朝的方向，面朝方向为东西南北之一）。
    - 否： 初始方向+=2，逆时针转 90°。

  - 结束条件： 回到起始点 / 同一点连续修改初始方向4次（原地转一圈，发现自己在孤岛）。

- 距离图 & 距离变换，用距离变换生成距离图。

  - 按照光栅扫描顺序（从左到右一行一行从上到下，像写字一样）。
  - 该像素是背景：0。
  - 该像素是前景：
    - d4：min(上方像素值, 左方像素值) + 1。
    - d8：min(上方, 左上方, 左方) + 1。
  - 然后我们按照逆光栅扫描顺序（就是逆过来），再来一遍。值 = min(老值, 逆光栅的新值)。
  - 应用：截取目标的骨架，即距离（在某个截线方向）的极大值点。

### 09 彩色图像

<img src=".\..\..\typora-user-images\image-20211225144338718.png" alt="image-20211225144338718" style="zoom: 67%;" />

Hue 色调，Saturation 饱和度，Value 值。

<img src=".\..\..\typora-user-images\image-20211225144403620.png" alt="image-20211225144403620" style="zoom:50%;" />

还有HSL（Hue Saturation Lightness）。

<img src=".\..\..\typora-user-images\image-20211225144611976.png" alt="image-20211225144611976" style="zoom:67%;" />

## 简答

- 傅里叶级数：测线性系统的阻抗，20个波叠加；一次DFT做两个实数序列。
- 二维FFT与一维FFT：
  - 二维FFT可以拆解为，先对x方向做一维FFT，再对y方向做一维FFT。
  - 对每一行的像素值替换成该行的FFT结果；然后列。
- 二维傅里叶变换 F(0,0) 物理意义：整个图像的灰度平均值。
- 为什么在频域做xxx：
  - 通过在频域分析不同频率分量的特性，可以更好的描述降噪/锐化等任务，带来图像增强的全新思路。
  - 可以在频域指定滤波器，做反变换，指导空间域的滤波。
  - 空间域卷积，频域相乘，计算量小。
- 图像分辨率（传感器多密集） / 像素值的分辨率（模拟信号离散化的阶数） / 信噪比。
- 线性变换，为什么遍历变换后像素位置，为什么要插值：
  - 遍历原像素位置并变换，目标图像的像素值可能会一楼（不一定恰好得到值）。
  - 遍历目标图像像素，根据坐标映射逆映射得到在原始图像的位置，这个位置不一定落在原图像的像素值上，因此要通过邻域像素的插值，得到不存在的像素值。
- 为什么不能贸然使用多项式插值（伪影），已经有值的点必须和原值一样。
- 直方图均衡为什么能增强整体对比度：直方图分布越均匀，图像对比度越好。
- 线性滤波器降噪 / 锐化边界，可行性分析 / 局限性：
  - 可行性：降噪，噪声多分布在高频分量，图像信号高频能量小，高频分量信噪比小。锐化边界：边界通常代表高频分量。
  - 局限性：要在降噪+模糊边界 / 锐化边界+放大噪声之间权衡。
- 逆滤波的问题：降质函数通常是低通滤波器，高频分量值很小，噪声多分布在高频分量，会显著放大噪声。
- 为什么前景后景距离，一个d4一个d8：
  - ![image-20211229221807676](.\..\..\typora-user-images\image-20211229221807676.png)
  - 比如前景背景都是d8，中心点与右上点邻接，但它们被上和右两个点围起来了，相当于一堵围墙，本应不邻接的，产生了歧义。
  - 前景背景都是d4：不邻接，但也没有墙。
- 为什么面积分割和边界分割是等价的：可以用$\part X=X-(X\ominus B)$（我减我的腐蚀）面积 → 边界，用形态学的区域填充 边界 → 面积。

## 计算

直方图均衡：

![image-20211225161217937](.\..\..\typora-user-images\image-20211225161217937.png)

![image-20211225161247891](.\..\..\typora-user-images\image-20211225161247891.png)

huffman编码：

![image-20211225161645787](.\..\..\typora-user-images\image-20211225161645787.png)

## 设计

图像增强，图像复原，图像分割，图像压缩。



