//
// Created by David on 26/06/2025.
//

#ifndef PARTICLES_TRANSFORM_H
#define PARTICLES_TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>


class Transform {
public:
    Transform() = default;

    Transform(glm::vec3 location, glm::quat rotation = glm::identity<glm::quat>(), glm::vec3 scale = glm::vec3{1}) :
            _location(location), _rotation(rotation), _scale(scale) {}

    glm::vec3 GetLocation() const { return _location; }

    glm::quat GetRotation() const { return _rotation; }

    glm::vec3 GetScale() const { return _scale; }

    void SetLocation(glm::vec3 location) {
        _location = location;
    }

    void SetRotation(glm::quat rotation) {
        _rotation = rotation;
    }
    
    void SetRotationEuler(glm::vec3 rotationEuler) {
        _rotation = glm::quat(rotationEuler);
    }

    void SetScale(glm::vec3 scale) {
        _scale = scale;
    }

    Transform& Translate(glm::vec3 relative) {
        _location += relative;
        return  *this;
    }
    
    Transform& Rotate(glm::quat relative) {
        _rotation *= relative;
        return *this;
    }
    
    Transform& Scale(glm::vec3 relative) {
        _scale *= relative;
        return *this;
    }
    
    /// Return 4x4 matrix of transforms in order scale, rotation, translation
    /// this could be cached for performance?
    glm::mat4 GetMatrix() const {
        auto mat = glm::translate(glm::identity<glm::mat4>(), _location) *
         glm::mat4_cast(_rotation) * 
         glm::scale(glm::identity<glm::mat4>(), _scale);
        
        return mat;
    }

protected:
    glm::vec3 _location{0};
    glm::quat _rotation{glm::identity<glm::quat>()};
    glm::vec3 _scale{1};

};

#endif //PARTICLES_TRANSFORM_H
