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
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
};

const std::vector<Vertex> cube_verts = {
        {
                {-1.0f, -1.0f, -1.0f,},
                {-1.0f, 0.0f,  0.0f},
                {0.583f, 0.771f, 0.014f,}
        },
        {
                {-1.0f, -1.0f, 1.0f,},
                {-1.0f, 0.0f,  0.0f},
                {0.609f, 0.115f, 0.436f}
        },
        {
                {-1.0f, 1.0f,  1.0f,},
                {-1.0f, 0.0f,  0.0f},
                {0.327f, 0.483f, 0.844f}
        },
        {
                {1.0f,  1.0f,  -1.0f,},
                {0.0f,  0.0f,  -1.0f},
                {0.822f, 0.569f, 0.201f}
        },
        {
                {-1.0f, -1.0f, -1.0f,},
                {0.0f,  0.0f,  -1.0f},
                {0.822f, 0.569f, 0.201f}
        },
        {
                {-1.0f, 1.0f,  -1.0f,},
                {0.0f,  0.0f,  -1.0f},
                {0.435f, 0.602f, 0.223f}
        },
        {
                {1.0f,  -1.0f, 1.0f,},
                {0.0f,  -1.0f, 0.0f},
                {0.310f, 0.747f, 0.185f}
        },
        {
                {-1.0f, -1.0f, -1.0f,},
                {0.0f,  -1.0f, 0.0f},
                {0.597f, 0.770f, 0.761f}
        },
        {
                {1.0f,  -1.0f, -1.0f,},
                {0.0f,  -1.0f, 0.0f},
                {0.559f, 0.436f, 0.730f}
        },
        {
                {1.0f,  1.0f,  -1.0f,},
                {0.0f,  0.0f,  -1.0f},
                {0.359f, 0.583f, 0.152f}
        },
        {
                {1.0f,  -1.0f, -1.0f,},
                {0.0f,  0.0f,  -1.0f},
                {0.483f, 0.596f, 0.789f}
        },
        {
                {-1.0f, -1.0f, -1.0f,},
                {0.0f,  0.0f,  -1.0f},
                {0.559f, 0.861f, 0.639f}
        },
        {
                {-1.0f, -1.0f, -1.0f,},
                {-1.0f, 0.0f,  0.0f},
                {0.195f, 0.548f, 0.859f}
        },
        {
                {-1.0f, 1.0f,  1.0f,},
                {-1.0f, 0.0f,  0.0f},
                {0.014f, 0.184f, 0.576f}
        },
        {
                {-1.0f, 1.0f,  -1.0f,},
                {-1.0f, 0.0f,  0.0f},
                {0.771f, 0.328f, 0.970f}
        },
        {
                {1.0f,  -1.0f, 1.0f,},
                {0.0f,  -1.0f, 0.0f},
                {0.406f, 0.615f, 0.116f}
        },
        {
                {-1.0f, -1.0f, 1.0f,},
                {0.0f,  -1.0f, 0.0f},
                {0.676f, 0.977f, 0.133f}
        },
        {
                {-1.0f, -1.0f, -1.0f,},
                {0.0f,  -1.0f, 0.0f},
                {0.971f, 0.572f, 0.833f}
        },
        {
                {-1.0f, 1.0f,  1.0f,},
                {0.0f,  0.0f,  1.0f},
                {0.140f, 0.616f, 0.489f
                }
        },
        {
                {-1.0f, -1.0f, 1.0f,},
                {0.0f,  0.0f,  1.0f},
                {0.997f, 0.513f, 0.064f}
        },
        {
                {1.0f,  -1.0f, 1.0f,},
                {0.0f,  0.0f,  1.0f},
                {0.945f, 0.719f, 0.592f}
        },
        {
                {1.0f,  1.0f,  1.0f,},
                {1.0f,  0.0f,  0.0f},
                {0.543f, 0.021f, 0.978f}
        },
        {
                {1.0f,  -1.0f, -1.0f,},
                {1.0f,  0.0f,  0.0f},
                {0.279f, 0.317f, 0.505f}
        },
        {
                {1.0f,  1.0f,  -1.0f,},
                {1.0f,  0.0f,  0.0f},
                {0.167f, 0.620f, 0.077f,}
        },
        {
                {1.0f,  -1.0f, -1.0f,},
                {1.0f,  0.0f,  0.0f},
                {0.347f, 0.857f, 0.137f}
        },
        {
                {1.0f,  1.0f,  1.0f,},
                {1.0f,  0.0f,  0.0f},
                {0.055f, 0.953f, 0.042f}
        },
        {
                {1.0f,  -1.0f, 1.0f,},
                {1.0f,  0.0f,  0.0f},
                {0.714f, 0.505f, 0.345f}
        },
        {
                {1.0f,  1.0f,  1.0f,},
                {0.0,   1.0f,  0.0f},
                {0.783f, 0.290f, 0.734f}
        },
        {
                {1.0f,  1.0f,  -1.0f,},
                {0.0,   1.0f,  0.0f},
                {0.722f, 0.645f, 0.174f}
        },
        {
                {-1.0f, 1.0f,  -1.0f,},
                {0.0,   1.0f,  0.0f},
                {0.302f, 0.455f, 0.848f}
        },
        {
                {1.0f,  1.0f,  1.0f,},
                {0.0f,  1.0f,  0.0f},
                {0.225f, 0.587f, 0.040f}
        },
        {
                {-1.0f, 1.0f,  -1.0f,},
                {0.0f,  1.0f,  0.0f},
                {0.517f, 0.713f, 0.338f}
        },
        {
                {-1.0f, 1.0f,  1.0f,},
                {0.0f,  1.0f,  0.0f},
                {0.053f, 0.959f, 0.120f}
        },
        {
                {1.0f,  1.0f,  1.0f,},
                {0.0f,  0.0f,  1.0f},
                {0.393f, 0.621f, 0.362f}
        },
        {
                {-1.0f, 1.0f,  1.0f,},
                {0.0f,  0.0f,  1.0f},
                {0.673f, 0.211f, 0.457f}
        },
        {
                {1.0f,  -1.0f, 1.0f},
                {0.0f,  0.0f,  1.0f},
                {0.820f, 0.883f, 0.371f}
        },
};

class DemoWindow : public Window {
public:
    DemoWindow() : Window(1920, 1080, "LearnOpenGL") {}

protected:
    Mesh<Vertex> mesh;
    Texture texture;
    ShaderProgram program;
    double previous = 0;

    struct {
        bool showDemoWindow = false;
        float scale = 1.0f;
        float rotation = 0.f;
        glm::vec2 translation{0.f};
        glm::vec3 colour{0.0f, 0.5f, 1.0f};
        float ambient_light_intensity = 0.2f;
        glm::vec3 light_position{5.0, 10.0, 5.0};
        glm::vec3 light_colour{1.0f, 1.0f, 1.0f};
        
    } Gui;

    void OnInit() override {
        // Limit FPS
        glfwSwapInterval(1);

        texture.CreateFromFile("assets/textures/opengl-logo.png");

        // Geometry
        mesh = Mesh<Vertex>::CreateFromPoints(cube_verts);
        mesh.AddAttrib(3, GL_FLOAT, 0);
        mesh.AddAttrib(3, GL_FLOAT, offsetof(Vertex, normal));
        mesh.AddAttrib(3, GL_FLOAT, offsetof(Vertex, color));
        mesh.SetTexture(std::move(texture));

        // Compile shaders and create shader program
        Shader vert(GL_VERTEX_SHADER, loadShaderSource("assets/shaders/3d.vert.glsl"));
        Shader frag(GL_FRAGMENT_SHADER, loadShaderSource("assets/shaders/3d.frag.glsl"));
        
        program = ShaderProgram(vert, frag);
        program.UseProgram();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
    }

    void OnGui() override {
        if (Gui.showDemoWindow)
            ImGui::ShowDemoWindow(&Gui.showDemoWindow);
        ImGui::Begin("Particle System", nullptr/*,  ImGuiWindowFlags_MenuBar*/);
        ImGui::Text("dear imgui says hello! (%s) (%d)", IMGUI_VERSION, IMGUI_VERSION_NUM);
        ImGui::Spacing();
        ImGui::SeparatorText("Object");
        ImGui::SliderFloat2("Translation", &Gui.translation[0], -2.0f, 2.0f);
        ImGui::SliderFloat("Scale", &Gui.scale, 0.0f, 5.0f);
        ImGui::SliderFloat("Rotation", &Gui.rotation, 0.0f, 360.0f);
        ImGui::ColorPicker3("Colour##Object", &Gui.colour[0]);

        ImGui::Spacing();
        ImGui::SeparatorText("Light");
        ImGui::SliderFloat3("Position", &Gui.light_position[0], -10.0f, 10.0f);
        ImGui::ColorPicker3("Colour##Light", &Gui.light_colour[0]);
        ImGui::SliderFloat("Ambient", &Gui.ambient_light_intensity, 0.0f, 1.0f);
        ImGui::End();
    }

    void OnUpdate(const TimeInfo &time) override {
        // Calculate fractional part of time
        float percent = (cos((float) time.currentTime / 5 * 2 * PI) + 1.f) / 2.f;

        // Set shaders and uniform values
        program.UseProgram();
        program.SetUniform("u_AmbientIntensity", Gui.ambient_light_intensity);
        program.SetUniform("u_LightColor", Gui.light_colour);
        program.SetUniform("u_LightPos", Gui.light_position);
        program.SetUniform("u_ObjectColor", Gui.colour);

        // Model transform
        auto model = glm::identity<glm::mat4>();
//        model = glm::rotate(model, (float) time.currentTime * PI * 2, glm::vec3{0.f, 0.f, 1.f});
//        model = glm::scale(model, glm::vec3{2, 2, 2});
        model = glm::translate(model, glm::vec3(Gui.translation.x, Gui.translation.y, 0.f));
        model = glm::rotate(model, glm::radians(Gui.rotation * (float)time.currentTime), glm::normalize(glm::vec3{1.f, 1.f, 1.f}));
        model = glm::scale(model, glm::vec3{Gui.scale, 1.0f, Gui.scale});

        // View transform
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        auto eye = glm::vec4(0.f, 2.f, 5.f, 1.f);
        eye = glm::rotate(glm::identity<glm::mat4>(), glm::radians(360.f * 1.0f), glm::vec3(0.f, 1.f, 0.f)) * eye;
        glm::mat4 view = glm::lookAt(glm::vec3(eye),
                                     glm::vec3(0, 0, 0),
                                     glm::vec3(0, 1, 0));

        // Projection transform
        glm::mat4 proj = glm::perspective(glm::radians(45.f), (float) width / (float) height, .1f, 100.0f);
//        glm::mat4 proj = glm::ortho(1.f * width / height, -1.f * width / height, -1.f, 1.f, -1.f, 100.f);

        // Model-View-Projection matrix
        glm::mat4 mvp = proj * view * model;
        program.SetUniform("u_MVP", mvp);
        
        // Model matrix
        program.SetUniform("u_Model", model);
        
        // Normal matrix
        glm::mat4 normal = glm::transpose(glm::inverse(model));
        program.SetUniform("u_Normal", normal);

        // Clear screen
        
        glClearColor(.1f, .1f, .1f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw geometry
        mesh.Render();
    }

    void OnClose() override {
        std::cout << "Close window" << std::endl;
    }
};


#endif //PARTICLES_DEMOWINDOW_H
