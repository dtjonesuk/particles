#version 330 core
in vec3 vertColor;

out vec4 fragColor;

uniform vec3 u_Colour;

void main() {
    fragColor = vec4(u_Colour, 1.0);
}