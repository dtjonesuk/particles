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

    glm::vec3 Location() const { return _location; }

    glm::quat Rotation() const { return _rotation; }

    glm::vec3 Scale() const { return _scale; }

    void SetLocation(glm::vec3 location) {
        _location = location;
    }

    void SetRotation(glm::quat rotation) {
        _rotation = rotation;
    }

    void SetScale(glm::vec3 scale) {
        _scale = scale;
    }

    /// Return 4x4 matrix of transforms in order scale, rotation, translation
    glm::mat4 Matrix() const {
//        auto mat = glm::identity<glm::mat4>();
        auto mat = glm::translate(glm::identity<glm::mat4>(), _location) *
         glm::mat4_cast(_rotation) * 
         glm::scale(glm::identity<glm::mat4>(), _scale);
        

        return mat;
    }

protected:
    glm::vec3 _location;
    glm::quat _rotation;
    glm::vec3 _scale;

};

#endif //PARTICLES_TRANSFORM_H
