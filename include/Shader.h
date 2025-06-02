//
// Created by David on 30/05/2025.
//

#ifndef PARTICLES_SHADER_H
#define PARTICLES_SHADER_H

#include <string>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Shader {
public:
    Shader(GLenum shaderType) : shaderType(CreateShader(shaderType)) {}

    Shader(GLenum shaderType, std::string_view source) : shaderType(shaderType), handle(0), sourceCode(source) {
        handle = CreateShader(shaderType);
        CompileShader(source);
    }
    
    Shader(const Shader& shader) = delete;
    Shader& operator=(const Shader& shader) = delete;

    virtual ~Shader() {
        if (handle)
            glDeleteShader(handle);
    }
    
    
    friend class ShaderProgram;
protected:
    GLenum shaderType;
    GLuint handle{0};
    std::string sourceCode{};
    bool compiled = false;
    
    GLuint CreateShader(GLenum type) {
        return glCreateShader(type);
    }

    void CompileShader(std::string_view source) {
        const GLchar *text = source.data();
        GLint length;
        GLint params;
        std::cout << source <<  std::endl;
        glShaderSource(handle, 1, &text, nullptr);
        glCompileShader(handle);
        compiled = true;
        glGetShaderiv(handle, GL_COMPILE_STATUS, &params);
        if (params != GL_TRUE) {
            int max_length    = 2048, actual_length = 0;
            char slog[2048];
            glGetShaderInfoLog( handle, max_length, &actual_length, slog );
            std::cerr << "ERROR: Shader index "<<  handle << " did not compile.\n"  << slog << std::endl;
            compiled = false;
        }
    }
};


#endif //PARTICLES_SHADER_H
