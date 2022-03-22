# 矩阵

GLSL/GLM 使用 mat4 数据类型存储 4*4 矩阵

## 矩阵转置

矩阵的转置计算（交换矩阵的行和列）

转置后的矩阵记作$R^T$

- GLSL使用 traspose(mat4) 函数
- GLM 库使用 glm::transpose(mat4) 函数

GLSL 中对 + 运算符在 mat4 上进行了重载，支持矩阵加法。

## 矩阵乘法

点与矩阵相乘得到点：$
\left(
\begin{matrix}
a*x & b*y & c*z & d \\
e*x & f*y & g*z & h \\
i*x & j*y & k*z & l \\
m*x & n*y & o*z & p
\end{matrix}
\right)
= \left(
\begin{matrix}
a & b & c & d \\
e & f & g & h \\
i & j & k & l \\
m & n & o & p
\end{matrix}
\right)
\left(
\begin{matrix}
x \\
y \\
z \\
1
\end{matrix}
\right)
$

矩阵相乘也叫做矩阵合并（A 的行数==B 的列数）:$
\begin{bmatrix}
a & b \\
c & d \\
\end{bmatrix} \times
\begin{bmatrix}
e & f \\
g & h \\
\end{bmatrix}=
\begin{bmatrix}
ae + bg & af + bh \\
ce + dg & cf + dh \\
\end{bmatrix}
$

$E(i,j) = A i 行 \cdot B j 列$

$
\left(
\begin{matrix}
1 & 3 \\
5 & 2 \\
0 & 4
\end{matrix}
\right)
\left(
\begin{matrix}
3 & 6 & 9 & 4 \\
2 & 7 & 8 & 3
\end{matrix}
\right)=
\left(
\begin{matrix}
9 & ? & 33 & 13 \\
19 & 44 & 61 & 26 \\
8 & 28 & 32 & ?
\end{matrix}
\right)
$


GLSL 和 GLM 都支持使用重载后的 * 运算符进行矩阵乘法

## 矩阵乘法的结合律

合并矩阵变换的能力来自矩阵乘法的结合律

$P^\prime = M1 * (M2 * (M3 * P)) = (M1 * M2 * M3) * P$

矩阵的相乘被认为是建立矩阵的连接，所以相乘后的矩阵可以被记作 $M_C$

$M_C = M_1 * M_2 * M_3$

可以得出：

$P^\prime = MC * P$

因为需要将相同的一系列矩阵变换应用到场景中的每个点上，使用矩阵乘法的好处可以通过预先计算好一些矩阵的合并，以成倍减少总的矩阵运算量。

## 矩阵乘法的分配率

- $(A+B)C = AC + BC$

- $A(B+C) = AC + BC$

## 单位矩阵

单位矩阵（对角线的值为 1， 其余值为 0）：
$
\begin{bmatrix}
1 & 0 & 0 & 0 \\
0 & 1 & 0 & 0 \\
0 & 0 & 1 & 0 \\
0 & 0 & 0 & 1
\end{bmatrix}
$

GLM 使用构造函数 glm::mat4 m(1.0f) 创建单位矩阵

一般用于按Y轴做对称操作

$
\left(
\begin{matrix}
-1 & 0 \\
0 & 1
\end{matrix}
\right)
\left(
\begin{matrix}
x \\
y
\end{matrix}
\right) = 
\left(
\begin{matrix}
-x \\
y
\end{matrix}
\right)
$

## 逆矩阵
矩阵 M 的逆矩阵 $M^{-1}$：
- GLSL 和 GLM 都使用 mat4.inverse() 计算逆矩阵

