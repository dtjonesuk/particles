//
// Created by David on 26/06/2025.
//

#ifndef PARTICLES_LIGHT_H
#define PARTICLES_LIGHT_H

#include <glm/glm.hpp>

#include "Buffer.h"
#include "Uniforms.h"

namespace framework {
    enum class LightType : uint8_t {
        DirectionalLight,
        PointLight    
    };
    
    class Light {
    public:
        Light(glm::vec3 position, glm::vec3 ambient = glm::vec3(0.2f), glm::vec3 diffuse = glm::vec3(0.8f),
              glm::vec3 specular = glm::vec3(0.5f)) : uniform(position, ambient, diffuse, specular) {

        }

        void Create(ShaderProgram& shader) {
            ubo.Generate();
            ubo.Bind();
            GLuint size = shader.GetUniformBlockSize("Light");
            ubo.BufferData(size, nullptr, GL_DYNAMIC_DRAW);
            ubo.BindToBindingPoint(LIGHT_BINDING_POINT);
            ubo.Unbind();
        }

        void Bind() {
            ubo.Bind();
            ubo.BindToBindingPoint(LIGHT_BINDING_POINT);
            ubo.BufferSubData(0, sizeof(LightUniformBuffer), &uniform);
        }

        void Unbind() { ubo.Unbind(); }

    protected:
        Buffer<GL_UNIFORM_BUFFER> ubo;

        struct alignas(16) LightUniformBuffer {
            LightUniformBuffer(glm::vec3 position, glm::vec3 ambient = glm::vec3(0.2f),
                               glm::vec3 diffuse = glm::vec3(0.8f),
                               glm::vec3 specular = glm::vec3(0.5f)) :
                    position(position),
                    ambient(ambient),
                    diffuse(diffuse),
                    specular(specular) {}

            glm::vec3 position;
            float dummy0{};

            glm::vec3 ambient;
            float dummy1{};

            glm::vec3 diffuse;
            float dummy2{};

            glm::vec3 specular;
            float dummy3{};
        } uniform;
    };
}
#endif //PARTICLES_LIGHT_H
