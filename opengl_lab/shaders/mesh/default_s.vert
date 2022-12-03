#version 330 core

in vec3 coord;
in vec2 texCoord;
in vec3 cc_normal;
out vec2 TexCoord;

//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;

void main()
{
	//projection * view * model * 
	gl_Position = vec4(coord, 25.0);
	TexCoord = texCoord;
}