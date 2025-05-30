//
// Created by David on 30/05/2025.
//

#ifndef PARTICLES_WINDOW_H
#define PARTICLES_WINDOW_H

#include <GLFW/glfw3.h>

class Window {
public:
    Window(int width, int height, const char* title) {
        window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
        g_window = this;
        if (window) {
            glfwMakeContextCurrent(window);
            glfwSetKeyCallback(window, &Window::keyboard_handler);
            glfwSetMouseButtonCallback(window, &Window::mousebutton_handler);
        }
    }
    
    bool Created() { return window != nullptr; }
    void MakeContextCurrent() { glfwMakeContextCurrent(window); }
    bool ShouldClose() { return glfwWindowShouldClose(window); }
    void Update() {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        
    }
    void SwapBuffers() { glfwSwapBuffers(window); }
    
    virtual ~Window() {
        if (window) {
            glfwDestroyWindow(window);
        }
    }
protected:
    void KeyboardHandler(int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    
    void MouseButtonHandler(int button, int action, int mods) {
        
    }
private:
    GLFWwindow* window{nullptr};
    
    static Window* g_window;
    
    static void keyboard_handler(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (g_window && g_window->window == window)
            g_window->KeyboardHandler(key, scancode, action, mods);
    }
    
    static void mousebutton_handler(GLFWwindow* window, int button, int action, int mods) {
        if (g_window && g_window->window == window)
            g_window->MouseButtonHandler(button, action, mods);
    }
};


#endif //PARTICLES_WINDOW_H
