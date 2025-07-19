//
// Created by David on 14/07/2025.
//

#ifndef PARTICLES_WIREFRAMERENDERER_H
#define PARTICLES_WIREFRAMERENDERER_H
#include "Renderer.h"

namespace framework {
    class WireframeRenderer : public Renderer {
    public:
        WireframeRenderer(std::shared_ptr<ShaderProgram> shaderProgram) : Renderer(std::move(shaderProgram)) {}

        void SetMaterial(const UnlitMaterial& material) {
            colour = material.colour;
        }
        
        void Use() override {
            Renderer::Use();
            
            program->SetUniform("u_Colour", colour);            
        }
    protected:
        glm::vec3 colour;
    };
}
#endif //PARTICLES_WIREFRAMERENDERER_H
