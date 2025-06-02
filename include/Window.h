//
// Created by David on 30/05/2025.
//

#ifndef PARTICLES_WINDOW_H
#define PARTICLES_WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

const glm::vec3 points[] = {
        {0.f, 0.5f, 0.f},
        {0.5f, -0.5f, 0.f},
        {-0.5f, -0.5f, 0.f}
};

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
    
    // One time setup
    void Init() {
        // Create 1 buffer to store points
        glGenBuffers(1, &vbo);
        // Bind buffer to current context
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        // Set buffer data (send to gpu) - 3 pts * 3 co-ords each * sizeof(float).
        glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(points[0]), &points[0], GL_STATIC_DRAW);
        

        // Create 1 vertex array object
        glGenVertexArrays(1, &vao);
        // Bind vertex array to current context
        glBindVertexArray(vao);
        // Enable attrib #0
        glEnableVertexAttribArray(0);
        // Bind vertex buffer to vertex array object
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        // Attrib #0: 3 floats, non-normalized, stride 0
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        
    }
    
    // Called once per frame
    void Update() {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        
        glClearColor(0.1f, 0.1f, 0.1f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        
    }
    void SwapBuffers() { glfwSwapBuffers(window); }
    
    virtual ~Window() {
        if (window) {
            glfwDestroyWindow(window);
        }
    }
protected:
    GLuint vbo = 0;
    GLuint vao = 0;
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
