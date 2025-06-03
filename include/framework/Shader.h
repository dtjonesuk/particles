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

#include "Resource.h"

namespace framework {
    using GLResource = Resource<GLuint>;
    class Shader : public GLResource {
    public:
        Shader(GLenum shaderType, std::string_view source) : shaderType(shaderType), sourceCode(source)  {
            GLuint handle = glCreateShader(shaderType);
            InitializeHandle(handle);
            Compile();
        }

        Shader(Shader &&other) noexcept : GLResource(std::move(other))  {
            std::cout << "Move constructing shader " << other._handle << std::endl;
            shaderType = other.shaderType;
            compiled = other.compiled;
            sourceCode = std::move(other.sourceCode);
        }

        Shader &operator=(Shader &&other) noexcept {
            std::cout << "Moving shader " << other._handle << std::endl;
            shaderType = other.shaderType;
            sourceCode = std::move(other.sourceCode);
            compiled = other.compiled;
            
            GLResource::operator=(std::move(other));

            return *this;
        }

        ~Shader() noexcept override {
            if (HasHandle()) {
                std::cout << "Delete shader " << Handle() << std::endl;
                glDeleteShader(Handle());
            }
        }

        void FreeHandle() override {
            if (HasHandle())
                glDeleteShader(Handle());
        }

        void Compile() {
            assert(sourceCode.data());
            const GLchar *text = sourceCode.data();

            glShaderSource(Handle(), 1, &text, nullptr);
            glCompileShader(Handle());
            compiled = true;

            // Check compilation status and report error
            GLint params;
            glGetShaderiv(Handle(), GL_COMPILE_STATUS, &params);
            if (params != GL_TRUE) {
                int max_length = 2048, actual_length = 0;
                char slog[2048];
                glGetShaderInfoLog(Handle(), max_length, &actual_length, slog);
                std::cerr << "ERROR: Shader index " << Handle() << " did not compile.\n" << slog << std::endl;
                compiled = false;
            }
        }
        
        [[nodiscard]] bool IsCompiled() const { return compiled; }
        [[nodiscard]] std::string_view SourceCode() const { return sourceCode; }
    protected:
        GLenum shaderType;
        std::string sourceCode{};
        bool compiled{false};
        
    };
}
#endif //PARTICLES_SHADER_H
