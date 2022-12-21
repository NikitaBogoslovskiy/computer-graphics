#version 330 core
#define PI 3.14159265

// IN ================

in Vertex {
	vec3 Normal; 
	vec2 TexCoord; 
	vec3 viewDir;
} vert;

in PointParams {
	vec3 lightDir;
	float dist;
    vec3 lvBisector;
} pointParams;

in SpotParams {
	vec3 lightDir;
	float dist;
    vec3 lvBisector;
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

    float roughness;
    float reflectivity;
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


vec4 ToonPointIllumination(PointLight pls, vec3 normal_n, vec3 viewDir_n);
vec4 ToonDirIllumination(DirLight dls, vec3 normal_n, vec3 viewDir_n);
vec4 ToonSpotIllumination(SpotLight sps, vec3 normal_n, vec3 viewDir_n);
float ToonCoeff(vec3 normal_n, vec3 lightDir_n);

void main() 
{
	vec3 normal_n = normalize(vert.Normal);
	vec3 viewDir_n = normalize(vert.viewDir);

    FragColor = (mtl.emission
        + pls.intensity * ToonPointIllumination(pls, normal_n, viewDir_n)
        + dls.intensity * ToonDirIllumination(dls, normal_n, viewDir_n)
        + sps.intensity * ToonSpotIllumination(sps, normal_n, viewDir_n)
        ) * texture(texture0, vert.TexCoord)
    ;
}

float _spotLightIntensity(float cutOff, float outerCutOff, vec3 lightDir_n, vec3 spotDir) {
    // that thing needs remake with spotExponent and stuff.
    float eps = cutOff - outerCutOff; // lower angle - bigger cos
    float theta = dot(lightDir_n, normalize(-spotDir)); 
    return clamp((theta - outerCutOff) / eps, 0.0, 1.0);
}

float ToonCoeff(vec3 normal_n, vec3 lightDir_n) {
    float diff = 0.2 + max (0.0, dot (normal_n, lightDir_n));
    return  0.3 + 0.7 * float(diff >= 0.4) + 0.3 * float(diff >= 0.7);
}

vec4 ToonPointIllumination(PointLight pls, vec3 normal_n, vec3 viewDir_n)
{
	vec3 lightDir_n = normalize(pointParams.lightDir);
    return  mtl.diffuse * pls.diffuse * ToonCoeff(normal_n, lightDir_n);
}

vec4 ToonDirIllumination(DirLight dls, vec3 normal_n, vec3 viewDir_n)
{
    vec3 lightDir_n = normalize(-dls.direction);
    return  mtl.diffuse * dls.diffuse * ToonCoeff(normal_n, lightDir_n);
}

vec4 ToonSpotIllumination(SpotLight sps, vec3 normal_n, vec3 viewDir_n)
{
    vec3 lightDir_n = normalize(spotParams.lightDir);

    // that thing needs remake with spotExponent and stuff.
    float eps = sps.cutOff - sps.outerCutOff; // lower angle - bigger cos
    float theta = dot(lightDir_n, normalize(-sps.direction));
    float intensity = float(theta >= sps.cutOff);

    //float intensity = _spotLightIntensity(sps.cutOff,  sps.outerCutOff, lightDir_n, sps.direction);

    return  mtl.diffuse * sps.diffuse * intensity * ToonCoeff(normal_n, lightDir_n);
}