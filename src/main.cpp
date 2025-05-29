#include <iostream>

#define GLAD_GL_IMPLEMENTATION

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <format>

const int WIDTH = 1920;
const int HEIGHT = 1080;

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

void error_callback(int error, const char* description)
{
    std::cerr << std::format("Error: {} {}\n", error, description);
}

int main() {
    // Init GLFW
    glfwInit();
    
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Set error callback for GL functions
    glfwSetErrorCallback(error_callback);
    
    // Load OpenGL functions via Glad loader...
    int gladVersion = gladLoadGL();
    if (gladVersion == 0) {
        std::cerr << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }

    printOpenGLVersionInfo();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
    
    // Terminate GLFW
    glfwTerminate();
    return 0;
}
