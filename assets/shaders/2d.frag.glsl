#version 330 core
in vec3 color;
in vec2 texCoord;

out vec4 frag_colour;

uniform float u_Percent;
uniform sampler2D u_Texture;

void main() {
//    vec3 default_col = vec3(1.0, 1.0, 1.0);
//    frag_colour = vec4(mix(color, default_col, u_Percent), 1.0);
    frag_colour = mix(vec4(color, 1.0), texture(u_Texture, texCoord), u_Percent);
}