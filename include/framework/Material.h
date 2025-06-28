//
// Created by David on 26/06/2025.
//

#ifndef PARTICLES_MATERIAL_H
#define PARTICLES_MATERIAL_H
#include <glm/glm.hpp>

struct Material {
    Material(glm::vec3 diffuse, float specular) : diffuseColour(diffuse), specularAmount(specular) {} 
    
    glm::vec3 diffuseColour;
    float specularAmount;
};
#endif //PARTICLES_MATERIAL_H
