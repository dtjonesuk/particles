#version 330 core
in vec3 pos;
in vec3 col;
out vec3 color;
uniform vec3 u_Color;
uniform mat4x4 u_Transform;

void main() {
    gl_Position = u_Transform * vec4(pos, 1.0);
    color = col;
}