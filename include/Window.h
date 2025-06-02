//
// Created by David on 30/05/2025.
//

#ifndef PARTICLES_WINDOW_H
#define PARTICLES_WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

struct TimeInfo {
    TimeInfo() {
        currentTime = glfwGetTime();
        if (previousTime == 0.f) {
            deltaTime = 0.f;
        } else {
            deltaTime = currentTime - previousTime;
        }
        previousTime = currentTime;
    }

    double currentTime;
    double deltaTime;
private:
    static double previousTime;
};

class Window {
public:
    Window(int width, int height, const char *title) {
        window = glfwCreateWindow(width, height, "LearnOpenGL", nullptr, nullptr);
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
        OnInit();
    }

    // Called once per frame
    void Update(TimeInfo timeInfo) {
        OnUpdate(timeInfo);

    }

    void SwapBuffers() { glfwSwapBuffers(window); }

    virtual ~Window() {
        if (window) {
            glfwDestroyWindow(window);
        }
    }

protected:
    virtual void OnInit() = 0;

    virtual void OnUpdate(TimeInfo timeInfo) = 0;

    virtual void OnKeyboardEvent(int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    virtual void OnMouseEvent(int button, int action, int mods) {}

    void KeyboardHandler(int key, int scancode, int action, int mods) {
        OnKeyboardEvent(key, scancode, action, mods);
    }

    void MouseButtonHandler(int button, int action, int mods) {
        OnMouseEvent(button, action, mods);
    }

    GLFWwindow *window{nullptr};

private:
    static Window *g_window;

    static void keyboard_handler(GLFWwindow *window, int key, int scancode, int action, int mods) {
        if (g_window && g_window->window == window)
            g_window->KeyboardHandler(key, scancode, action, mods);
    }

    static void mousebutton_handler(GLFWwindow *window, int button, int action, int mods) {
        if (g_window && g_window->window == window)
            g_window->MouseButtonHandler(button, action, mods);
    }
};


#endif //PARTICLES_WINDOW_H
