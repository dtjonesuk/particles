//
// Created by David on 05/06/2025.
//

#ifndef PARTICLES_MESH_H
#define PARTICLES_MESH_H

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "framework/Resource.h"

namespace framework {
    template<typename TVertex>
    class Mesh {
    public:
        Mesh() = default;
        
        ~Mesh() {
            if (vbo)
                glDeleteBuffers(1, &vbo);
            if (ibo)
                glDeleteBuffers(1, &ibo);
            if (vao)
                glDeleteBuffers(1, &vao);
        }
        
        void CreateFromPoints(const std::vector<TVertex> &points) {
            vertices = points;    
            CreateVertexBuffer();
            CreateVertexArray();
        }
        
        void AddAttrib(GLint count, GLenum type = GL_FLOAT, std::ptrdiff_t offset = 0) {
            glEnableVertexAttribArray(attribCount);
            glVertexAttribPointer(attribCount++, count, type, GL_FALSE, sizeof(TVertex), (void *)offset);
        }
        
        void Bind() {
            glBindVertexArray(vao);
        }
        
        void Draw() {
            Bind();
            glDrawArrays(drawMode, 0, vertices.size());
        }
        
        const std::vector<TVertex>& Vertices() const { return vertices; }
        
    protected:
        std::vector<TVertex> vertices;
        GLuint vbo{0};
        GLuint ibo{0};
        GLuint vao{0};
        GLenum drawMode = GL_TRIANGLES;
        size_t attribCount{0};


        void CreateVertexBuffer(GLenum mode = GL_STATIC_DRAW) {
            // Create 1 buffer to store points
            glGenBuffers(1, &vbo);
            // Bind buffer to current context
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            // Set buffer data (send to gpu) - 3 pts * 3 co-ords each * sizeof(float).
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TVertex), &vertices[0], mode);
        }

        void CreateVertexArray() {
            // Create 1 vertex array object
            glGenVertexArrays(1, &vao);
            // Bind vertex array to current context
            glBindVertexArray(vao);
            // Bind vertex buffer to vertex array object
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
        }
    };
}
#endif //PARTICLES_MESH_H
