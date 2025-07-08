#include <iostream>

#define GLAD_GL_IMPLEMENTATION

#define ENABLE_IMGUI

#include "glutils.h"
#include "DemoWindow.h"


void error_callback(int error, const char *description) {
    std::cerr << std::format("Error: {} {}\n", error, description);
}

int main() {

    // Create a GLFWwindow object that we can use for GLFW's functions
    DemoWindow window;
    if (!window.Create()) {
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

    enableGLDebugging();

    printOpenGLVersionInfo();

    if (glfwExtensionSupported("GL_ARB_gl_spirv")) {
        std::cout << "SPIR-V extension supported!" << std::endl;
    }


    window.InitOpenGL();

    while (!window.ShouldClose()) {
        // Handle windowing events            
        glfwPollEvents();

        // Draw the frame
        TimeInfo timeInfo;
        window.Update(timeInfo);
        window.SwapBuffers();
    }
    window.Destroy();
    

    // Terminate GLFW
    glfwTerminate();

    return 0;
}
