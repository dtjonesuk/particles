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

int s_DebugLogLevel = 4;
#define LOG_ERROR(x,y) std::cout << std::format(x,y) << std::endl;

static void OpenGLLogMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:
            if ((int)s_DebugLogLevel > 0)
            {
                LOG_ERROR("[OpenGL Debug HIGH] {0}", message);
//                if (s_DebugLogLevel == DebugLogLevel::HighAssert)
//                    GLCORE_ASSERT(false, "GL_DEBUG_SEVERITY_HIGH");
            }
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            if ((int)s_DebugLogLevel > 2)
                LOG_ERROR("[OpenGL Debug MEDIUM] {0}", message);
            break;
        case GL_DEBUG_SEVERITY_LOW:
            if ((int)s_DebugLogLevel > 3)
                LOG_ERROR("[OpenGL Debug LOW] {0}", message);
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            if ((int)s_DebugLogLevel > 4)
                LOG_ERROR("[OpenGL Debug NOTIFICATION] {0}", message);
            break;
    }
}

void enableGLDebugging()
{
    glDebugMessageCallback(OpenGLLogMessage, nullptr);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
}
