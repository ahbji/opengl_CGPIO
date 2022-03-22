#version 410

// 从管线输入
in vec4 varyingColor;
// 输出到管线
out vec4 color;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

void main(void)
{
    color = varyingColor;
}
