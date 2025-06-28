#version 330 core
in vec3 vertColor;

out vec4 fragColor;

uniform vec3 u_Color;

void main() {
    fragColor = vec4(u_Color, 1.0);
}