在顶点着色器中定义 uniform 变量（统一变量） offset 。

每次顶点着色器运行时，会将 offset 加给所绘制图元的 x 坐标。

```glsl
#version 410
// 定义 uniform 变量
uniform float offset;
void main(void)
{
	if (gl_VertexID == 0)
        gl_Position = vec4( 0.25+offset,-0.25, 0.0, 1.0);
	else if (gl_VertexID == 1) 
        gl_Position = vec4(-0.25+offset,-0.25, 0.0, 1.0);
  	else 
        gl_Position = vec4( 0.25+offset, 0.25, 0.0, 1.0);
}
```

display 方法维护变量 x 用于偏移图元的 x 轴位置，每当 display 调用时，x 的值都会改变。
同时，每当 x 值到达 1.0 或 -1.0 时，都会改变方向。
x 的值会被赋值到顶点着色器的 offset Uniform 变量中。

每次调用 display 时，需要初始化缓冲区：
- 初始化深度缓冲区：对于复杂3D场景，需要确保深度对比不会受旧的深度数据影响。
- 初始化颜色缓冲区：以避免图元移动时留下一串轨迹
```c++
void display(GLFWwindow* window, double currentTime) {
    // 初始化深度缓冲区
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glUseProgram(renderingProgram);

    x += inc;
    if (x > 1.0f) inc = -0.01f;
    if (x < -1.0f) inc = 0.01f;
    // 获取 uniform 变量 offset
    GLuint offsetLoc = glGetUniformLocation(renderingProgram, "offset");
    // 将 x 值复制给 offset
    glProgramUniform1f(renderingProgram, offsetLoc, x);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
```



