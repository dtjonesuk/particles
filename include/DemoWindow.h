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

#include "glutils.h"
#include "framework/Window.h"
#include "framework/ShaderProgram.h"
#include "glm/gtc/type_ptr.hpp"
#include "framework/Mesh.h"

using namespace framework;

#define PI std::numbers::pi_v<float>

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 uv;
};

const std::vector<Vertex> points = {
        {{0.f,   sinf(glm::radians(60.f)) - 0.5f, 0.f},
                {1.0f, 0.f,  0.f},  {0.5f,  1.5f}},
        {{0.5f,  -0.5f,                           0.f},
                {0.f,  1.0f, 0.f},  {-.5f, 0.f}},
        {{-0.5f, -0.5f,                           0.f},
                {0.f,  0.f,  1.0f}, {1.5f,  0.f}}
};

class DemoWindow : public Window {
public:
    DemoWindow() : Window(1920, 1080, "LearnOpenGL") {}

protected:
    Mesh<Vertex> mesh;
    Texture texture;
    ShaderProgram program;
    double previous = 0;
    GLint u_Color{0};
    GLint u_Percent{0};
    GLint u_Transform{0};
    GLint u_Texture{0};
    struct {
        bool showDemoWindow = true;
        float scale = 1.0f;
        float rotation = 0.f;
        glm::vec2 translation{0.f};
        glm::vec3 colour{0.1f, 0.1f, 0.1f};
    } Gui;

    void OnInit() override {
        // Limit FPS
        glfwSwapInterval(1);

        texture.CreateFromFile("assets/textures/opengl-logo.png");

        // Geometry
        mesh = Mesh<Vertex>::CreateFromPoints(points);
        mesh.AddAttrib(3, GL_FLOAT, offsetof(Vertex, pos));
        mesh.AddAttrib(3, GL_FLOAT, offsetof(Vertex, color));
        mesh.AddAttrib(2, GL_FLOAT, offsetof(Vertex, uv));
        mesh.SetTexture(std::move(texture));

        // Compile shaders and create shader program
        Shader vert(GL_VERTEX_SHADER, loadShaderSource("assets/shaders/2d.vert.glsl"));
        Shader frag(GL_FRAGMENT_SHADER, loadShaderSource("assets/shaders/2d.frag.glsl"));

        program = ShaderProgram(vert, frag);
        program.UseProgram();
    }

    void OnGui() override {
        if (Gui.showDemoWindow)
            ImGui::ShowDemoWindow(&Gui.showDemoWindow);
        ImGui::Begin("Particle System", nullptr/*,  ImGuiWindowFlags_MenuBar*/);
        ImGui::Text("dear imgui says hello! (%s) (%d)", IMGUI_VERSION, IMGUI_VERSION_NUM);
        ImGui::Spacing();
        ImGui::SeparatorText("Transform");
        ImGui::SliderFloat2("Translation", &Gui.translation[0], -2.0f, 2.0f);        
        ImGui::SliderFloat("Scale", &Gui.scale, 0.0f, 5.0f);        
        ImGui::SliderFloat("Rotation", &Gui.rotation, 0.0f, 360.0f);
        
        ImGui::Spacing();
        ImGui::SeparatorText("Background");
        ImGui::ColorPicker3("Colour", &Gui.colour[0]);
        ImGui::End();
    }

    void OnUpdate(const TimeInfo &time) override {
        // Calculate fractional part of time
        float percent = (cos((float) time.currentTime / 5 * 2 * PI) + 1.f) / 2.f;

        // Set shaders and uniform values
        program.UseProgram();
        program.SetUniform<float>("u_Percent", percent);

        // Model transform
        auto model = glm::identity<glm::mat4>();
//        model = glm::rotate(model, (float) time.currentTime * PI * 2, glm::vec3{0.f, 0.f, 1.f});
//        model = glm::scale(model, glm::vec3{2, 2, 2});
        model = glm::translate(model, glm::vec3(Gui.translation.x, Gui.translation.y, 0.f));
        model = glm::rotate(model, glm::radians(Gui.rotation), glm::vec3{0.f, 0.f, 1.f});
        model = glm::scale(model, glm::vec3{Gui.scale, Gui.scale, Gui.scale});

        // View transform
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        auto eye = glm::vec4(0.f, 2.f, -5.f, 1.f);
        eye = glm::rotate(glm::identity<glm::mat4>(), glm::radians(360.f * percent), glm::vec3(0.f, 1.f, 0.f)) * eye;
        glm::mat4 view = glm::lookAt(glm::vec3(eye),
                                     glm::vec3(0, 0, 0),
                                     glm::vec3(0, 1, 0));

        // Projection transform
        glm::mat4 proj = glm::perspective(glm::radians(45.f), (float) width / (float) height, .1f, 100.0f);
//        glm::mat4 proj = glm::ortho(1.f * width / height, -1.f * width / height, -1.f, 1.f, -1.f, 100.f);

        // Model-View-Projection transform
        glm::mat4 mvp = proj * view * model;

        program.SetUniform("u_Transform", mvp);
        
        // Clear screen
        glClearColor(Gui.colour.r, Gui.colour.g, Gui.colour.b, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw geometry
        mesh.Render();
    }
  
    void OnClose() override {
        std::cout << "Close window" << std::endl;
    }
};


#endif //PARTICLES_DEMOWINDOW_H
