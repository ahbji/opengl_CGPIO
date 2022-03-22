#version 410

// 从管线输入
//in vec4 varyingColor;
in vec2 tc;

// 输出到管线
out vec4 color;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

uniform sampler2D samp;

void main(void)
{
    color = texture(samp, tc);
}
