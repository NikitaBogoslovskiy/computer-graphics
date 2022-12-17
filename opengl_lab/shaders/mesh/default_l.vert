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
	vec3 FragPos;
} Vert;

void main()
{
	Vert.FragPos = vec3(model * vec4(coord, 1.0));
	Vert.normal = mat3(transpose(inverse(model))) * normal;
	Vert.TexCoord = texCoord;
	
	Vert.rayToPLS = pls_position.xyz - Vert.FragPos;
	Vert.viewDir = viewPos - Vert.FragPos;
	Vert.distance = length(Vert.rayToPLS);

	gl_Position = projection * view * vec4(Vert.FragPos, 1.0);
}