//
// Created by David on 26/06/2025.
//

#ifndef PARTICLES_GEOMETRY_H
#define PARTICLES_GEOMETRY_H

#include "Transform.h"

class Geometry {
public:
    Geometry() = default;
    
    Transform& GetTransform() { return _transform; }
    const Transform& GetTransform() const { return _transform; }
    void SetTransform(const Transform &transform) {_transform = transform; }
    
    void Render() {
        RenderGeometry();
    }
    
protected:
    Transform _transform;
    virtual void RenderGeometry() = 0;
};
#endif //PARTICLES_GEOMETRY_H
