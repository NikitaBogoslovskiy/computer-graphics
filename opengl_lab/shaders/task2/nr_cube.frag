#version 330 core
out vec4 FragColor;

in vec4 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform float mixRatio;

void main()
{
	FragColor = mix(texture(texture1, TexCoord), ourColor, mixRatio);
}