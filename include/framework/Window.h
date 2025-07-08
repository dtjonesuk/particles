//
// Created by David on 30/05/2025.
//

#ifndef PARTICLES_WINDOW_H
#define PARTICLES_WINDOW_H

#include <stdexcept>
#include <map>

#ifdef WIN32
// Font support
#include <shlobj_core.h>
#include <filesystem>

#endif

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>




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
        enum WindowOption : int {
            OpenGLProfile = GLFW_OPENGL_PROFILE,
            OpenGLForwardCompat = GLFW_OPENGL_FORWARD_COMPAT,
            ContextVersionMajor = GLFW_CONTEXT_VERSION_MAJOR,
            ContextVersionMinor = GLFW_CONTEXT_VERSION_MINOR,
            OpenGLDebugContext = GLFW_OPENGL_DEBUG_CONTEXT,
            Resizable = GLFW_RESIZABLE,
            Visibe = GLFW_VISIBLE,
            Focused = GLFW_FOCUSED,
            Topmost = GLFW_FLOATING,
            Maximized = GLFW_MAXIMIZED,
            CenterCursor = GLFW_CENTER_CURSOR,
            DoubleBuffer = GLFW_DOUBLEBUFFER,
            TransparentFrameBuffer = GLFW_TRANSPARENT_FRAMEBUFFER,
            Samples = GLFW_SAMPLES,
            FocusOnShow = GLFW_FOCUS_ON_SHOW,
            ScaleToMonitor = GLFW_SCALE_TO_MONITOR,
        };

        static const std::map<WindowOption, int> DefaultWindowOptions;

        Window(int width, int height, std::string_view title) : _width(width), _height(height), _title(title) {}

        bool Create(const std::map<WindowOption, int> &options = DefaultWindowOptions) {
            InitGlfw();
            // Set all the required options for GLFW
            for (auto [option, value]: options)
                glfwWindowHint(option, value);

            _window = glfwCreateWindow(_width, _height, _title.data(), nullptr, nullptr);

            if (!_window) {
                return false;
            }

            glfwMakeContextCurrent(_window);

            // Set user pointer to this object so callbacks can reference us
            glfwSetWindowUserPointer(_window, this);

            glfwSetWindowCloseCallback(_window, &Window::close_handler);
            glfwSetKeyCallback(_window, &Window::keyboard_handler);
            glfwSetMouseButtonCallback(_window, &Window::mousebutton_handler);
            glfwSetCursorPosCallback(_window, &Window::mousepos_handler);
            glfwSetScrollCallback(_window, &Window::scroll_handler);

            return true;
        }

        bool Created() { return _window != nullptr; }

        void MakeContextCurrent() { glfwMakeContextCurrent(_window); }

        bool ShouldClose() { return glfwWindowShouldClose(_window); }

        // One time setup
        void InitOpenGL() {
#ifdef ENABLE_IMGUI
            InitGui();
#endif
            OnInit();
        }

        // Called once per frame
        void Update(TimeInfo timeInfo) {
#ifdef ENABLE_IMGUI
            UpdateGui(timeInfo);
#endif
            OnUpdate(timeInfo);
#ifdef ENABLE_IMGUI            
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
        }

        void SwapBuffers() { glfwSwapBuffers(_window); }

        void Destroy() {
            // Cleanup
#ifdef ENABLE_IMGUI
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
#endif
            glfwDestroyWindow(_window);
            OnClose();
            _window = nullptr;
        }

        virtual ~Window() {
            if (_window) {
                glfwDestroyWindow(_window);
            }
        }

    protected:
        void InitGlfw() {
            // Init GLFW
            glfwInit();
        }

#ifdef ENABLE_IMGUI
        void InitGui() {
            const char *glsl_version = "#version 330";
            // Setup Dear ImGui context
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO &io = ImGui::GetIO();
            (void) io;
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

            // Setup fonts
#ifdef WIN32
            char fontsFolder[MAX_PATH];

            SHGetSpecialFolderPathA(NULL, fontsFolder, CSIDL_FONTS, FALSE);
            std::filesystem::path fontPath(fontsFolder);
            fontPath /= "segoeui.ttf";
            io.Fonts->AddFontFromFileTTF(fontPath.string().c_str(), 28.0f);
#else
            ImFontConfig *fontConfig = new ImFontConfig();
            fontConfig->FontData = nullptr;
            fontConfig->SizePixels = 24.0f;
            io.Fonts->AddFontDefault(fontConfig);
#endif


            // Setup Dear ImGui style
            ImGui::StyleColorsDark();
            //ImGui::StyleColorsLight();

            // Setup Platform/Renderer backends
            ImGui_ImplGlfw_InitForOpenGL(_window, true);
            ImGui_ImplOpenGL3_Init(glsl_version);
        }

        void UpdateGui(const TimeInfo &timeInfo) {
            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            OnGui();

            // Rendering
            ImGui::Render();
        }
#endif

        virtual void OnGui() = 0;

        virtual void OnInit() = 0;

        virtual void OnUpdate(const TimeInfo &timeInfo) = 0;

        virtual void OnKeyboardEvent(int key, int scancode, int action, int mods) {
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
                glfwSetWindowShouldClose(_window, GLFW_TRUE);
        }

        virtual void OnMouseClick(int button, int action, int mods) {}

        virtual void OnMouseMove(double xpos, double ypos) {}
        
        virtual void OnMouseScroll(double xoffset, double yoffset) {}

        virtual void OnClose() {}

        GLFWwindow *_window{nullptr};
        int _width{}, _height{};
        std::string _title{};

    private:
        void KeyboardHandler(int key, int scancode, int action, int mods) {
            OnKeyboardEvent(key, scancode, action, mods);
        }

        void MouseButtonHandler(int button, int action, int mods) {
            OnMouseClick(button, action, mods);
        }

        void MousePositionHandler(double xpos, double ypos) {
            OnMouseMove(xpos, ypos);
        }
        
        void MouseScrollHandler(double xoffset, double yoffset) {
            OnMouseScroll(xoffset, yoffset);
        }

        void CloseWindowEvent() {
        }

        /// We store a pointer to the Window object in the user pointer of the GLFW window
        /// Then we call the member function of the respective Window object to handle the actual event.
        
        static void keyboard_handler(GLFWwindow *window, int key, int scancode, int action, int mods) {
            Window *window_obj = (Window *) glfwGetWindowUserPointer(window);
            if (window_obj) {
                // Ignore any inputs that Imgui is handling...
#ifdef ENABLE_IMGUI
                ImGuiIO &io = ImGui::GetIO();
                if (!io.WantCaptureKeyboard)
#endif
                    window_obj->KeyboardHandler(key, scancode, action, mods);
            }
        }

        static void mousebutton_handler(GLFWwindow *window, int button, int action, int mods) {
            Window *window_obj = (Window *) glfwGetWindowUserPointer(window);
            if (window_obj) {
                // Ignore any inputs that Imgui is handling...
#ifdef ENABLE_IMGUI
                ImGuiIO &io = ImGui::GetIO();
                if (!io.WantCaptureMouse)
#endif
                    window_obj->MouseButtonHandler(button, action, mods);
            }
        }

        static void mousepos_handler(GLFWwindow *window, double xpos, double ypos) {
            Window *window_obj = (Window *) glfwGetWindowUserPointer(window);
            if (window_obj) {
                // Ignore any inputs that Imgui is handling...
#ifdef ENABLE_IMGUI
                ImGuiIO &io = ImGui::GetIO();
                if (!io.WantCaptureMouse)
#endif
                    window_obj->MousePositionHandler(xpos, ypos);
            }
        }

        static void close_handler(GLFWwindow *window) {
            Window *window_obj = (Window *) glfwGetWindowUserPointer(window);
            if (window_obj) {
                window_obj->CloseWindowEvent();
            }
        }
        
        static void scroll_handler(GLFWwindow *window, double xoffset, double yoffset) {
            Window *window_obj = (Window *) glfwGetWindowUserPointer(window);
            if (window_obj) {
                // Ignore any inputs that Imgui is handling...
#ifdef ENABLE_IMGUI
                ImGuiIO &io = ImGui::GetIO();
                if (!io.WantCaptureMouse)
#endif
                    window_obj->MouseScrollHandler(xoffset, yoffset);
            }
        }
    };

}
#endif //PARTICLES_WINDOW_H
