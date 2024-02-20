# di 03 几何变换与插值

geometric transform & interpolation。

## 基本概念

原始图像：还没有被处理的图像。（不过，当context是图像复原的时候，原始图像指的是没有被损坏/干扰的图像。）

目标图像：希望处理后得到的结果。

坐标系是固定的，称为图像（画布）坐标系。（图形学恰好相反，对坐标系做变换。）

几何变换的两个步骤：

- 坐标映射：$\overrightarrow{X'}=T(\overrightarrow X)$，将【原始图像每个像素的坐标】映射到【目标图像的新坐标】。
- 像素赋值：$f(\overrightarrow{X'})\leftarrow f(\overrightarrow{X})$，把【原始图像的像素值】赋给【新坐标处的像素】。

## 仿射变换，affine transformation

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

覆盖：平移、选择、缩放。

### 平移，translation

$$
\left\{\begin{array}{}
x'=x+t_x\\
y'=y+t_y
\end{array}\right.
\\
A=\left[\begin{array}{}
1&0&t_x \\
0&1&t_y
\end{array}\right]
$$

### 缩放，scaling

$$
\left\{\begin{array}{}
x'=s_x\cdot x\\
y'=s_y\cdot y
\end{array}\right.
\\
A=\left[\begin{array}{}
s_x&0&0 \\
0&s_y&0
\end{array}\right]
$$

### 旋转，rotation

$$
\left\{\begin{array}{}
x'=\cos\theta\cdot x-\sin\theta\cdot y\\
y'=\sin\theta\cdot x+\cos\theta\cdot y
\end{array}\right.
\\
A=\left[\begin{array}{}
\cos\theta & -\sin\theta & 0 \\
\sin\theta & \cos\theta & 0
\end{array}\right]
$$

旋转的正方向定义为【x轴正方向到y轴正方向】。貌似一般是顺时针，因为y轴正方向指向下。

旋转的技术细节：我们一般希望图像绕图像中心旋转，但按照上面的公式，图像是绕左上角旋转的。因此我们先平移原点，再旋转，再把原点平移回来。但是做3次矩阵乘法计算量太大，因此我们把这3步操作综合成一个矩阵乘法，直接一步到位、
$$
\left[\begin{array}{}
x'\\y'
\end{array}{}\right]
=
R
\left[\begin{array}{}
x-W/2\\y-H/2
\end{array}\right]{}
+
\left[\begin{array}{}
W/2\\H/2
\end{array}{}\right]
\\
=
R
\left[\begin{array}{}
x\\y
\end{array}\right]{}
-
\left[\begin{array}{}
\cos\theta & -\sin\theta \\
\sin\theta & \cos\theta
\end{array}\right]{}
\left[\begin{array}{}
W/2\\H/2
\end{array}\right]{}
+
\left[\begin{array}{}
W/2\\H/2
\end{array}{}\right]
\\
=
\left[\begin{array}{}
\cos\theta & -\sin\theta & \frac W2(1-\cos\theta)+\frac H2\sin\theta
\\
\sin\theta & \cos\theta & \frac H2(1-\cos\theta)-\frac W2\sin\theta
\end{array}\right]{}
\left[\begin{array}{}
x\\y\\1
\end{array}{}\right]
$$
（这个不重要的）

## 几何变换第二步：赋值的问题

如果是模拟信号，信号是连续的，那么赋值没问题。如果是数字信号，信号是离散的，那么 原始图像有像素的点 经过变换后 不一定落在 目标图像有像素的点，目标图像有像素的点 也不一定恰好得到 原始图像的数据。

### 程序实现几何变换的遍历问题

遍历原始图像像素的坐标，求映射到的目标图像像素的坐标？还是，遍历目标图像像素，寻找映射到此处的原始图像像素？

如果采用【遍历原始图像像素】的方法，会出现【原始图像像素点不一定映射在目标图像有像素的点】【目标图像有像素的点不一定恰好得到新数据】的问题。

因此我们【遍历目标图像像素】，求原坐标变换的反变换。（实验时需要用到）

如果【目标图像像素反变换后的原始图像位置】上没有像素（没有恰好落在数字信号的采样点上），我们需要通过插值得到像素数据。

### 图像的插值，interpolation

信号的两个基本假设：

- 能量有限假设：在有限时间范围内，能量是有限的。
- 连续性假设：时间间隔趋向于0的时候，信号的值是连续的。（可以理解为相似性假设）
- 物理含义：相邻信号值尽量接近。

插值：根据邻域值估算本不存在的值。

计算插值时，首先定义邻域。

接下来定义插值准则（插值函数）：如何根据邻域的值进行估计。

插值函数：要求连续且严格通过所有采样点。

#### 最近邻插值，nearest interpolation

找离它最近的有定义的点，采用那个点的像素值。

在放大倍数比较大的时候，会产生锯齿，图像的光滑性受损害。

#### 线性插值，linear interpolation

邻域需包含两个采样点，两点确定一条直线。

$f(x')=\alpha f(x_{i-1})+(1-\alpha)f(x_i)$。

#### 双线性插值，bilinear interpolation

图像中的线性插值，二维的线性插值。

![image-20210823121337145](.\..\..\typora-user-images\image-20210823121337145.png)

总共进行3次一维线性插值。可以先算好每个邻域点对插值结果的最终贡献，然后一次得到。

避免了边界的锯齿，但边界变模糊了，图像的清晰度受损害。

#### 样条插值（补充）

只是一种更高阶的炫酷多项式插值，比线性插值更好看，缺点是可能会在本来平滑的部分插值产生纹理。（伪影，本来不存在却出现在照片上的图像）







