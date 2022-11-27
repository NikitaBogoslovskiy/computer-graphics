#version 330 core

in vec3 coord;
in vec4 color;

out vec4 vcolor;

uniform vec2 offset;

void main() {
    gl_Position = vec4(coord.xy + offset, -coord.z, 1.0);
    vcolor = color;
}