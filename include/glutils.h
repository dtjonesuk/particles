//
// Created by David on 29/05/2025.
//

#ifndef PARTICLES_GLUTILS_H
#define PARTICLES_GLUTILS_H
#include <filesystem>

void printOpenGLVersionInfo();
std::string loadShaderSource(const std::filesystem::path &path);
void enableGLDebugging();

#endif //PARTICLES_GLUTILS_H
