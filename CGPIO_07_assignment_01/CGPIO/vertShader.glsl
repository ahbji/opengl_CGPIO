#version 410

layout (location= 0) in vec3 vertPos;
layout (location= 1) in vec3 vertNormal;

out vec4 varyingColor;

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
uniform mat4 norm_matrix; // 用于变换法向量

void main(void)
{
    vec4 color;
	
	// 将顶点 position 变换到 view 空间
	vec4 P = mv_matrix * vec4(vertPos,1.0);
	
	// 将法向量变换到 view 空间
	vec3 N = normalize((norm_matrix * vec4(vertNormal,1.0)).xyz);

	// 计算 view 空间的光照向量
	vec3 L = normalize(light.position - P.xyz);
	
    // view 向量等于 view 空间中的负顶点位置
	vec3 V = normalize(-P.xyz);
	
    // R 是 -L 的相对于 N 的景象
	vec3 R = reflect(-L,N);

    // 环境光分量
	vec3 ambient =
		((globalAmbient * material.ambient)
		+ (light.ambient * material.ambient)).xyz;
	// 漫反射分量
	vec3 diffuse =
		light.diffuse.xyz * material.diffuse.xyz
		* max(dot(N,L), 0.0);
	// 镜面反射分量
	vec3 specular =
		pow(max(dot(R,V), 0.0f), material.shininess)
		* material.specular.xyz * light.specular.xyz;

    // 输出 color 到管线
	varyingColor = vec4((ambient + diffuse + specular), 1.0);
	
    // 输出 position 到管线
	gl_Position = proj_matrix * mv_matrix * vec4(vertPos,1.0);
}
