#version 330 core
in vec3 coord;
in vec4 color;
out vec4 vcolor;
uniform vec2 scale;

void main() {
    gl_Position = vec4(coord.xy * scale, coord.z, 1.2);
    vcolor = color;
}