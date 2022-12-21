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
    float eps;
    float outerCutOff;  // OUTER CONE
						// FOR PENUMBRA BETWEEN INNER AND OUTER CONES
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
	vec3 attenuation;

    float intensity;
} sps;


vec4 ToonPoint(PointLight pls, vec3 normal_n, vec3 viewDir_n);
vec4 ToonDir(DirLight dls, vec3 normal_n, vec3 viewDir_n);
vec4 ToonSpot(SpotLight sps, vec3 normal_n, vec3 viewDir_n);

void main() 
{
	vec3 normal_n = normalize(vert.Normal);
	vec3 viewDir_n = normalize(vert.viewDir);

    FragColor = (mtl.emission
        + pls.intensity * ToonPoint(pls, normal_n, viewDir_n)
        + dls.intensity * ToonDir(dls, normal_n, viewDir_n)
        + sps.intensity * ToonSpot(sps, normal_n, viewDir_n)
        ) * texture(texture0, vert.TexCoord)
    ;
}

float _spotLightIntensity(float eps, float outerCutOff, vec3 lightDir_n, vec3 spotDir) {
    float theta = dot(normalize(lightDir_n), normalize(-spotDir)); 
    return clamp((theta - outerCutOff) * eps, 0.0, 1.0);
}

float ToonCoeff(vec3 normal_n, vec3 lightDir_n) {
    float diff = 0.2 + max (0.0, dot (normal_n, lightDir_n));
    return  0.3 + 0.7 * float(diff >= 0.4) + 0.3 * float(diff >= 0.7);
}

vec4 ToonPoint(PointLight pls, vec3 normal_n, vec3 viewDir_n)
{
	vec3 lightDir_n = normalize(pointParams.lightDir);
    return  mtl.diffuse * pls.diffuse * ToonCoeff(normal_n, lightDir_n);
}

vec4 ToonDir(DirLight dls, vec3 normal_n, vec3 viewDir_n)
{
    vec3 lightDir_n = normalize(-dls.direction);
    return  mtl.diffuse * dls.diffuse * ToonCoeff(normal_n, lightDir_n);
}

vec4 ToonSpot(SpotLight sps, vec3 normal_n, vec3 viewDir_n)
{
    vec3 lightDir_n = normalize(spotParams.lightDir);

    //float eps = sps.cutOff - sps.outerCutOff; // lower angle - bigger cos
    //float theta = dot(lightDir_n, normalize(-sps.direction));
    //float intensity = float(theta >= sps.cutOff);

    float att = 1.0 / (sps.attenuation[0] 
    + sps.attenuation[1] * spotParams.dist 
    + sps.attenuation[2] * spotParams.dist * spotParams.dist);

    float intensity = _spotLightIntensity(sps.eps,  sps.outerCutOff, lightDir_n, sps.direction);

    return  att * intensity * mtl.diffuse * sps.diffuse *  ToonCoeff(normal_n, lightDir_n);
}
