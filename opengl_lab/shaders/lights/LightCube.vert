#version 330 core
layout (location = 0) in vec3 coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec4 inColor;

out vec4 vColor;

void main()
{
	gl_Position = projection * view * model * vec4(coord, 1.0);
	vColor =  inColor;
}