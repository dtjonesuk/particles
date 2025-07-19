//
// Created by David on 15/06/2025.
//

#ifndef PARTICLES_CAMERA_H
#define PARTICLES_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    // Use LookAt to set target
    glm::vec3 GetTarget() const { return _target; }

    void SetPosition(glm::vec3 position) { _position = position; }

    glm::vec3 GetPosition() const { return glm::vec3(_position); }

    void SetViewport(glm::ivec2 viewport) {
        _viewport = viewport;
    }

    glm::ivec2 GetViewport() const { return _viewport; }

    const glm::mat4 GetViewMatrix() const { return CalcView(); };

    const glm::mat4 GetProjectionMatrix() const { return CalcProjection(); }

    float GetFov() const { return _fov; }

    void SetFov(float fov) { _fov = fov; }
    /// Transformation controls

    // Orient camera towards location in 'at'
    void LookAt(glm::vec3 at, glm::vec3 up) {
        _target = at;
        _up = up;
    }

    //  Translate the camera and the target
    void Pan(glm::vec2 input) {
        glm::vec4 view_input = glm::inverse(GetViewMatrix()) * glm::vec4(input, 0, 0);

        _position += glm::vec3(view_input);
        _target += glm::vec3(view_input);

    }

    // Orbit the camera about the target in the Y-axis
    /// TODO: Fix the trackball rotation bug!
    void Orbit(float theta, float phi) {
        _position = glm::rotate(glm::identity<glm::mat4>(), theta, _up) * glm::vec4(_position, 1.0f);
        auto right = glm::cross(_target - glm::vec3(_position), _up);
        _position = glm::rotate(glm::identity<glm::mat4>(), phi, right) * glm::vec4(_position, 1.0f);
    }

    // Zoom the camera
    void Zoom(float value) {
        _position += (_target - _position) * value;
    }

    virtual ~Camera() = default;
    
protected:
    glm::vec3 _position{};
    glm::ivec2 _viewport{};
    glm::vec3 _target{};
    glm::vec3 _up{};
    glm::vec2 _clippingPlanes {0.1f, 100.f};
    float _fov = 45.0f;

    Camera() = default;

    Camera(glm::vec3
           location,
           glm::vec3 lookAt, glm::vec3
           up,
           glm::ivec2 viewport
    ) : _position(location), _target(lookAt), _up(up), _viewport(viewport) {}
    

    [[nodiscard]] virtual glm::mat4 CalcView() const {
        return glm::lookAt(_position,
                           _target,
                           _up);
    }

    [[nodiscard]] virtual glm::mat4 CalcProjection() const = 0;

};

class PerspectiveCamera final : public Camera {
public:
    PerspectiveCamera() = default;

    PerspectiveCamera(glm::vec3 location, glm::vec3 lookAt, glm::vec3 up, glm::ivec2 viewport) : Camera(location,
                                                                                                        lookAt, up,
                                                                                                        viewport) {
    }



protected:

    [[nodiscard]] glm::mat4 CalcProjection() const override {
        return glm::perspective(glm::radians(_fov), (float) _viewport.x / (float) _viewport.y, 
                                _clippingPlanes.s, /* near */ 
                                _clippingPlanes.t /* far */);
    }
};

class OrthographicCamera final : public Camera {
public:
    OrthographicCamera() = default;

    OrthographicCamera(glm::vec3 location, glm::vec3 lookAt, glm::vec3 up, glm::ivec2 viewport) : Camera(location,
                                                                                                         lookAt, up,
                                                                                                         viewport) {
    }

protected:
    glm::mat4 CalcProjection() const override {
        return glm::ortho(1.f * _viewport.x / _viewport.y, -1.f * _viewport.x / _viewport.y, -1.f, 1.f,
                          _clippingPlanes.s /* near */,
                          _clippingPlanes.t /* far */);
    }
};

#endif //PARTICLES_CAMERA_H
