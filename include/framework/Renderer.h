//
// Created by David on 14/07/2025.
//

#ifndef PARTICLES_RENDERER_H
#define PARTICLES_RENDERER_H

#include <memory>
#include "GlNamedResource.h"
#include "ShaderProgram.h"

namespace framework {
    struct alignas(16) RendererWorldBuffer {
        glm::mat4 view;
        glm::mat4 projection;
        glm::vec3 cameraPosition;
    };

    struct alignas(16) RendererObjectBuffer {
        glm::mat4 model;
        glm::mat4 mvp;
        glm::mat4 normal;
    };

    class Renderer {
    public:
        Renderer(std::shared_ptr<ShaderProgram> shaderProgram) : program(std::move(shaderProgram)) {}

        void SetCamera(Camera *cam) {
            SetViewMatrix(cam->GetViewMatrix());
            SetProjectionMatrix(cam->GetProjectionMatrix());
            SetCameraPosition(cam->GetPosition());
        }

        void SetSceneEntity(SceneEntity *entity) {
            glm::mat4 model = entity->GetLocalTransform().GetMatrix();
            SetObjectMatrix(model);
        }

        void SetViewMatrix(const glm::mat4 matrix) {
            worldBuffer.view = matrix;
        }

        void SetProjectionMatrix(const glm::mat4 matrix) {
            worldBuffer.projection = matrix;
        }

        void SetCameraPosition(const glm::vec3 position) {
            worldBuffer.cameraPosition = position;
        }

        void SetObjectMatrix(const glm::mat4 matrix) {
            objectBuffer.model = matrix;
        }

        void SetObjectNormalMatrix(const glm::mat4 matrix) {
            objectBuffer.normal = matrix;
        }

        virtual void Create() {
            program->UniformBlockBinding("World", WORLD_BINDING_POINT);
            program->UniformBlockBinding("Object", OBJECT_BINDING_POINT);

            worldUbo.Create();
            GLint size = program->GetUniformBlockSize("World");
            worldUbo.Storage(size, nullptr, GL_DYNAMIC_STORAGE_BIT);
            worldUbo.BindToBindingPoint(GL_UNIFORM_BUFFER, WORLD_BINDING_POINT);

            objectUbo.Create();
            size = program->GetUniformBlockSize("Object");
            objectUbo.Storage(size, nullptr, GL_DYNAMIC_STORAGE_BIT);
            objectUbo.BindToBindingPoint(GL_UNIFORM_BUFFER, OBJECT_BINDING_POINT);
        }

        virtual void Use() {
            program->Use();

            objectBuffer.mvp = worldBuffer.projection * worldBuffer.view * objectBuffer.model;
            objectBuffer.normal = transpose(inverse(worldBuffer.view * objectBuffer.model));

            worldUbo.BindToBindingPoint(GL_UNIFORM_BUFFER, WORLD_BINDING_POINT);
            worldUbo.SubData(0, sizeof(worldBuffer), &worldBuffer);

            objectUbo.BindToBindingPoint(GL_UNIFORM_BUFFER, OBJECT_BINDING_POINT);
            objectUbo.SubData(0, sizeof(objectBuffer), &objectBuffer);
        }

        virtual void UnUse() {
            program->Unuse();
        }

        virtual ~Renderer() = default;

    protected:
        std::shared_ptr<ShaderProgram> program;

        RendererWorldBuffer worldBuffer;
        RendererObjectBuffer objectBuffer;
        GlNamedBuffer worldUbo;
        GlNamedBuffer objectUbo;

        const GLint WORLD_BINDING_POINT = 1;
        const GLint OBJECT_BINDING_POINT = 2;
    };
}
#endif //PARTICLES_RENDERER_H
