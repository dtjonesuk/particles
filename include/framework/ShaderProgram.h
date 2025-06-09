//
// Created by David on 30/05/2025.
//

#ifndef PARTICLES_SHADERPROGRAM_H
#define PARTICLES_SHADERPROGRAM_H

#include <memory>

#include "GlResource.h"
#include "Shader.h"
#include "glm/gtc/type_ptr.hpp"

namespace framework {
    class ShaderProgram {
    public:
        /* Default constructor */
        ShaderProgram() = default;

        /* Move constructor */
        ShaderProgram(ShaderProgram &&other) noexcept {
            program = std::exchange(other.program, 0);
        }

        /* Move assignment */
        ShaderProgram &operator=(ShaderProgram &&other) noexcept {
            program = std::exchange(other.program, 0);
            return *this;
        }

        template<typename ...Shaders>
        explicit ShaderProgram(Shaders &... shaders) {
            GLuint handle = glCreateProgram();
            program.SetHandle(handle);

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
                char *strInfoLog = new char[infoLogLength + 1];
                glGetProgramInfoLog(handle, infoLogLength, nullptr, strInfoLog);
                msg += strInfoLog;
                delete[] strInfoLog;

                throw std::runtime_error(msg);
            }
        }

        template<typename T>
        void SetUniform(std::string_view location, const T &param) {
            GLuint loc = glGetUniformLocation(program.Handle(), location.data());
            if constexpr (std::is_same_v<T, glm::vec2>) {
                glUniform2fv(loc, 1, &param[0]);
            } else if constexpr (std::is_same_v<T, glm::vec3>) {
                glUniform2fv(loc, 1, &param[0]);
            } else if constexpr (std::is_same_v<T, glm::mat3>) {
                glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(param));
            } else if constexpr (std::is_same_v<T, glm::mat4>) {
                glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(param));
            } else if constexpr (std::is_same_v<float, T>) {
                glUniform1f(loc, param);
            } else if constexpr (std::is_same_v<double, T>) {
                glUniform1d(loc, param);
            } else if constexpr (std::is_signed_v<T>) {
                glUniform1i(loc, param);
            } else if constexpr (std::is_unsigned_v<T>) {
                glUniform1ui(loc, param);
            } else {
                static_assert(false, "Unsupported uniform type!");
            }
        }

        void UseProgram() const { program.Bind(); }

        void UnuseProgram() const { program.Unbind(); }

        [[nodiscard]] GLint GetUniformLocation(std::string_view name) const {
            GLint loc = glGetUniformLocation(program.Handle(), name.data());
            return loc;
        }

    protected:
        GlShaderProgram program;
    };

}

#endif //PARTICLES_SHADERPROGRAM_H
