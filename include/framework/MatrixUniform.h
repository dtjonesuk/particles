//
// Created by David on 28/06/2025.
//

#ifndef PARTICLES_MATRIXUNIFORM_H
#define PARTICLES_MATRIXUNIFORM_H
#include <glm/glm.hpp>

struct alignas(16) MatrixUniform {
    glm::mat4 model;    
    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 normal;
};
#endif //PARTICLES_MATRIXUNIFORM_H
