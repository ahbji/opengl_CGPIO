#version 410

in vec2 tc;
// in vec4 varyingColor;
// 输出到管线
out vec4 color;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

uniform sampler2D samp;

void main(void)
{
    // color = varyingColor;
    color = texture(samp, tc);
}
