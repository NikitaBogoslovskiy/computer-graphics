#version 330 core

in vec3 coord;
in vec2 texCoord;
in vec3 cc_normal;
out vec2 TexCoord;

void main()
{
	gl_Position = vec4(coord, 1.0);
	TexCoord = texCoord;
}