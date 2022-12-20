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

vec4 PhongPointIllumination(PointLight pls, vec3 normal_n, vec3 viewDir_n);
vec4 PhongDirIllumination(DirLight dls, vec3 normal_n, vec3 viewDir_n);
vec4 PhongSpotIllumination(SpotLight sps, vec3 normal_n, vec3 viewDir_n);

void main() 
{
	vec3 normal_n = normalize(vert.Normal);
	vec3 viewDir_n = normalize(vert.viewDir);

    FragColor = (mtl.emission
        + pls.intensity * PhongPointIllumination(pls, normal_n, viewDir_n)
        + dls.intensity * PhongDirIllumination(dls, normal_n, viewDir_n)
        + sps.intensity * PhongSpotIllumination(sps, normal_n, viewDir_n)
        ) * texture(texture0, vert.TexCoord)
    ;
}

float _spotLightIntensity(float cutOff, float outerCutOff, vec3 lightDir_n, vec3 spotDir) {
    // that thing needs remake with spotExponent and stuff.
    float eps = cutOff - outerCutOff; // lower angle - bigger cos
    float theta = dot(lightDir_n, normalize(-spotDir)); 
    return clamp((theta - outerCutOff) / eps, 0.0, 1.0);
}

vec4 _phongIllum( vec4 lightAmbient, vec4 lightDiffuse, vec4 lightSpecular,
                  vec3 normal_n, vec3 viewDir_n, vec3 lightDir_n
) {
    // ambient
	vec4 result = mtl.ambient * lightAmbient;

	// diffuse
	float dInt = max(0.0, dot(normal_n, lightDir_n));
	result += dInt * mtl.diffuse * lightDiffuse;
	
	// specular
	vec3 reflectDir = reflect(-lightDir_n, normal_n);
	float sInt = pow(max(0.0, dot(viewDir_n, reflectDir)), mtl.shininess);
	result += sInt * mtl.specular * lightSpecular;

	return result;
}
vec4 PhongPointIllumination(PointLight pls, vec3 normal_n, vec3 viewDir_n)
{
	vec3 lightDir_n = normalize(pointParams.lightDir);
	float att = 1.0 / (pls.attenuation[0] 
    + pls.attenuation[1] * pointParams.dist 
    + pls.attenuation[2] * pointParams.dist * pointParams.dist);
	
    return att * _phongIllum(pls.ambient, pls.diffuse, pls.specular, normal_n, viewDir_n, lightDir_n);
}

vec4 PhongDirIllumination(DirLight dls, vec3 normal_n, vec3 viewDir_n)
{
    vec3 lightDir_n = normalize(-dls.direction);
    return _phongIllum(dls.ambient, dls.diffuse, dls.specular, normal_n, viewDir_n, lightDir_n);
}



vec4 PhongSpotIllumination(SpotLight sps, vec3 normal_n, vec3 viewDir_n)
{
    vec3 lightDir_n = normalize(spotParams.lightDir);

    float att = 1.0 / (sps.attenuation[0] 
    + sps.attenuation[1] * spotParams.dist 
    + sps.attenuation[2] * spotParams.dist * spotParams.dist);

    float intensity = _spotLightIntensity(sps.cutOff,  sps.outerCutOff, lightDir_n, sps.direction);

    return att * intensity * _phongIllum(sps.ambient, sps.diffuse, sps.specular, normal_n, viewDir_n, lightDir_n);
} 
