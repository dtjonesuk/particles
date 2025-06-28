//
// Created by David on 02/06/2025.
//

#ifndef PARTICLES_DEMOWINDOW_H
#define PARTICLES_DEMOWINDOW_H

#include <numbers>

#include <stb_image.h>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "glm/gtc/type_ptr.hpp"

#include "glutils.h"
#include "framework/Window.h"
#include "framework/Buffer.h"
#include "framework/ShaderProgram.h"
#include "framework/StaticMesh.h"
#include "framework/Camera.h"
#include "framework/Light.h"
#include "framework/Material.h"
#include "framework/MatrixBuffer.h"

using namespace framework;

#define PI std::numbers::pi_v<float>
#define LIGHT_BINDING_POINT 1
#define MATERIAL_BINDING_POINT 2
#define MATRICES_BINDING_POINT 3

class DemoWindow : public Window {
public:
    DemoWindow() : Window(1920, 1080, "Particle System") {}

protected:
    std::unique_ptr<StaticMesh> cube;
    std::unique_ptr<StaticMesh> sphere;
    std::unique_ptr<ShaderProgram> solid_shader;
    std::unique_ptr<ShaderProgram> unlit_shader;
    std::unique_ptr<PerspectiveCamera> camera;

    Buffer<GL_UNIFORM_BUFFER> light_ubo;
    Buffer<GL_UNIFORM_BUFFER> material_ubo;
    Buffer<GL_UNIFORM_BUFFER> matrices_ubo;

    double previous = 0;
    bool bOrbiting = false;
    bool bPanning = false;
    bool bZooming = false;

    const float MOUSE_SENSITIVITY = 500.0f;
    const float SCROLL_SENSITIVITY = 10.0f;

    struct {
        bool showDemoWindow = false;

        // Object properties
        glm::vec3 rotationEuler{0.0f, 0.0f, 0.0f};
        glm::vec2 translation{0.f};
        glm::vec3 scale{1.0f, 1.0f, 1.0f};
        glm::vec3 colour{0.0f, 0.5f, 1.0f};

        // Light properties
        float ambient_light_intensity = 0.2f;
        glm::vec3 light_position{0.0, 5.0, 2.0};
        glm::vec3 light_colour{1.0f, 1.0f, 1.0f};

        // Camera properties
        glm::vec3 camera_position{};
        float fov{45.0f};
    } Gui;

    void OnInit() override {
        // Limit FPS
        glfwSwapInterval(1);

        Texture texture = Texture::CreateFromFile("assets/textures/opengl-logo.png");

        // Geometry
        sphere = std::make_unique<StaticMesh>(CreateSphere(32, 16));
        cube = std::make_unique<StaticMesh>(CreateCube());

        //mesh->SetTexture(std::move(texture));

        camera = std::make_unique<PerspectiveCamera>(
                PerspectiveCamera({0.f, 2.f, 5.f}, {0, 0, 0}, {0, 1, 0}, {1920, 1080}));

        // Compile shaders and create shader program
        unlit_shader = std::make_unique<ShaderProgram>(ShaderProgram(loadShaderSource("assets/shaders/unlit.vert.glsl"),
                                                                     loadShaderSource(
                                                                             "assets/shaders/unlit.frag.glsl")));

        solid_shader = std::make_unique<ShaderProgram>(
                ShaderProgram(loadShaderSource("assets/shaders/lambert.vert.glsl"),
                              loadShaderSource("assets/shaders/lambert.frag.glsl")));


        GLint size;

        glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &size);
        std::cout << "Uniform buffer bindings: " << size << std::endl;

        light_ubo.Generate();
        light_ubo.Bind();
        size = solid_shader->GetUniformBlockSize("Light");
        std::cout << "Light uniform size: " << size << "(" << sizeof(Light) << ")" << std::endl;
        light_ubo.BufferData(size, nullptr, GL_DYNAMIC_DRAW);
        light_ubo.BindToBindingPoint(LIGHT_BINDING_POINT);
        light_ubo.Unbind();

        material_ubo.Generate();
        material_ubo.Bind();
        size = solid_shader->GetUniformBlockSize("Material");
        std::cout << "Material uniform size: " << size << "(" << sizeof(Material) << ")" << std::endl;
        material_ubo.BufferData(size, nullptr, GL_DYNAMIC_DRAW);
        material_ubo.BindToBindingPoint(MATERIAL_BINDING_POINT);
        material_ubo.Unbind();

        matrices_ubo.Generate();
        matrices_ubo.Bind();
        size = solid_shader->GetUniformBlockSize("Matrices");
        std::cout << "Matrices uniform size: " << size << "(" << sizeof(MatrixBuffer) << ")" << std::endl;
        matrices_ubo.BufferData(size, nullptr, GL_DYNAMIC_DRAW);
        matrices_ubo.BindToBindingPoint(MATRICES_BINDING_POINT);
        matrices_ubo.Unbind();

        solid_shader->UniformBlockBinding("Material", MATERIAL_BINDING_POINT);
        solid_shader->UniformBlockBinding("Light", LIGHT_BINDING_POINT);
        solid_shader->UniformBlockBinding("Matrices", MATRICES_BINDING_POINT);
        
        // Enable z-buffer
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glm::vec2 range;
        glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, &range[0]);
        std::cout << "Line width range: " << range[0] << " - " << range[1] << std::endl;
        glDisable(GL_LINE_SMOOTH);
    }

    void OnGui() override {
        if (Gui.showDemoWindow)
            ImGui::ShowDemoWindow(&Gui.showDemoWindow);

        ImGui::Begin("Particle System", nullptr/*,  ImGuiWindowFlags_MenuBar*/);
        ImGui::LabelText("ALT + left mouse", "Orbit camera");
        ImGui::LabelText("ALT + middle mouse", "Pan camera");
        ImGui::LabelText("ALT + right mouse", "Zoom camera");
        ImGui::LabelText("Scrollwheel", "Zoom camera");

        ImGui::Spacing();
        ImGui::SeparatorText("Object");
        ImGui::SliderFloat2("Position##Object", &Gui.translation[0], -5.0f, 5.0f);
        ImGui::SliderFloat3("Scale##Object", &Gui.scale[0], 0.0f, 5.0f);
        ImGui::SliderFloat3("Rotation##Object", &Gui.rotationEuler[0], 0.0f, 360.0f);
        ImGui::ColorEdit3("Colour##Object", &Gui.colour[0]);

        ImGui::Spacing();
        ImGui::SeparatorText("Camera");
        ImGui::InputFloat3("Position##Camera", &Gui.camera_position[0]);
        ImGui::SliderFloat("FOV", &Gui.fov, 0.0f, 90.0f);

        ImGui::Spacing();
        ImGui::SeparatorText("Light");
        ImGui::SliderFloat3("Position##Light", &Gui.light_position[0], -10.0f, 10.0f);
        ImGui::ColorEdit3("Colour##Light", &Gui.light_colour[0]);
        ImGui::SliderFloat("Ambient", &Gui.ambient_light_intensity, 0.0f, 1.0f);
        ImGui::End();
    }

    void OnUpdate(const TimeInfo &time) override {
        // Calculate fractional part of time
        float percent = (cos((float) time.currentTime / 5 * 2 * PI) + 1.f) / 2.f;

        // Set shaders and uniform values
        solid_shader->Use();

        {
            Light light(Gui.light_position, Gui.light_colour, Gui.ambient_light_intensity);
            light_ubo.Bind();
            light_ubo.BindToBindingPoint(LIGHT_BINDING_POINT);
            light_ubo.BufferSubData(0, sizeof(Light), &light);
            light_ubo.Unbind();
        }

        {
            Material material(Gui.colour, 1.f);
            material_ubo.Bind();
            material_ubo.BindToBindingPoint(MATERIAL_BINDING_POINT);
            material_ubo.BufferSubData(0, sizeof(Material), &material);
            material_ubo.Unbind();
        }

        // Model transform
        sphere->SetTransform({{Gui.translation.x, Gui.translation.y, 0.f},
                              glm::vec3(glm::radians(Gui.rotationEuler.x), 
                                        glm::radians(Gui.rotationEuler.y),
                                        glm::radians(Gui.rotationEuler.z)),
                              Gui.scale
                             });

        MatrixBuffer matrices;
        matrices.model = sphere->GetTransform().Matrix();

        // View transform
        int width, height;
        glfwGetFramebufferSize(_window, &width, &height);
        glViewport(0, 0, width, height);

        camera->SetViewport({width, height});
        camera->SetFov(Gui.fov);

        Gui.camera_position = camera->GetPosition();

        matrices.view = camera->GetViewMatrix();
        matrices.projection = camera->GetProjectionMatrix();
        matrices.normal = glm::transpose(glm::inverse(matrices.model));

        matrices_ubo.Bind();
        matrices_ubo.BindToBindingPoint(MATRICES_BINDING_POINT);
        matrices_ubo.BufferSubData(0, sizeof(MatrixBuffer), &matrices);
        matrices_ubo.Unbind();
        

        // Clear screen
        glClearColor(.1f, .1f, .1f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw geometry
//        cube->Render();
        sphere->Render();

//        unlit_shader->Use();
//        unlit_shader->SetUniform("u_MVP", mvp);
//        unlit_shader->SetUniform("u_Color", glm::vec3{0.f, 0.f, 0.f});
//        glDisable(GL_LINE_SMOOTH);
//        glLineWidth(2.0f);
//        sphere->RenderWireframe();
        solid_shader->Unuse();

    }

    void OnClose() override {
        std::cout << "Close window" << std::endl;
    }

    double prevX, prevY;

    void OnMouseClick(int button, int action, int mods) override {
        if (action == GLFW_PRESS && (mods & GLFW_MOD_ALT)) {
            glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            switch (button) {
                case GLFW_MOUSE_BUTTON_1:
                    bOrbiting = true;
                    bPanning = false;
                    bZooming = false;

                    glfwGetCursorPos(_window, &prevX, &prevY);
                    if (glfwRawMouseMotionSupported()) {
                        glfwSetInputMode(_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
                    }
                    break;
                case GLFW_MOUSE_BUTTON_2:
                    bOrbiting = false;
                    bPanning = false;
                    bZooming = true;

                    glfwGetCursorPos(_window, &prevX, &prevY);
                    if (glfwRawMouseMotionSupported()) {
                        glfwSetInputMode(_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
                    }
                    break;
                case GLFW_MOUSE_BUTTON_3:
                    bOrbiting = false;
                    bPanning = true;
                    bZooming = false;

                    glfwGetCursorPos(_window, &prevX, &prevY);
                    if (glfwRawMouseMotionSupported()) {
                        glfwSetInputMode(_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
                    }
                    break;
                default:
                    std::cout << "Mouse Button (unknown) pressed." << std::endl;
            }
        } else if (action == GLFW_RELEASE) {
            bOrbiting = false;
            bPanning = false;
            bZooming = false;

            glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetInputMode(_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
        }
    }

    void OnMouseMove(double xpos, double ypos) override {
        double deltaX = xpos - prevX;
        double deltaY = ypos - prevY;

        if (bOrbiting) {
            camera->Orbit(-deltaX / MOUSE_SENSITIVITY, -deltaY / MOUSE_SENSITIVITY);
        } else if (bPanning) {
            camera->Pan({-deltaX / MOUSE_SENSITIVITY, deltaY / MOUSE_SENSITIVITY});
        } else if (bZooming) {
            camera->Zoom(deltaY / MOUSE_SENSITIVITY);
        }

        prevX = xpos;
        prevY = ypos;
    }

    void OnMouseScroll(double xoffset, double yoffset) override {
        if (!(bOrbiting || bPanning || bZooming)) {
            camera->Zoom(yoffset / SCROLL_SENSITIVITY);
        }
    }
};


#endif //PARTICLES_DEMOWINDOW_H
