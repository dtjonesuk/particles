#version 330 core
in vec3 pos;
out vec3 color;
uniform vec3 u_Color;

void main() {
    gl_Position = vec4(pos, 1.0);
    color = u_Color;
}