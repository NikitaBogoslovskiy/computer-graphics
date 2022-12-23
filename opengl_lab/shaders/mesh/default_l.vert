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

out SpotParams {
	vec3 lightDir;
	float dist;
	vec3 lvBisector;
} spotParams;

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

uniform struct SpotLight {
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
} sps;

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

	spotParams.lightDir = vec3(sps.position) - vPos3;
	spotParams.dist = length(spotParams.lightDir);
	spotParams.lvBisector = normalize(spotParams.lightDir) + vert.viewDir;

	gl_Position = transform.projection * transform.view * vPos4;
}