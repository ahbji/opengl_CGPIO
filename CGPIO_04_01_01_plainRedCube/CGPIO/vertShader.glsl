#version 410

// 顶点属性变量
// - in 将会从缓冲区中接收数据
// - vec3 顶点数据类型
// - layout(location=0) layout 修饰符，识别号为 0
layout (location=0) in vec3 position;

// 约定俗成所有 shader 中包含同样的 uniform 变量声明
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

void main(void)
{
	// 将 vec3 position 转换为 vec4
	// 将变换矩阵应用到 position
	gl_Position = proj_matrix * mv_matrix * vec4(position,1.0);
} 
