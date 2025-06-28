//
// Created by David on 30/05/2025.
//
#include "framework/Window.h"

namespace framework {
    double TimeInfo::previousTime{0.f};

    const std::map<Window::WindowOption, int> Window::DefaultWindowOptions{
            {ContextVersionMajor, 4},
            {ContextVersionMinor, 6},
            {OpenGLProfile, GLFW_OPENGL_CORE_PROFILE},
            {OpenGLForwardCompat, GL_TRUE},
            {OpenGLDebugContext, GL_TRUE},
            {Resizable, GL_FALSE},
            {Samples, 8}
    };
}