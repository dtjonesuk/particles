//
// Created by David on 12/07/2025.
//

#ifndef PARTICLES_SOLIDMATERIALRENDERER_H
#define PARTICLES_SOLIDMATERIALRENDERER_H


#include "ShaderProgram.h"
#include "Renderer.h"

#include <utility>
#include "Light.h"
#include "Material.h"


namespace framework {
    /// Encorporates the material properties required to represent the material
    /// and the shader program required to render such.
    namespace {
        struct alignas(16) SolidMaterialLightBuffer {
            glm::vec3 position;
            float _dummy0;
            glm::vec3 ambient;
            float _dummy1;
            glm::vec3 diffuse;
            float _dummy2;
            glm::vec3 specular;
            float _dummy3;
        };

        struct alignas(16) SolidMaterialBuffer {
            glm::vec3 ambient;
            float _dummy1;
            glm::vec3 diffuse;
            float _dummy2;
            glm::vec3 specular;
            float shininess;
        };
    }

    class SolidMaterialRenderer : public Renderer {
    public:
        SolidMaterialRenderer(std::shared_ptr<ShaderProgram> shaderProgram) : Renderer(std::move(shaderProgram)) {} 

        void SetMaterial(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular,
                         float shininess = 32.0f) {
            materialBuffer.ambient = ambient;
            materialBuffer.diffuse = diffuse;
            materialBuffer.specular = specular;
            materialBuffer.shininess = shininess;
        }
        
        void SetMaterial(const SolidMaterial &material) {
            materialBuffer.ambient = material.ambient;
            materialBuffer.diffuse = material.diffuse;
            materialBuffer.specular = material.specular;
            materialBuffer.shininess = material.shininess;
        }

        void SetLight(const Light &light) {
            lightBuffer.position = light.position;
            lightBuffer.ambient = light.ambient;
            lightBuffer.diffuse = light.diffuse;
            lightBuffer.specular = light.specular;
        }

        void Create() override {
            Renderer::Create();
            
            program->UniformBlockBinding("Material", MATERIAL_BINDING_POINT);
            program->UniformBlockBinding("Light", LIGHT_BINDING_POINT);
            
            materialUbo.Create();
            GLuint size = program->GetUniformBlockSize("Material");
            materialUbo.Storage(size, nullptr, GL_DYNAMIC_STORAGE_BIT);
            materialUbo.BindToBindingPoint(GL_UNIFORM_BUFFER, MATERIAL_BINDING_POINT);

            lightUbo.Create();
            size = program->GetUniformBlockSize("Light");
            lightUbo.Storage(size, nullptr, GL_DYNAMIC_STORAGE_BIT);
            lightUbo.BindToBindingPoint(GL_UNIFORM_BUFFER, LIGHT_BINDING_POINT);
        }
        
        void Use() override {
            Renderer::Use();
            
            lightUbo.BindToBindingPoint(GL_UNIFORM_BUFFER, LIGHT_BINDING_POINT);
            lightUbo.SubData(0, sizeof(lightBuffer), &lightBuffer);

            materialUbo.BindToBindingPoint(GL_UNIFORM_BUFFER, MATERIAL_BINDING_POINT);
            materialUbo.SubData(0, sizeof(materialBuffer), &materialBuffer);
        }
        
        
    protected:
        SolidMaterialBuffer materialBuffer;
        SolidMaterialLightBuffer lightBuffer;
        GlNamedBuffer materialUbo;
        GlNamedBuffer lightUbo;

    private:
        const GLint MATERIAL_BINDING_POINT = 3;
        const GLint LIGHT_BINDING_POINT = 4;

    };
}
#endif //PARTICLES_SOLIDMATERIALRENDERER_H
