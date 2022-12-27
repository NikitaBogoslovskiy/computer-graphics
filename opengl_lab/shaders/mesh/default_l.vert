#version 330 core

// IN ================

in vec3 coord;
in vec3 normal;
in vec2 texCoord;

// OUT ===============

out Vertex {
	vec3 Normal; 
	vec2 TexCoord; 
	vec3 viewDir;
} vert;

out PointParams {
	vec3 lightDir;
	float dist;
	vec3 lvBisector;
} pointParams;

out SpotParams1 {
	vec3 lightDir;
	float dist;
	vec3 lvBisector;
} spotParams1;

out SpotParams2 {
	vec3 lightDir;
	float dist;
	vec3 lvBisector;
} spotParams2;

// UNIFORMS ===============

uniform struct Transform {
	mat4 model;
	mat4 view;
	mat4 projection;
	mat3 normalTr;
	vec3 viewPos; // global coords
} transform;

uniform struct PointLight {
    vec4 position;

	vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 attenuation;

	float intensity;
} pls;

struct SpotLight {
    vec3 position;
    vec3 direction;
    float eps;
    float outerCutOff;  // OUTER CONE
						// FOR PENUMBRA BETWEEN INNER AND OUTER CONES
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
	vec3 attenuation;

	float intensity;
};

uniform SpotLight sps1;
uniform SpotLight sps2;

// actually should pull here lightDirs for the lights but now whatever
void main()
{
	vec4 vPos4 = transform.model * vec4(coord, 1.0);
	vec3 vPos3 = vec3(vPos4);

	vert.Normal = transform.normalTr * normal;
	vert.TexCoord = texCoord;
	vert.viewDir = normalize(transform.viewPos - vPos3);
	
	pointParams.lightDir = vec3(pls.position) - vPos3;
	pointParams.dist = length(pointParams.lightDir);
	pointParams.lvBisector = normalize(pointParams.lightDir) + vert.viewDir;

	spotParams1.lightDir = vec3(sps1.position) - vPos3;
	spotParams1.dist = length(spotParams1.lightDir);
	spotParams1.lvBisector = normalize(spotParams1.lightDir) + vert.viewDir;
	
	spotParams2.lightDir = vec3(sps2.position) - vPos3;
	spotParams2.dist = length(spotParams2.lightDir);
	spotParams2.lvBisector = normalize(spotParams2.lightDir) + vert.viewDir;

	gl_Position = transform.projection * transform.view * vPos4;
}