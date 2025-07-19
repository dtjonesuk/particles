//
// Created by David on 29/06/2025.
//

#ifndef PARTICLES_MATERIAL_H
#define PARTICLES_MATERIAL_H

#include <cstdint>

#include <glm/glm.hpp>
#include "ShaderProgram.h"

namespace framework {
    enum class MaterialType : uint8_t {
        Unlit,
        Wireframe,
        Solid
    };

    class Material {
    public:
        const MaterialType type;
        const std::string name;

    protected:
        Material(MaterialType type, std::string_view name) : type(type), name(name) {}
    };

    class UnlitMaterial : public Material {
    public:
        UnlitMaterial(std::string_view name, glm::vec3 colour) : Material(MaterialType::Wireframe, name), colour(colour) {}
        
        glm::vec3 colour;
    };

    class SolidMaterial : public Material { 
    public:
        SolidMaterial(std::string_view name, glm::vec3 ambientColour, glm::vec3 diffuseColour, glm::vec3 specularColour,
                      float shininess) :
                Material(MaterialType::Solid, name), 
                ambient(ambientColour), 
                diffuse(diffuseColour), 
                specular(specularColour),
                shininess(shininess) {}
                
                std::string name;
                glm::vec3 ambient;
                glm::vec3 diffuse;
                glm::vec3 specular;
                float shininess;
    };

}
#endif //PARTICLES_MATERIAL_H
