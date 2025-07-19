//
// Created by David on 02/06/2025.
//

#ifndef PARTICLES_DEMOWINDOW_H
#define PARTICLES_DEMOWINDOW_H

#include <numbers>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "glm/gtc/type_ptr.hpp"

//#include "glutils.h"
#include "framework/Window.h"
#include "framework/Buffer.h"
#include "framework/ShaderProgram.h"
#include "framework/Camera.h"
#include "framework/Light.h"
#include "framework/StaticMesh.h"
#include "framework/InstancedMesh.h"
#include "framework/Wireframe.h"
#include "framework/Scene.h"

#include <glad/glad.h>

#include <stb_image.h>

using namespace framework;

#define PI std::numbers::pi_v<float>


class DemoWindow : public Window {
public:
    DemoWindow() : Window(1920, 1080, "Particle System") {}

protected:
    std::shared_ptr<Scene> scene;
    std::shared_ptr<SceneEntity> sphere, cube, wireframe;

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
        float specularAmount{8.0f};

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

        scene = std::make_shared<Scene>();
        scene->MakePerspectiveCamera({0.f, 2.f, 5.f}, {0, 0, 0}, {0, 1, 0}, {1920, 1080});


        // Unlit materials
        auto white_unlit = std::make_shared<UnlitMaterial>("white", glm::vec3{1.0, 1.0, 1.0f});

        // Standard materials
        auto emerald_solid = std::make_shared<SolidMaterial>("emerald",
                                                             glm::vec3{0.0215, 0.1745, 0.0215},
                                                             glm::vec3{0.07568, 0.61424, 0.07568},
                                                             glm::vec3{0.633, 0.727811, 0.633},
                                                             0.6f * 128);

        auto bronze_solid = std::make_shared<SolidMaterial>("pearl",
                                                                 glm::vec3{0.25, 0.20725, 0.20725},
                                                                 glm::vec3{1, 0.829, 0.829},
                                                                 glm::vec3{0.96648, 0.296648, 0.296648},
                                                                 0.088f * 128);
        // Geometry
        Transform transform;

        // Sphere
        auto sphere_geo = std::make_shared<StaticMesh>(StaticMesh::CreateSphere(32, 16));
        sphere_geo->SetMaterial(emerald_solid);
        sphere = scene->AddEntity<SceneEntity>(scene, transform, sphere_geo);

        // Cube
        auto cube_geo = std::make_shared<StaticMesh>(StaticMesh::CreateCube());
        cube_geo->SetMaterial(bronze_solid);
        transform.Translate({3.0, 0, 0});
        transform.Rotate(glm::vec3(glm::radians(30.0f), glm::radians(30.0f), glm::radians(30.0f)));
        cube = scene->AddEntity<SceneEntity>(scene, transform, cube_geo);

        // Wireframe Box
        auto wireframe_geo = std::make_shared<WireframeMesh>(WireframeMesh::CreateCube());
        wireframe_geo->SetMaterial(white_unlit);
        transform.Reset();
        transform.SetScale({2, 2, 2});
        wireframe = scene->AddEntity<SceneEntity>(scene, transform, wireframe_geo);
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
        ImGui::SliderFloat("Specular", &Gui.specularAmount, 0.0f, 64.0f);

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
        // Sync scene entities with the GUI
        
        sphere->SetLocalTransform({{Gui.translation.x, Gui.translation.y, 0.f},
                                   glm::vec3(glm::radians(Gui.rotationEuler.x),
                                             glm::radians(Gui.rotationEuler.y),
                                             glm::radians(Gui.rotationEuler.z)),
                                   Gui.scale});

        int width, height;
        glfwGetFramebufferSize(_window, &width, &height);
        scene->GetCamera()->SetViewport({width, height});
        scene->GetCamera()->SetFov(Gui.fov);
        Gui.camera_position = scene->GetCamera()->GetPosition();

        scene->GetLight().position = Gui.light_position;
        
        // Draw scene geometry
        scene->DrawScene();
        
    }

    void OnClose() override {
        std::cout << "Close window" << std::endl;
    }

    double prevX{}, prevY{};

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
            scene->GetCamera()->Orbit(-deltaX / MOUSE_SENSITIVITY, -deltaY / MOUSE_SENSITIVITY);
        } else if (bPanning) {
            scene->GetCamera()->Pan({-deltaX / MOUSE_SENSITIVITY, deltaY / MOUSE_SENSITIVITY});
        } else if (bZooming) {
            scene->GetCamera()->Zoom(deltaY / MOUSE_SENSITIVITY);
        }

        prevX = xpos;
        prevY = ypos;
    }

    void OnMouseScroll(double xoffset, double yoffset) override {
        if (!(bOrbiting || bPanning || bZooming)) {
            scene->GetCamera()->Zoom(yoffset / SCROLL_SENSITIVITY);
        }
    }
};


#endif //PARTICLES_DEMOWINDOW_H
