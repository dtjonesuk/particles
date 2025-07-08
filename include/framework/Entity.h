//
// Created by David on 04/07/2025.
//

#ifndef PARTICLES_ENTITY_H
#define PARTICLES_ENTITY_H

#include <vector>
#include <memory>

#include "Transform.h"
#include "Material.h"

namespace framework {
    class Scene;

    class SceneEntity : public std::enable_shared_from_this<SceneEntity> {
    public:
        SceneEntity(std::shared_ptr<Scene> scene, std::shared_ptr<Geometry> geometry) : scene(scene), geometry(geometry) {}
        SceneEntity(std::shared_ptr<Scene> scene, Transform transform, std::shared_ptr<Geometry> geometry) : scene(scene), transform(transform), geometry(geometry) {}

        std::shared_ptr<SceneEntity> GetParent() const { return parent; }

        void SetParent(std::shared_ptr<SceneEntity> newParent) {
            // add this to new parent's children
            if (newParent)
                newParent->children.push_back(shared_from_this());
            
            // remove from old parents children
            if (parent)
                parent->children.erase(
                        std::find(parent->children.begin(), parent->children.end(), shared_from_this())
                );
            
            // set parent
            parent = newParent;
        }

        const std::vector<std::shared_ptr<SceneEntity>> &GetChildren() const { return children; }

        std::vector<std::shared_ptr<SceneEntity>> &GetChildren() { return children; }

        const Transform &GetLocalTransform() const { return transform; }

        Transform &GetLocalTransform() { return transform; }

        void SetLocalTransform(Transform newTransform) { transform = newTransform; }

        const std::shared_ptr<Geometry> GetGeometry() const { return geometry; }

    protected:
        std::shared_ptr<Scene> scene;
        Transform transform;
        std::shared_ptr<Geometry> geometry;

        std::shared_ptr<SceneEntity> parent{nullptr};
        std::vector<std::shared_ptr<SceneEntity>> children;

    };

}
#endif //PARTICLES_ENTITY_H
