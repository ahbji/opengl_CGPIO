glDrawArrays 指定了三角形，同时也指定了管线中有三个顶点。

```c++
void display(GLFWwindow *window, double currentTime)
{
    ...
    // 第二个参数指定从第几个点开始绘制
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
```

顶点着色器会在每个迭代运行3遍：
- 内置变量 gl_VertexID 会自增（初始值为 0）
- 通过检测 gl_VertexID 的值，着色器设计可以在没存运行时输出不同的点。

```glsl
#version 410
void main(void)
{
	if (gl_VertexID == 0)
		gl_Position = vec4( 0.25,-0.25, 0.0, 1.0);
	else if (gl_VertexID == 1) 
		gl_Position = vec4(-0.25,-0.25, 0.0, 1.0);
  	else 
		gl_Position = vec4( 0.25, 0.25, 0.0, 1.0);
}
```



