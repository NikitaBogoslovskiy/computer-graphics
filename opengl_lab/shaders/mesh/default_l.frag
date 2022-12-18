#version 330 core

// IN ================

in Vertex {
	vec3 Normal; 
	vec2 TexCoord; 
	vec3 viewDir;
} vert;

in PointParams {
	vec3 lightDir;
	float dist;
} pointParams;

in SpotParams {
	vec3 lightDir;
	float dist;
} spotParams;

// OUT ===============

out vec4 FragColor;

// UNIFORMS ===============

uniform sampler2D texture0;

uniform struct Material {
    vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 emission;
	float shininess;
} mtl;

uniform struct PointLight {
    vec4 position;

	vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 attenuation;

    float intensity;
} pls;

uniform struct DirLight {
    vec3 direction;
	
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;

    float intensity;
} dls;

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

vec4 PointIllumination(PointLight pls, vec3 normal_n, vec3 viewDir_n);
vec4 DirIllumination(DirLight dls, vec3 normal_n, vec3 viewDir_n);
vec4 SpotIllumination(SpotLight sps, vec3 normal_n, vec3 viewDir_n);

void main() 
{
	vec3 normal_n = normalize(vert.Normal);
	vec3 viewDir_n = normalize(vert.viewDir);

    // well we may iterate over all lightcasters
	FragColor = mtl.emission 
        + pls.intensity * PointIllumination(pls, normal_n, viewDir_n)
        + dls.intensity * DirIllumination(dls, normal_n, viewDir_n)
        + sps.intensity * SpotIllumination(sps, normal_n, viewDir_n)
    ;
    FragColor *= texture(texture0, vert.TexCoord);
}

vec4 PointIllumination(PointLight pls, vec3 normal_n, vec3 viewDir_n)
{
	vec3 lightDir_n = normalize(pointParams.lightDir);

	float att = 1.0 / (pls.attenuation[0] 
    + pls.attenuation[1] * pointParams.dist 
    + pls.attenuation[2] * pointParams.dist * pointParams.dist);
	
	// ambient
	vec4 result = mtl.ambient * pls.ambient;

	// diffuse
	float Ndot = max(0.0, dot(normal_n, lightDir_n));
	result += mtl.diffuse * pls.diffuse * Ndot;
	
	// specular
	vec3 reflectDir = reflect(-lightDir_n, normal_n);
	float RdotVpow = pow(max(0.0, dot(viewDir_n, reflectDir)), mtl.shininess);
	result += mtl.specular * pls.specular * RdotVpow;

	return result * att;
}

vec4 DirIllumination(DirLight dls, vec3 normal_n, vec3 viewDir_n)
{
    vec3 lightDir_n = normalize(-dls.direction);

	// ambient
	vec4 result = dls.ambient * mtl.ambient;

    // diffuse
    float dInt = max(0.0, dot(normal_n, lightDir_n));
	result += dInt * dls.diffuse * mtl.diffuse;

    // specular 
    vec3 reflectDir = reflect(-lightDir_n, normal_n);
    float sInt = pow(max(0.0, dot(viewDir_n, reflectDir)), mtl.shininess);
	result += sInt * dls.specular * mtl.specular;
    
    return result;
}

vec4 SpotIllumination(SpotLight sps, vec3 normal_n, vec3 viewDir_n)
{
    vec3 lightDir_n = normalize(spotParams.lightDir);

    float att = 1.0 / (sps.attenuation[0] 
    + sps.attenuation[1] * spotParams.dist 
    + sps.attenuation[2] * spotParams.dist * spotParams.dist);

    // ambient
    vec4 result = sps.ambient * mtl.ambient;

    // diffuse
    float dInt = max(0.0, dot(normal_n, lightDir_n));
    result += dInt * sps.diffuse * mtl.diffuse;

    // specular
    vec3 reflectDir = reflect(-lightDir_n, normal_n);
    float sInt = pow(max(0.0, dot(viewDir_n, reflectDir)), mtl.shininess);
    result += sInt * sps.specular * mtl.specular;

    // spotlight intensity

    // that thing needs remake with spotExponent and stuff.
    float eps = sps.cutOff - sps.outerCutOff; // lower angle - bigger cos
    float theta = dot(lightDir_n, normalize(-sps.direction)); 
    float intensity = clamp((theta - sps.outerCutOff) / eps, 0.0, 1.0);

    return result * att * intensity;
}   