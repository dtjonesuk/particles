//
// Created by David on 26/06/2025.
//

#ifndef PARTICLES_LIGHT_H
#define PARTICLES_LIGHT_H

#include <glm/glm.hpp>


struct alignas(16) Light {
    Light(glm::vec3 position, glm::vec3 colour, float ambientIntensity) : position(position), colour(colour), ambientIntensity(ambientIntensity) {}

    glm::vec3 position;
    float dummy0{};
    glm::vec3 colour;
    float ambientIntensity{};
    
};

#endif //PARTICLES_LIGHT_H
