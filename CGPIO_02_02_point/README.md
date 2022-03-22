## 绘制顶点

顶点的绘制需要使用`顶点着色器`和`片段着色器`

图元包括：点、线、简单图形，多数 3D 模型通常是由许多三角形图元构成。

图元由顶点组成：例如三角形有三个顶点。

顶点可以由许多个源产生：
- 从文件读取并由 C++/OpenGL 应用载入缓冲区
- 硬编码到 C++ 代码或 GLSL 代码中。

在加载顶点前， C++/OpenGL 应用必须编译并链接合适的顶点着色器和片段着色器，然后将之载入 ShaderProgram 。

绘制定点前，需要初始化`顶点数组对象` VAO （Vertex Array Objects）：
- C++/OpenGL 应用一般将定点数据集以缓冲区的形式发送到 OpenGL 管线，这些缓冲区最后都会被存入顶点数组对象
- 即使应用不需要缓冲区（例如在着色器中硬编码顶点），OpenGL 仍然要求应用创建 VAO ，并将其绑定到管线。

最后，在渲染循环中， 载入 ShaderProgram 到 OpenGL 管道，然后通知 OpenGL 绘制图元。

## 顶点着色器 Vertex Shader

顶点着色器的主要目标是将顶点数据发送到管线，例如以下 GLSL 代码定义的顶点着色器：
```glsl
#version 410
// 设置顶点在 3D 控件的坐标位置
// 硬编码原点位置值 : (0,0,0)
// gl_Position 是预定义的输出变量，使用 vec4 四元组记录顶点的齐次坐标
// gl_Position 的值最终会输出到光栅化着色器进行光栅化
void main(void)
{
    gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
}
```

## 片段着色器 Fragment Shader

片段着色器的主要目的是给将要绘制的像素赋予 RGB 颜色，例如以下 GLSL 代码定义的片段着色器：
```glsl
#version 410
// 输出颜色值
// out 表明 color 变量是个输出变量
"out vec4 color;
void main(void)
{
    color = vec4(0.0, 0.0, 1.0, 1.0);
}
···



