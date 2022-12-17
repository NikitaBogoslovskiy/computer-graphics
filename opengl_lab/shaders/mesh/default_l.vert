#version 330 core

in vec3 coord;
in vec3 normal;
in vec2 texCoord;

out Vertex {
	vec3 FragPos;
	vec3 Normal; 
	vec2 TexCoord; 
	vec3 viewDir;
} Vert;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 viewPos;

void main()
{
	Vert.FragPos = vec3(model * vec4(coord, 1.0));
	Vert.Normal = mat3(transpose(inverse(model))) * normal;
	Vert.TexCoord = texCoord;
	Vert.viewDir = viewPos - Vert.FragPos;
	
	gl_Position = projection * view * vec4(Vert.FragPos, 1.0);
}