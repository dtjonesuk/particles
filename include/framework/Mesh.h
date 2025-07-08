//
// Created by David on 05/06/2025.
//

#ifndef PARTICLES_MESH_H
#define PARTICLES_MESH_H

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "GlResource.h"
#include "Geometry.h"
#include "Texture.h"

namespace framework {

    struct VertexRGB {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 color;
    };

    struct VertexUV {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
    };

    struct VertexRGBUV {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 color;
        glm::vec2 uv;
    };


    template<typename TVertex>
    class Mesh : public Geometry {
    public:
        Mesh(GLint drawMode = GL_TRIANGLES) : _drawMode(drawMode) {}

        Mesh(Mesh &other) = delete;

        Mesh &operator=(Mesh &other) = delete;

        Mesh(Mesh &&other) = default;

        Mesh &operator=(Mesh &&other) = default;

        void AddAttrib(GLint count, GLenum type, std::ptrdiff_t offset) {
            glEnableVertexAttribArray(_attribCount);
            glVertexAttribPointer(_attribCount++, count, type, GL_FALSE, sizeof(TVertex), (void *) offset);
        }

        void SetDrawMode(GLint mode) {
            _drawMode = mode;
        }

        GLint GetDrawMode() const { return _drawMode; }

        const std::vector<TVertex> &GetVertices() const { return _vertices; }

        const std::vector<TVertex> &GetIndicies() const { return _indices; }

    protected:
        std::vector<TVertex> _vertices;
        std::vector<uint16_t> _indices;

        Buffer<GL_ARRAY_BUFFER> _vbo;
        Buffer<GL_ELEMENT_ARRAY_BUFFER> _ibo;
        GlVertexArray _vao;

        GLenum _drawMode;
        size_t _attribCount{0};

        void CreateVertexBuffer(const std::vector<TVertex> vertices, GLenum mode = GL_STATIC_DRAW) {
            _vao.Bind();
            // Create buffer to store points
            _vbo.Generate();
            
            // Bind buffer to current context
            _vbo.Bind();

            // Set buffer data (send to gpu)
            _vertices = vertices;
            _vbo.BufferData(vertices.size() * sizeof(TVertex), &vertices[0], mode);
//            _vao.Unbind();
        }

        void CreateVertexArray() {
            // Create vertex array object
            _vao.Generate();
            // Bind vertex array to current context
            _vao.Bind();
            // Bind vertex buffer to vertex array object
//            _vbo.Bind();
//            if (_ibo.Handle())
//                _ibo.Bind();
//            _vao.Unbind();
        }

        void CreateIndexBuffer(std::vector<uint16_t> indices, GLenum mode = GL_STATIC_DRAW) {
            _vao.Bind();
            // Create index buffer
            _ibo.Generate();
            // Bind to current context
            _ibo.Bind();
            // Send data to GPU
            _indices = indices;
            _ibo.BufferData(indices.size() * sizeof(uint16_t), &indices[0], mode);
            _ibo.Unbind();
            
//            _vao.Unbind();
        }
    };
}
#endif //PARTICLES_MESH_H
