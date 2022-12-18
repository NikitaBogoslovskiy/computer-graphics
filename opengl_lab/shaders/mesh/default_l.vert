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
} pointParams;

out SpotParams {
	vec3 lightDir;
	float dist;
} spotParams;

// UNIFORMS ===============

uniform struct Transform {
	mat4 model;
	mat4 view;
	mat4 projection;
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
    float cutOff;       // COSINE OF INNER CONE
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

	vert.Normal = mat3(transpose(inverse(transform.model))) * normal;
	vert.TexCoord = texCoord;
	vert.viewDir = transform.viewPos - vPos3;
	
	pointParams.lightDir = vec3(pls.position) - vPos3;
	pointParams.dist = length(pointParams.lightDir);

	// actually its interesting which way is better
	// if statement based on isOn or just recomputing these everytime
	spotParams.lightDir = vec3(sps.position) - vPos3;
	spotParams.dist = length(spotParams.lightDir);

	gl_Position = transform.projection * transform.view * vPos4;
}