//
// Created by David on 28/06/2025.
//

#ifndef PARTICLES_MATRIXBUFFER_H
#define PARTICLES_MATRIXBUFFER_H
#include <glm/glm.hpp>

struct alignas(16) MatrixBuffer {
    glm::mat4 model;    
    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 normal;
};
#endif //PARTICLES_MATRIXBUFFER_H
