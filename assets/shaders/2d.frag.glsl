#version 330 core
in vec3 color;
in vec2 texCoord;

out vec4 frag_colour;

uniform float u_Percent;
uniform sampler2D u_Texture;
float blendOverlay(float base, float blend) {
    return base<0.5?(2.0*base*blend):(1.0-2.0*(1.0-base)*(1.0-blend));
}

vec4 blendOverlay(vec4 base, vec4 blend) {
    return vec4(blendOverlay(base.r,blend.r),blendOverlay(base.g,blend.g),blendOverlay(base.b,blend.b), 1.0);
}
void main() {
//    vec3 default_col = vec3(1.0, 1.0, 1.0);
//    frag_colour = vec4(mix(color, default_col, u_Percent), 1.0);
    vec4 base =vec4(color, 1.0);
    vec4 blend = texture(u_Texture, texCoord);
    
    frag_colour = blendOverlay(base, blend);
}