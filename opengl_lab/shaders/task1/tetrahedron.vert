#version 330 core

in vec3 coord;
in vec4 color;

out vec4 vcolor;

//uniform vec2 offset;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    //gl_Position = vec4(coord.xy + offset, coord.z, 1.0);
    gl_Position = projection * view * model * vec4(coord, 1.0);
    vcolor = color;
}