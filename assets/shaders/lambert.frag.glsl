#version 430 core

in vec3 vertNormal;
in vec3 vertFragPos;
in vec2 vertUV;

out vec4 fragColor;

layout(std140) uniform Light {
    vec3 position;
    vec3 colour;
    float ambientIntensity;
} light;

layout(std140) uniform Material {
    vec3 diffuseColour;
    float specularAmount;
} material;

uniform sampler2D u_Texture;


void main() {
    vec3 norm = normalize(vertNormal);
    vec3 lightDir = normalize(light.position - vertFragPos);
    float diff = max(dot(norm, lightDir), 0);

    vec3 diffuse =  min(light.ambientIntensity + diff, 1.0) * light.colour * material.diffuseColour;
    fragColor = vec4(diffuse, 1.0);
}