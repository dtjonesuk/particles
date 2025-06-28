#version 330 core
in vec3 pos;

out vec3 vertColor;

uniform mat4x4 u_MVP;
  
void main() {
    gl_Position = u_MVP * vec4(pos, 1.0);
}