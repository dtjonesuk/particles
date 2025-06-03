//
// Created by David on 30/05/2025.
//

#ifndef PARTICLES_SHADERPROGRAM_H
#define PARTICLES_SHADERPROGRAM_H

#include <memory>

#include "Resource.h"
#include "Shader.h"

namespace framework {
    using GLResource = Resource<GLuint>;
    
    class ShaderProgram : public GLResource {
    public:
        /* Default constructor */
        ShaderProgram() = default;

        /* Move constructor */
        ShaderProgram(ShaderProgram &&other) noexcept : GLResource(std::move(other)) {
        }

        /* Move assignment */
        ShaderProgram &operator=(ShaderProgram &&other) noexcept {
            GLResource::operator=(std::move(other));
            return *this;
        }
        
        template<typename ...Shaders>
        explicit ShaderProgram(Shaders&... shaders) {
            GLuint handle = glCreateProgram();
            SetHandle(handle);

            (glAttachShader(handle, shaders.Handle()), ...);

            glLinkProgram(handle);

            // Check for linking errors:
            GLint params;
            glGetProgramiv(handle, GL_LINK_STATUS, &params);

            // Print the linking log:
            if (GL_TRUE != params) {
                std::string msg("Program linking failure: ");

                GLint infoLogLength;
                glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &infoLogLength);
                char* strInfoLog = new char[infoLogLength + 1];
                glGetProgramInfoLog(handle, infoLogLength, nullptr, strInfoLog);
                msg += strInfoLog;
                delete[] strInfoLog;

//                std::cerr << msg << std::endl;

                throw std::runtime_error(msg);
            }
        }
        
        ~ShaderProgram() override {
            if (HasHandle()) {
                std::cout << "Delete shader program " << Handle() << std::endl;
                glDeleteProgram(Handle());
            }
        }

        void UseProgram() const { glUseProgram(Handle()); }

        [[nodiscard]] GLint GetUniformLocation(std::string_view name) const {
            GLint loc = glGetUniformLocation(Handle(), name.data());
            return loc;
        }

        void FreeHandle() override {
            glDeleteShader(Handle());
        }
    };

}

#endif //PARTICLES_SHADERPROGRAM_H
