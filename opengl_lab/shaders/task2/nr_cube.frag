#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform float mixRatio;

void main()
{
	vec4 col = vec4(ourColor, 1.0);
	//FragColor = texture(texture1, TexCoord) * vec4(ourColor, 1.0);  
	FragColor = col + mixRatio * (texture(texture1, TexCoord) - col);
}