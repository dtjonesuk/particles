#include <iostream>

#define GLAD_GL_IMPLEMENTATION

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <format>

#include "glutils.h"
#include "Window.h"
#include "ShaderProgram.h"
#include "Shader.h"

const int WIDTH = 1920;
const int HEIGHT = 1080;

void error_callback(int error, const char *description) {
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
    glfwWindowHint(GLFW_SAMPLES, 8);
    
    {
        // Create a GLFWwindow object that we can use for GLFW's functions
        Window window(WIDTH, HEIGHT, "LearnOpenGL");
        if (!window.Created()) {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return -1;
        }

        window.MakeContextCurrent();
        
        // Set error callback for GL functions
        glfwSetErrorCallback(error_callback);
        
        // Load OpenGL functions via Glad loader...
        int gladVersion = gladLoadGL();
        if (gladVersion == 0) {
            std::cerr << "Failed to initialize OpenGL context" << std::endl;
            return -1;
        }

        printOpenGLVersionInfo();
        
        if (glfwExtensionSupported("GL_ARB_gl_spirv")) {
            std::cout << "SPIR-V extension supported!" << std::endl;
        }
        
        Shader vert(GL_VERTEX_SHADER, loadShaderSource("shaders/vert.glsl"));
        Shader frag(GL_FRAGMENT_SHADER, loadShaderSource("shaders/frag.glsl"));
        ShaderProgram program(vert, frag); 
        
        window.Init();
        glfwSwapInterval( 1 );
        
        while (!window.ShouldClose()) {
            glfwPollEvents();
            program.UseProgram();
            window.Update();
            window.SwapBuffers();
        }
    }

    // Terminate GLFW
    glfwTerminate();

    return 0;
}
