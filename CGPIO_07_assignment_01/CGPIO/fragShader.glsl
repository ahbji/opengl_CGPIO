#version 410

in vec4 varyingColor;
out vec4 fragColor;

// uniforms 变量与顶点着色器一致，但不直接在当前片段着色器中使用

struct PositionalLight
{	vec4 ambient;  
	vec4 diffuse;  
	vec4 specular;  
	vec3 position;
};

struct Material
{	vec4 ambient;  
	vec4 diffuse;  
	vec4 specular;  
	float shininess;
};

uniform vec4 globalAmbient;
uniform PositionalLight light;
uniform Material material;
uniform mat4 mv_matrix;	 
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;

// 差值光照相关颜色
// gl_Position 自动被差值

void main(void)
{
    fragColor = varyingColor;
}
