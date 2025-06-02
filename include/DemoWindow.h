//
// Created by David on 02/06/2025.
//

#ifndef PARTICLES_DEMOWINDOW_H
#define PARTICLES_DEMOWINDOW_H

#include <numbers>

#include "glutils.h"
#include "Window.h"
#include "ShaderProgram.h"

#define PI std::numbers::pi_v<float>

const glm::vec3 points[] = {
        {0.f,   0.5f,  0.f},
        {1.0f,  0.f,   0.f},
        {0.5f,  -0.5f, 0.f},
        {0.f,   1.0f,  0.f},
        {-0.5f, -0.5f, 0.f},
        {0.f,   0.f,   1.0f}
};

class DemoWindow : public Window {
public:
    DemoWindow() : Window(1920, 1080, "LearnOpenGL") {}

protected:
    ShaderProgram program;
    double previous = 0;
    GLuint vbo = 0;
    GLuint vao = 0;
//    GLint u_Color = 0;
    GLint u_Percent = 0;

    void OnInit() override {
        // Create 1 buffer to store points
        glGenBuffers(1, &vbo);
        // Bind buffer to current context
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        // Set buffer data (send to gpu) - 3 pts * 3 co-ords each * sizeof(float).
        glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), &points[0], GL_STATIC_DRAW);

        // Create 1 vertex array object
        glGenVertexArrays(1, &vao);
        // Bind vertex array to current context
        glBindVertexArray(vao);
        // Enable attrib #0
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        // Bind vertex buffer to vertex array object
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        // Attrib #0: 3 floats, non-normalized, stride 0
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * 3 * sizeof(float), nullptr);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * 3 * sizeof(float), (void *) (3 * sizeof(float)));

        // Compile shaders and create shader program
        Shader vert(GL_VERTEX_SHADER, loadShaderSource("shaders/vert.glsl"));
        Shader frag(GL_FRAGMENT_SHADER, loadShaderSource("shaders/frag.glsl"));
        program = ShaderProgram(vert, frag);
        program.UseProgram();
        u_Percent = program.GetUniformLocation("u_Percent");
    }

    void OnUpdate(TimeInfo time) override {
        // Calculate fractional part of time
        float percent = (cos((float)time.currentTime / 2 * 2 * PI) + 1.f) / 2.f;

        // Set shaders and uniform values
        program.UseProgram();
        glUniform1f(u_Percent, percent);

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        glClearColor(0.1f, 0.1f, 0.1f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
};


#endif //PARTICLES_DEMOWINDOW_H
