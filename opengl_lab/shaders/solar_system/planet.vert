#version 330 core

in vec3 coord;
in vec2 texCoord;
in vec3 cc_normal;
in mat4 instanceMatrix;

out vec2 TexCoord;

uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * instanceMatrix * vec4(coord, 1.0);
	TexCoord = texCoord;
}