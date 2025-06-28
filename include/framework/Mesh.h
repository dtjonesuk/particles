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
    template<typename TVertex>
    class Mesh : public Geometry {
    public:
        Mesh() = default;
        Mesh(Mesh&& other) = default;
        Mesh& operator=(Mesh&& other) = default;
        
        void AddAttrib(GLint count, GLenum type, std::ptrdiff_t offset) {
            glEnableVertexAttribArray(_attribCount);
            glVertexAttribPointer(_attribCount++, count, type, GL_FALSE, sizeof(TVertex), (void *)offset);
        }
        
        void SetDrawMode(GLint mode) {
            _drawMode = mode;
        }
        
        GLint GetDrawMod() const { return _drawMode;}
        
        const std::vector<TVertex>& GetVertices() const { return _vertices; }
        const std::vector<TVertex>& GetIndicies() const { return _indices; }
        
    protected:
        std::vector<TVertex> _vertices;
        std::vector<uint16_t> _indices;
        std::vector<uint16_t> _wire_indices;
        
        GlBuffer _vbo;
        GlBuffer _ibo;
        GlBuffer _wibo;
        GlVertexArray _vao;

        GLenum _drawMode = GL_POINTS;
        size_t _attribCount{0};

        void CreateVertexBuffer(const std::vector<TVertex> vertices, GLenum mode = GL_STATIC_DRAW) {
            // Create buffer to store points
            _vbo.Generate();
            // Bind buffer to current context
            _vbo.Bind(GL_ARRAY_BUFFER);

            // Set buffer data (send to gpu)
            _vertices = vertices;
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TVertex), &vertices[0], mode);
        }

        void CreateVertexArray() {
            // Create vertex array object
            _vao.Generate();
            // Bind vertex array to current context
            _vao.Bind();
            // Bind vertex buffer to vertex array object
            _vbo.Bind(GL_ARRAY_BUFFER);
        }
        
        void CreateIndexBuffer(std::vector<uint16_t> indices, GLenum mode = GL_STATIC_DRAW) {
            // Create index buffer
            _ibo.Generate();
            // Bind to current context
            _ibo.Bind(GL_ELEMENT_ARRAY_BUFFER);
            // Send data to GPU
            _indices = indices;
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint16_t), &indices[0], mode);
        }

        void CreateWireframeIndexBuffer(std::vector<uint16_t> indices, GLenum mode = GL_STATIC_DRAW) {
            // Create index buffer
            _wibo.Generate();
            // Bind to current context
            _wibo.Bind(GL_ELEMENT_ARRAY_BUFFER);
            // Send data to GPU
            _wire_indices = indices;
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint16_t), &indices[0], mode);
        }
    };
}
#endif //PARTICLES_MESH_H
