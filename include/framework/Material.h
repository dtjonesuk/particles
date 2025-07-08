//
// Created by David on 29/06/2025.
//

#ifndef PARTICLES_MATERIAL_H
#define PARTICLES_MATERIAL_H

#include <cstdint>

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Uniforms.h"


namespace framework {
    enum class MaterialType : uint8_t {
        Unlit,
        Diffuse,
        Phong,
        Blinn
    };

    class Material {
    public:
        const MaterialType type;
        
        const std::string name;

        virtual void Create(ShaderProgram &shader) = 0;

        virtual void Bind(ShaderProgram& shader) = 0;

        virtual void Unbind() = 0;
            
    protected:
        Material(MaterialType type, std::string_view name) : type(type), name(name) {}
    };

    class UnlitMaterial : public Material {
    public:
        UnlitMaterial(std::string_view name, glm::vec3 colour) : Material(MaterialType::Unlit, name), colour(colour) {}

        void Create(ShaderProgram& shader) override {
            
        }
        
        void Bind(ShaderProgram& shader) override {
            shader.SetUniform("u_Color", colour);
        }
        
        void Unbind() override {
            
        }
    protected:
        glm::vec3 colour;
    };

    class SolidMaterial : public Material {
    public:
        SolidMaterial(std::string_view name, glm::vec3 ambientColour, glm::vec3 diffuseColour, glm::vec3 specularColour,
                      float shininess) :
                Material(MaterialType::Phong, name),
                uniform(ambientColour, diffuseColour, specularColour, shininess) {}

        void Create(ShaderProgram& shader) override {
            ubo.Generate();
            ubo.Bind();
            GLuint size = shader.GetUniformBlockSize("Material");
            ubo.BufferData(size, nullptr, GL_DYNAMIC_DRAW);
            ubo.BindToBindingPoint(MATERIAL_BINDING_POINT);
            ubo.Unbind();
        }

        void Bind(ShaderProgram& shader) override {
            ubo.Bind();
            ubo.BindToBindingPoint(MATERIAL_BINDING_POINT);
            ubo.BufferSubData(0, sizeof(uniform), &uniform);
        }

        void Unbind() override {
            ubo.Unbind();
        }

    protected:
        Buffer<GL_UNIFORM_BUFFER> ubo;
        struct alignas(16) MaterialUniform {
            MaterialUniform(glm::vec3 ambientColour, glm::vec3 diffuseColour, glm::vec3 specularColour,
                            float shininess = 32.0f)
                    :
                    ambient(ambientColour),
                    diffuse(diffuseColour),
                    specular(specularColour),
                    shininess(shininess) {}

            glm::vec3 ambient;
            float dummy0;
            glm::vec3 diffuse;
            float dummy1;
            glm::vec3 specular;
            float shininess;
        } uniform;
    };
}
#endif //PARTICLES_MATERIAL_H
