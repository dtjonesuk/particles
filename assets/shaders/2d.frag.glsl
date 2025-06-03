#version 330 core
in vec3 color;
out vec4 frag_colour;

uniform float u_Percent;

void main() {
    vec3 default_col = vec3(1.0, 1.0, 1.0);
    frag_colour = vec4(mix(color, default_col, u_Percent), 1.0);
}