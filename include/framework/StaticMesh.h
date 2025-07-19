//
// Created by David on 18/06/2025.
//

#ifndef PARTICLES_STATICMESH_H
#define PARTICLES_STATICMESH_H

#include "Mesh.h"

namespace framework {

    class StaticMesh : public Mesh<VertexUV> {
    public:
        StaticMesh() {
            SetDrawMode(GL_TRIANGLES);
        }

        static StaticMesh CreateFromIndices(const std::vector<VertexUV> &points, const std::vector<uint16_t> &indices) {
            StaticMesh mesh;
            mesh.CreateVertexArray();
            mesh.CreateVertexBuffer(points);
            mesh.CreateIndexBuffer(indices);
            mesh.AddAttrib(3, GL_FLOAT, offsetof(VertexUV, position));
            mesh.AddAttrib(3, GL_FLOAT, offsetof(VertexUV, normal));
            mesh.AddAttrib(2, GL_FLOAT, offsetof(VertexUV, uv));
            mesh._vao.Unbind();
            return mesh;
        }

        static StaticMesh CreateFromPoints(const std::vector<VertexUV> &points) {
            StaticMesh mesh;
            mesh.CreateVertexArray();
            mesh.CreateVertexBuffer(points);
            mesh.AddAttrib(3, GL_FLOAT, offsetof(VertexUV, position));
            mesh.AddAttrib(3, GL_FLOAT, offsetof(VertexUV, normal));
            mesh.AddAttrib(2, GL_FLOAT, offsetof(VertexUV, uv));
            mesh._vao.Unbind();
            return mesh;
        }

        static StaticMesh CreateCube() {
            const glm::vec3 positions[6][4] =
                    {
                            {{-1.0f, -1.0f, -1.0f}, {-1.0f, -1.0f, 1.0f},  {1.0f,  -1.0f, 1.0f},  {1.0f,  -1.0f, -1.0f}},
                            {{-1.0f, 1.0f,  1.0f},  {-1.0f, 1.0f,  -1.0f}, {1.0f,  1.0f,  -1.0f}, {1.0f,  1.0f,  1.0f}},
                            {{-1.0f, 1.0f,  -1.0f}, {-1.0f, -1.0f, -1.0f}, {1.0f,  -1.0f, -1.0f}, {1.0f,  1.0f,  -1.0f}},
                            {{1.0f,  1.0f,  1.0f},  {1.0f,  -1.0f, 1.0f},  {-1.0f, -1.0f, 1.0f},  {-1.0f, 1.0f,  1.0f}},
                            {{-1.0f, 1.0f,  1.0f},  {-1.0f, -1.0f, 1.0f},  {-1.0f, -1.0f, -1.0f}, {-1.0f, 1.0f,  -1.0f}},
                            {{1.0f,  1.0f,  -1.0f}, {1.0f,  -1.0f, -1.0f}, {1.0f,  -1.0f, 1.0f},  {1.0f,  1.0f,  1.0f}},
                    };

            const glm::vec3 normals[6] =
                    {
                            {0.f,  -1.f, 0.f},
                            {0.f,  1.f,  0.f},
                            {0.f,  0.f,  -1.f},
                            {0.f,  0.f,  1.f},
                            {-1.f, 0.f,  0.f},
                            {1.f,  0.f,  0.f},
                    };

            const glm::vec2 texCoords[4] = {{0.f, 0.f},
                                            {1.f, 0.f},
                                            {1.f, 1.f},
                                            {0.f, 1.f}};

            std::vector<VertexUV> vertices;
            std::vector<uint16_t> indices;

            for (size_t i = 0; i < 6; ++i) {
                uint16_t idx = (uint16_t) vertices.size();
                indices.emplace_back(idx);
                indices.emplace_back(idx + 2);
                indices.emplace_back(idx + 1);
                indices.emplace_back(idx);
                indices.emplace_back(idx + 3);
                indices.emplace_back(idx + 2);

                for (size_t j = 0; j < 4; ++j) {
                    vertices.emplace_back(positions[i][j], normals[i], texCoords[j]);
                }
            }

            auto mesh = StaticMesh::CreateFromIndices(vertices, indices);
            return mesh;
        }

        static StaticMesh CreateSphere(int udim, int vdim) {
            const float PI = std::numbers::pi;
            std::vector<VertexUV> vertices;
            std::vector<uint16_t> indices;

            float uStep = 2 * PI / udim;
            float vStep = PI / vdim;

            for (int i = 0; i <= vdim; i++) {
                float phi = PI / 2 - i * vStep;
                float xy = cosf(phi);
                float z = sinf(phi);

                for (int j = 0; j <= udim; ++j) {
                    float theta = j * uStep;
                    VertexUV vertex;

                    vertex.position.x = xy * cosf(theta);
                    vertex.position.z = xy * sinf(theta);
                    vertex.position.y = z;

                    // Unit sphere coordinates are already normalized
                    vertex.normal.x = vertex.position.x;
                    vertex.normal.z = vertex.position.z;
                    vertex.normal.y = vertex.position.y;

                    vertex.uv[0] = j / udim;
                    vertex.uv[1] = i / vdim;

                    vertices.push_back(vertex);
                }
            }

            for (int i = 0; i < vdim; ++i) {
                int k1 = i * (udim + 1);
                int k2 = k1 + udim + 1;

                for (int j = 0; j < udim; ++j, ++k1, ++k2) {
                    if (i != 0) {
                        indices.push_back(k1);
                        indices.push_back(k2);
                        indices.push_back(k1 + 1);

                    }
                    if (i != (vdim - 1)) {
                        indices.push_back(k1 + 1);
                        indices.push_back(k2);
                        indices.push_back(k2 + 1);
                    }

//                wire_indices.push_back(k1);
//                wire_indices.push_back(k2);
//                if (i != 0) {
//                    wire_indices.push_back(k1);
//                    wire_indices.push_back(k1 + 1);
//                }
                }
            }

            auto mesh = StaticMesh::CreateFromIndices(vertices, indices);
//        mesh.SetDrawMode(GL_LINES);
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
        }
    };


}
#endif //PARTICLES_STATICMESH_H
