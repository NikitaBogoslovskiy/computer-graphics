#version 330 core

in vec3 coord;
in vec2 texCoord;
in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 viewPos;

uniform vec4 pls_position;
uniform vec4 pls_ambient;
uniform vec4 pls_diffuse;
uniform vec4 pls_specular;
uniform vec3 pls_attenuation;

out Vertex {
	vec2 TexCoord; 
	vec3 normal; 
	vec3 rayToPLS; 
	vec3 viewDir;
	float distance; 
} Vert;

void main()
{
	gl_Position = projection * view * model * vec4(coord, 1.0);
	
	Vert.TexCoord = texCoord;
	Vert.normal = normal;
	Vert.rayToPLS = pls_position.xyz - coord;
	Vert.distance = length(Vert.rayToPLS);
	Vert.viewDir = viewPos - coord;
}