#version 330 core
out vec4 FragColor;

in vec4 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform float mixRatio;

void main()
{
	//FragColor = texture(texture1, TexCoord) * vec4(ourColor, 1.0);  
	FragColor = ourColor + mixRatio * (texture(texture1, TexCoord) - ourColor);
}