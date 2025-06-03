//
// Created by David on 30/05/2025.
//

#ifndef PARTICLES_WINDOW_H
#define PARTICLES_WINDOW_H

#include <stdexcept>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"


namespace framework {
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
        Window(int width, int height, std::string_view title) {
            if (g_window != nullptr) {
                throw std::runtime_error("Only one Window object is supported!");
            }
            g_window = this;
            window = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
            if (window) {
                glfwMakeContextCurrent(window);
                glfwSetWindowCloseCallback(window, &Window::close_handler);
                glfwSetKeyCallback(window, &Window::keyboard_handler);
                glfwSetMouseButtonCallback(window, &Window::mousebutton_handler);
            }
        }

        bool Created() { return window != nullptr; }

        void MakeContextCurrent() { glfwMakeContextCurrent(window); }

        bool ShouldClose() { return glfwWindowShouldClose(window); }

        // One time setup
        void Init() { OnInit(); }

        // Called once per frame
        void Update(TimeInfo timeInfo) { OnUpdate(timeInfo); }

        void SwapBuffers() { glfwSwapBuffers(window); }

        void Destroy() {
            glfwDestroyWindow(window);
            OnClose();
            window = nullptr;
        }
        
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

        virtual void OnClose() {}

        GLFWwindow *window{nullptr};
        
    private:
        void KeyboardHandler(int key, int scancode, int action, int mods) {
            OnKeyboardEvent(key, scancode, action, mods);
        }

        void MouseButtonHandler(int button, int action, int mods) {
            OnMouseEvent(button, action, mods);
        }

        void CloseWindowEvent() {
        }

        static Window *g_window;

        static void keyboard_handler(GLFWwindow *window, int key, int scancode, int action, int mods) {
            if (g_window && g_window->window == window)
                g_window->KeyboardHandler(key, scancode, action, mods);
        }

        static void mousebutton_handler(GLFWwindow *window, int button, int action, int mods) {
            if (g_window && g_window->window == window)
                g_window->MouseButtonHandler(button, action, mods);
        }

        static void close_handler(GLFWwindow *window) {
            if (g_window && g_window->window == window) {
                g_window->CloseWindowEvent();
            }
        }
    };

}
#endif //PARTICLES_WINDOW_H
