#version 330 core
in vec3 pos;
in vec3 normal;
in vec3 color;

out vec3 vertColor;
out vec3 vertNormal;
out vec3 vertFragPos;
//out vec2 texCoord;

uniform vec3 u_Color;
uniform mat4x4 u_Normal;
uniform mat4x4 u_Model;
uniform mat4x4 u_MVP;

void main() {
    gl_Position = u_MVP * vec4(pos, 1.0);
    vertColor = color;
    vertNormal =  mat3(u_Normal) * normal;
    vertFragPos = vec3(u_Model * vec4(pos, 1.0));
    
//    texCoord = uv;
}