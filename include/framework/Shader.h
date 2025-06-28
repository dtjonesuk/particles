//
// Created by David on 30/05/2025.
//

#ifndef PARTICLES_SHADER_H
#define PARTICLES_SHADER_H

#include <string>
#include <iostream>
#include <cassert>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include "GlResource.h"

namespace framework {
    class Shader {
    public:
        Shader(GLenum shaderType, std::string_view source) : shaderType(shaderType), sourceCode(source) {
            GLuint handle = glCreateShader(shaderType);
            shader.SetHandle(handle);
            Compile();
        }

        Shader(Shader &&other) noexcept {
            std::cout << "Move constructing shader " << other.shader.Handle() << std::endl;
            shaderType = other.shaderType;
            compiled = other.compiled;
            sourceCode = std::move(other.sourceCode);
            shader = std::move(other.shader);
        }

        Shader &operator=(Shader &&other) noexcept {
            std::cout << "Moving shader " << other.shader.Handle() << std::endl;
            shaderType = other.shaderType;
            sourceCode = std::move(other.sourceCode);
            compiled = other.compiled;
            shader = std::move(other.shader);

            return *this;
        }

        void Compile() {
            assert(sourceCode.data());
            const GLchar *text = sourceCode.data();

            glShaderSource(shader.Handle(), 1, &text, nullptr);
            glCompileShader(shader.Handle());
            compiled = true;

            // Check compilation status and report error
            GLint params;
            glGetShaderiv(shader.Handle(), GL_COMPILE_STATUS, &params);
            if (params != GL_TRUE) {
                compiled = false;
                std::string msg("Shader Compilation Error: ");
                
                GLint infoLogLength;
                glGetShaderiv(shader.Handle(), GL_INFO_LOG_LENGTH, &infoLogLength);
                char *strInfoLog = new char[infoLogLength + 1];
                glGetShaderInfoLog(shader.Handle(), infoLogLength,nullptr, strInfoLog);
                msg += strInfoLog;
                delete[] strInfoLog;
                
                throw std::runtime_error(msg);
            }
        }

        bool HasHandle() const { return shader.HasHandle(); }

        GLuint Handle() const { return shader.Handle(); }

        [[nodiscard]] bool IsCompiled() const { return compiled; }

        [[nodiscard]] std::string_view SourceCode() const { return sourceCode; }

    protected:
        GlShader shader;
        GLenum shaderType;
        std::string sourceCode{};
        bool compiled{false};

    };
}
#endif //PARTICLES_SHADER_H
