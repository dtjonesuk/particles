#version 330 core
in vec3 vertColor;
in vec3 vertNormal;
in vec3 vertFragPos;
//in vec2 texCoord;

out vec4 fragColor;

uniform float u_AmbientIntensity;
uniform vec3 u_LightPos;
uniform vec3 u_LightColor;
uniform vec3 u_ObjectColor;

uniform sampler2D u_Texture;

void main() {
    //    vec3 default_col = vec3(1.0, 1.0, 1.0);
    //    frag_colour = vec4(mix(color, default_col, u_Percent), 1.0);
    //    vec4 base =vec4(color, 1.0);
    //    vec4 blend = texture(u_Texture, texCoord);

    vec3 ambient = u_AmbientIntensity * u_LightColor;
    
    vec3 norm = normalize(vertNormal);
    vec3 lightDir = normalize(u_LightPos - vertFragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_LightColor;
    
    fragColor = vec4((ambient + diffuse)*u_ObjectColor, 1.0);
//    fragColor = vec4(vertColor, 1.0);
}