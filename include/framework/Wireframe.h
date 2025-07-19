//
// Created by David on 18/06/2025.
//

#ifndef PARTICLES_WIREFRAME_H
#define PARTICLES_WIREFRAME_H

#include "StaticMesh.h"

namespace framework {
    struct WireframeVertex {
        glm::vec3 position;
        glm::vec3 colour;
    };

    class WireframeMesh : public Mesh<WireframeVertex> {
    public:
        WireframeMesh() {
            SetDrawMode(GL_LINES);
        }

        static WireframeMesh
        CreateFromIndices(const std::vector<WireframeVertex> &points, const std::vector<uint16_t> &indices) {
            WireframeMesh mesh;
            mesh.CreateVertexArray();
            mesh.CreateVertexBuffer(points);
            mesh.CreateIndexBuffer(indices);
            mesh.AddAttrib(3, GL_FLOAT, offsetof(WireframeVertex, position));
            mesh.AddAttrib(3, GL_FLOAT, offsetof(WireframeVertex, colour));
            mesh._vao.Unbind();
            return mesh;
        }

        static WireframeMesh CreateFromPoints(const std::vector<WireframeVertex> &points) {
            WireframeMesh mesh;
            mesh.CreateVertexArray();
            mesh.CreateVertexBuffer(points);
            mesh.AddAttrib(3, GL_FLOAT, offsetof(WireframeVertex, position));
            mesh.AddAttrib(3, GL_FLOAT, offsetof(WireframeVertex, colour));
            mesh._vao.Unbind();
            return mesh;
        }

        static WireframeMesh CreateCube(glm::vec3 colour = {1.0f, 1.0f, 1.0f}) {
            const glm::vec3 positions[6][4] =
                    {
                            {{-1.0f, -1.0f, -1.0f}, {-1.0f, -1.0f, 1.0f},  {1.0f,  -1.0f, 1.0f},  {1.0f,  -1.0f, -1.0f}},
                            {{-1.0f, 1.0f,  1.0f},  {-1.0f, 1.0f,  -1.0f}, {1.0f,  1.0f,  -1.0f}, {1.0f,  1.0f,  1.0f}},
                            {{-1.0f, 1.0f,  -1.0f}, {-1.0f, -1.0f, -1.0f}, {1.0f,  -1.0f, -1.0f}, {1.0f,  1.0f,  -1.0f}},
                            {{1.0f,  1.0f,  1.0f},  {1.0f,  -1.0f, 1.0f},  {-1.0f, -1.0f, 1.0f},  {-1.0f, 1.0f,  1.0f}},
                            {{-1.0f, 1.0f,  1.0f},  {-1.0f, -1.0f, 1.0f},  {-1.0f, -1.0f, -1.0f}, {-1.0f, 1.0f,  -1.0f}},
                            {{1.0f,  1.0f,  -1.0f}, {1.0f,  -1.0f, -1.0f}, {1.0f,  -1.0f, 1.0f},  {1.0f,  1.0f,  1.0f}},
                    };

            std::vector<WireframeVertex> vertices;
            std::vector<uint16_t> indices;

            for (size_t i = 0; i < 6; ++i) {
                uint16_t idx = (uint16_t) vertices.size();
                indices.emplace_back(idx);
                indices.emplace_back(idx + 1);
                indices.emplace_back(idx + 1);
                indices.emplace_back(idx + 2);
                indices.emplace_back(idx + 2);
                indices.emplace_back(idx + 3);
                indices.emplace_back(idx + 3);
                indices.emplace_back(idx);

                for (size_t j = 0; j < 4; ++j) {
                    vertices.emplace_back(positions[i][j], colour);
                }
            }

            auto mesh = WireframeMesh::CreateFromIndices(vertices, indices);
            return mesh;
        }

    protected:
        void RenderGeometry() const override {
            this->_vao.Bind();

            if (this->_indices.empty()) {
                glDrawArrays(this->_drawMode, 0, this->_vertices.size());
            } else {
                this->_ibo.Bind();
                glDrawElements(this->_drawMode, this->_indices.size(), GL_UNSIGNED_SHORT, nullptr);
            }
            
            this->_vao.Unbind();
        }
    };
}
#endif //PARTICLES_WIREFRAME_H
