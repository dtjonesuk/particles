#version 330 core
in vec3 pos;
in vec3 color;

out vec3 vertColor;

layout (std140) uniform World {
    mat4x4 view;
    mat4x4 projection;
    vec3 cameraPosition;
} world;

layout (std140) uniform Object {
    mat4x4 model;
    mat4x4 mvp;
    mat4x4 normal;
} object;
  
void main() {
    gl_Position = world.projection * world.view * object.model * vec4(pos, 1.0);
    vertColor = color;
}