//
// Created by David on 29/05/2025.
//
#include <iostream>
#include <fstream>

#include "glad/glad.h"
#include "glutils.h"

void printOpenGLVersionInfo() {
    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *vendor = glGetString(GL_VENDOR);
    const GLubyte *version = glGetString(GL_VERSION);
    const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    std::cout << "GL Vendor : " << vendor << "\n";
    std::cout << "GL Renderer : " << renderer << "\n";
    std::cout << "GL Version (string) : " << version << "\n";
    std::cout << "GL Version (integer) : " << major << "." << minor << "\n";
    std::cout << "GLSL Version : " << glslVersion << std::endl;
}

std::string loadShaderSource(const std::filesystem::path &path) {
    std::ifstream file{path};
    std::stringstream ss;
    ss << file.rdbuf();
    
    return ss.str();
}
