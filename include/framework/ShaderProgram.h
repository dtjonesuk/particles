//
// Created by David on 30/05/2025.
//

#ifndef PARTICLES_SHADERPROGRAM_H
#define PARTICLES_SHADERPROGRAM_H

#include <memory>

#include "GlNamedResource.h"
#include "Shader.h"
#include "glm/gtc/type_ptr.hpp"

namespace framework {
    class ShaderProgram {
    public:
        /* Default constructor */
        ShaderProgram() = default;

        /* Move constructor */
        ShaderProgram(ShaderProgram &&other) noexcept {
            program = std::exchange(other.program, {});
        }

        /* Move assignment */
        ShaderProgram &operator=(ShaderProgram &&other) noexcept {
            program = std::exchange(other.program, {});
            return *this;
        }

        ShaderProgram(std::string_view vsFilename, std::string_view fsFilename) {
            GlNamedShader vs = Shader::FromFile(GL_VERTEX_SHADER, vsFilename);
            GlNamedShader fs = Shader::FromFile(GL_FRAGMENT_SHADER, fsFilename);

            program.Create();

            // Attach shaders
            program.AttachShader(vs);
            program.AttachShader(fs);

            // Link program
            bool status = program.Link();

            // Print the linking log:
            if (!status) {
                std::string msg("Program linking failure: ");

                msg += program.GetLinkInfoLog();

                std::cerr << msg << std::endl;

                throw std::runtime_error(msg);
            }
        }

        template<typename T>
        void SetUniform(std::string_view location, const T &param) {
            program.SetUniform(location, param);
        }

        void Use() const { program.Use(); }

        void Unuse() const { program.UnUse(); }

        [[nodiscard]] GLint GetUniformLocation(std::string_view name) const {
            GLint loc = glGetUniformLocation(program.Handle(), name.data());
            return loc;
        }

        [[nodiscard]] GLuint GetUniformBlockIndex(std::string_view name) const {
            GLuint loc = glGetUniformBlockIndex(program.Handle(), name.data());
            return loc;
        }

        [[nodiscard]] GLint GetUniformBlockSize(std::string_view name) const {
            GLint size;
            glGetActiveUniformBlockiv(program.Handle(), GetUniformBlockIndex(name),
                                      GL_UNIFORM_BLOCK_DATA_SIZE, &size);
            return size;
        }

        void UniformBlockBinding(std::string_view name, GLuint bindingPoint) {
            GLuint index = GetUniformBlockIndex(name);
            glUniformBlockBinding(program.Handle(), index, bindingPoint);
        }

        const GlNamedProgram& GlObject() const { return program; }

    protected:
        GlNamedProgram program;
    };

}

#endif //PARTICLES_SHADERPROGRAM_H
