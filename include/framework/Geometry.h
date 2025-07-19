//
// Created by David on 26/06/2025.
//

#ifndef PARTICLES_GEOMETRY_H
#define PARTICLES_GEOMETRY_H

#include <utility>

#include "Transform.h"
#include "Material.h"

namespace framework {
    class Geometry {
    public:
        Geometry() = default;

        const std::shared_ptr<Material> GetMaterial() const { return material; }

        void SetMaterial(std::shared_ptr<Material> newMaterial) { material = std::move(newMaterial); }

        void Render() const {
            RenderGeometry();
        }

    protected:
        std::shared_ptr<Material> material;

        virtual void RenderGeometry() const = 0;
    };
}
#endif //PARTICLES_GEOMETRY_H
