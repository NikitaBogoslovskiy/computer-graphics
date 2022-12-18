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

vec4 PhongPointIllumination(PointLight pls, vec3 normal_n, vec3 viewDir_n);
vec4 ToonPointIllumination(PointLight pls, vec3 normal_n, vec3 viewDir_n);

vec4 PhongDirIllumination(DirLight dls, vec3 normal_n, vec3 viewDir_n);
vec4 ToonDirIllumination(DirLight dls, vec3 normal_n, vec3 viewDir_n);

vec4 PhongSpotIllumination(SpotLight sps, vec3 normal_n, vec3 viewDir_n);
vec4 ToonSpotIllumination(SpotLight sps, vec3 normal_n, vec3 viewDir_n);

float ToonCoeff(vec3 normal_n, vec3 lightDir_n);

void main() 
{
	vec3 normal_n = normalize(vert.Normal);
	vec3 viewDir_n = normalize(vert.viewDir);

    // well we may iterate over all lightcasters
	
    /*FragColor = mtl.emission * texture(texture0, vert.TexCoord)
        + pls.intensity * PhongPointIllumination(pls, normal_n, viewDir_n)
        + dls.intensity * PhongDirIllumination(dls, normal_n, viewDir_n)
        + sps.intensity * PhongSpotIllumination(sps, normal_n, viewDir_n)
    ;*/
    

    FragColor = mtl.emission
        + pls.intensity * ToonPointIllumination(pls, normal_n, viewDir_n)
        + dls.intensity * ToonDirIllumination(dls, normal_n, viewDir_n)
        + sps.intensity * ToonSpotIllumination(sps, normal_n, viewDir_n)
    ;
}

float ToonCoeff(vec3 normal_n, vec3 lightDir_n) {
    float diff = 0.2 + max (0.0, dot (normal_n, lightDir_n));
    return  0.3 + 0.7 * float(diff >= 0.4) + 0.3 * float(diff >= 0.7);
}

// POINT ILLUMINATION ====================================================

vec4 PhongPointIllumination(PointLight pls, vec3 normal_n, vec3 viewDir_n)
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

	return result * att * texture(texture0, vert.TexCoord);
}

vec4 ToonPointIllumination(PointLight pls, vec3 normal_n, vec3 viewDir_n)
{
	vec3 lightDir_n = normalize(pointParams.lightDir);
    return  mtl.diffuse * pls.diffuse * ToonCoeff(normal_n, lightDir_n);
}

// DIRECTED ILLUMINATION ====================================================

vec4 PhongDirIllumination(DirLight dls, vec3 normal_n, vec3 viewDir_n)
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
    
    return result * texture(texture0, vert.TexCoord);
}

vec4 ToonDirIllumination(DirLight dls, vec3 normal_n, vec3 viewDir_n)
{
    vec3 lightDir_n = normalize(-dls.direction);
    return  mtl.diffuse * pls.diffuse * ToonCoeff(normal_n, lightDir_n);
}

// SPOT ILLUMINATION ====================================================

vec4 PhongSpotIllumination(SpotLight sps, vec3 normal_n, vec3 viewDir_n)
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

    return result * att * intensity * texture(texture0, vert.TexCoord);
}   

vec4 ToonSpotIllumination(SpotLight sps, vec3 normal_n, vec3 viewDir_n)
{
    vec3 lightDir_n = normalize(spotParams.lightDir);

    // that thing needs remake with spotExponent and stuff.
    float eps = sps.cutOff - sps.outerCutOff; // lower angle - bigger cos
    float theta = dot(lightDir_n, normalize(-sps.direction)); 
    //float intensity = clamp((theta - sps.outerCutOff) / eps, 0.0, 1.0);

    float intensity = float(theta >= sps.cutOff);

    return  mtl.diffuse * pls.diffuse * intensity * ToonCoeff(normal_n, lightDir_n);
}
