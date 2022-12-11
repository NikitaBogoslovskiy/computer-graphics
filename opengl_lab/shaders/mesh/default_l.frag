#version 330 core

out vec4 FragColor;

uniform sampler2D texture0;

uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform vec4 emission;
uniform float shininess;

uniform vec4 pls_position;
uniform vec4 pls_ambient;
uniform vec4 pls_diffuse;
uniform vec4 pls_specular;
uniform vec3 pls_attenuation;

in Vertex {
	vec2 TexCoord; 
	vec3 normal; 
	vec3 rayToPLS; 
	vec3 viewDir;
	float distance; 
} Vert;

void main()
{
	vec3 normal_n = normalize(Vert.normal);
	vec3 rayToPLS_n = normalize(Vert.rayToPLS);
	vec3 viewDir_n = normalize(Vert.viewDir);

	float att = 1.0 / (pls_attenuation[0] + 
	pls_attenuation[1] * Vert.distance + 
	pls_attenuation[2] * Vert.distance * Vert.distance);
	
	FragColor = emission;
	
	FragColor += ambient * pls_ambient; // * att

	float Ndot = max(0.0, dot(normal_n, rayToPLS_n));
	FragColor += diffuse * pls_diffuse * Ndot * att;
	
	float RdotVpow = max(1.0, pow(dot(reflect(-rayToPLS_n, normal_n), viewDir_n), shininess));
	FragColor += specular * pls_specular * RdotVpow * att;

	FragColor *= texture(texture0, Vert.TexCoord);
}