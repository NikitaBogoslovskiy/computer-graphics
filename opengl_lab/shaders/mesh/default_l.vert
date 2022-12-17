#version 330 core

in vec3 coord;
in vec3 normal;
in vec2 texCoord;

out Vertex {
	vec3 FragPos;
	vec3 Normal; 
	vec2 TexCoord; 
	vec3 viewDir;
} vert;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 viewPos;

// actually should pull here lightDirs for the lights but now whatever
void main()
{
	vert.FragPos = vec3(model * vec4(coord, 1.0));
	vert.Normal = mat3(transpose(inverse(model))) * normal;
	vert.TexCoord = texCoord;
	vert.viewDir = viewPos - vert.FragPos;
	
	gl_Position = projection * view * vec4(vert.FragPos, 1.0);
}