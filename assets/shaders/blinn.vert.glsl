#version 430 core
in vec3 pos;
in vec3 normal;
in vec2 uv;

out vec3 vertColor;
out vec3 vertNormal;
out vec3 vertFragPos;
out vec2 vertUV;

layout (std140) uniform Matrices {
    mat4x4 model;
    mat4x4 view;
    mat4x4 projection;
    mat4x4 normal;
} mats;
  
void main() {
    gl_Position = mats.projection * mats.view * mats.model  * vec4(pos, 1.0);
    vertNormal =  vec3(mats.normal * vec4(normal,1));
    vertFragPos = vec3(mats.model * vec4(pos, 1.0));
    vertUV = uv;
}