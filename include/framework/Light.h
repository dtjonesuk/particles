//
// Created by David on 26/06/2025.
//

#ifndef PARTICLES_LIGHT_H
#define PARTICLES_LIGHT_H

#include <glm/glm.hpp>

namespace framework {
    enum class LightType : uint8_t {
        DirectionalLight,
        PointLight    
    };
  
    class Light {
    public:
        Light(glm::vec3 position, glm::vec3 ambient = glm::vec3(0.2f), glm::vec3 diffuse = glm::vec3(0.8f),
              glm::vec3 specular = glm::vec3(0.5f)) : position(position), ambient(ambient), diffuse(diffuse), specular(specular) {

        }
        
        glm::vec3 position;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;

    };

}
#endif //PARTICLES_LIGHT_H
