#version 430 core

in vec3 pos;
in vec3 normal;
in vec2 texcoord;

out vec3 vertColor;
out vec3 vertNormal;
out vec3 vertFragPos;
out vec2 vertTexcoord;

layout (std140) uniform World {
    mat4 view;
    mat4 projection;
    vec3 cameraPosition; 
} world;

layout (std140) uniform Object {
    mat4 model;
    mat4 mvp;
    mat4 normal;
} object;
  
void main() {
    gl_Position = object.mvp  * vec4(pos, 1.0);
    vertNormal =  vec3(object.normal * vec4(normal, 0.0));
    vertFragPos = vec3(object.model * vec4(pos, 1.0));
    vertTexcoord = texcoord;
}