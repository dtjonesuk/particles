//
// Created by David on 05/06/2025.
//

#ifndef PARTICLES_MESH_H
#define PARTICLES_MESH_H

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "GlResource.h"
#include "Texture.h"

namespace framework {
    template<typename TVertex>
    class Mesh {
    public:
        Mesh() = default;
        Mesh(Mesh&& other) = default;
        Mesh& operator=(Mesh&& other) = default;
        
        static Mesh CreateFromPoints(const std::vector<TVertex> &points) {
            Mesh mesh;
            mesh.vertices = points;    
            mesh.CreateVertexBuffer();
            mesh.CreateVertexArray();
            return mesh;
        }
        
        void AddAttrib(GLint count, GLenum type, std::ptrdiff_t offset) {
            glEnableVertexAttribArray(attribCount);
            glVertexAttribPointer(attribCount++, count, type, GL_FALSE, sizeof(TVertex), (void *)offset);
        }
        
        void Render() {
            vao.Bind();
            if (texture) {
                texture.Bind();
            }
            glDrawArrays(drawMode, 0, vertices.size());
        }
        
        const std::vector<TVertex>& Vertices() const { return vertices; }
        
        void SetTexture(Texture&& tex) {
            texture = std::move(tex);
        }
        
    protected:
        std::vector<TVertex> vertices;
        GlBuffer vbo;
        GlBuffer ibo;
        GlVertexArray vao;
        Texture texture;
        GLenum drawMode = GL_TRIANGLES;
        size_t attribCount{0};


        void CreateVertexBuffer(GLenum mode = GL_STATIC_DRAW) {
            // Create buffer to store points
            vbo.Generate();
            // Bind buffer to current context
            vbo.Bind(GL_ARRAY_BUFFER);
            // Set buffer data (send to gpu) - 3 pts * 3 co-ords each * sizeof(float).
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TVertex), &vertices[0], mode);
        }

        void CreateVertexArray() {
            GLuint handle;
            // Create vertex array object
            vao.Generate();
            // Bind vertex array to current context
            vao.Bind();
            // Bind vertex buffer to vertex array object
            vbo.Bind(GL_ARRAY_BUFFER);
        }
    };
}
#endif //PARTICLES_MESH_H
