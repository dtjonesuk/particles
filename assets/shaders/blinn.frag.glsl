#version 430 core

in vec3 vertNormal;
in vec3 vertFragPos;
in vec2 vertUV;

out vec4 fragColor;

layout (std140) uniform World {
    mat4 view;
    mat4 projection;
    vec3 cameraPosition;
} world;

layout (std140) uniform Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} light;

layout(std140) uniform Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
} material;

uniform sampler2D u_Texture;

void main() {
    vec3 norm = normalize(vertNormal);
    vec3 lightDir = normalize(light.position - vertFragPos);
    vec3 viewDir = normalize(world.cameraPosition - vertFragPos);

    vec3 ambient = light.ambient * material.ambient;

    float diff = max(dot(norm, lightDir), 0);
    vec3 diffuse =  min(diff, 1.0) * light.diffuse * material.diffuse;

    // Phong model for now
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * light.specular * material.specular;

    fragColor = vec4(ambient + diffuse + specular, 1.0);
//    fragColor = vec4(vertNormal, 1.0);
}