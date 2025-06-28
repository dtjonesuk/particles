//
// Created by David on 18/06/2025.
//

#ifndef PARTICLES_STATICMESH_H
#define PARTICLES_STATICMESH_H

#include "Mesh.h"

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

    class StaticMesh : public Mesh<VertexUV> {
    public:
        StaticMesh() {
            SetDrawMode(GL_TRIANGLES);
        }

        void SetTexture(Texture &&tex) {
            texture = std::move(tex);
        }


        static StaticMesh CreateFromIndices(const std::vector<VertexUV> &points, const std::vector<uint16_t> &indices) {
            StaticMesh mesh;
            mesh.CreateVertexBuffer(points);
            mesh.CreateVertexArray();
            mesh.CreateIndexBuffer(indices);
            mesh.AddAttrib(3, GL_FLOAT, offsetof(VertexUV, position));
            mesh.AddAttrib(3, GL_FLOAT, offsetof(VertexUV, normal));
            mesh.AddAttrib(2, GL_FLOAT, offsetof(VertexUV, uv));
            return mesh;
        }

        static StaticMesh CreateFromIndices(const std::vector<VertexUV> &points, const std::vector<uint16_t> &indices,
                                            const std::vector<uint16_t> &wire_indices) {
            StaticMesh mesh;
            mesh.CreateVertexBuffer(points);
            mesh.CreateVertexArray();
            mesh.CreateIndexBuffer(indices);
            mesh.CreateWireframeIndexBuffer(wire_indices);
            mesh.AddAttrib(3, GL_FLOAT, offsetof(VertexUV, position));
            mesh.AddAttrib(3, GL_FLOAT, offsetof(VertexUV, normal));
            mesh.AddAttrib(2, GL_FLOAT, offsetof(VertexUV, uv));
            return mesh;
        }

        static StaticMesh CreateFromPoints(const std::vector<VertexUV> &points) {
            StaticMesh mesh;
            mesh.CreateVertexBuffer(points);
            mesh.CreateVertexArray();
//            mesh.drawMode = GL_POINTS;
            mesh.AddAttrib(3, GL_FLOAT, offsetof(VertexUV, position));
            mesh.AddAttrib(3, GL_FLOAT, offsetof(VertexUV, normal));
            mesh.AddAttrib(2, GL_FLOAT, offsetof(VertexUV, uv));
            return mesh;
        }

        void RenderWireframe() {
            if (_wire_indices.empty())
                return;

            this->_vao.Bind();

            glPolygonOffset(1, 1);
//            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            this->_wibo.Bind(GL_ELEMENT_ARRAY_BUFFER);
            glDrawElements(GL_LINES, this->_wire_indices.size(), GL_UNSIGNED_SHORT, nullptr);
        }


    protected:
        Texture texture;

        void RenderGeometry()

        override {
            this->_vao.

                    Bind();

            this->_ibo.Bind(GL_ELEMENT_ARRAY_BUFFER);
            if (texture) {
                texture.

                        Bind();
            }
            if (this->_indices.

                    empty()

                    ) {
                glDrawArrays(this->_drawMode, 0, this->_vertices.

                        size()

                );
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                glDrawElements(this->_drawMode, this->_indices.

                                       size(),

                               GL_UNSIGNED_SHORT, nullptr);

//                glPolygonOffset(1 ,1);
//                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//                this->_wibo.Bind(GL_ELEMENT_ARRAY_BUFFER);
//                glDrawElements(GL_LINES, this->_wire_indices.size(), GL_UNSIGNED_SHORT, nullptr);
            }
        }
    };

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
                vertices.emplace_back(VertexUV{positions[i][j], normals[i], texCoords[j]});
            }
        }

        auto mesh = StaticMesh::CreateFromIndices(vertices, indices);
        return mesh;
    }

    static StaticMesh CreateSphere(int udim, int vdim) {
        const float PI = std::numbers::pi;
        std::vector<VertexUV> vertices;
        std::vector<uint16_t> indices;
        std::vector<uint16_t> wire_indices;

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

                wire_indices.push_back(k1);
                wire_indices.push_back(k2);
                if (i != 0) {
                    wire_indices.push_back(k1);
                    wire_indices.push_back(k1 + 1);
                }
            }
        }

        auto mesh = StaticMesh::CreateFromIndices(vertices, indices, wire_indices);
//        mesh.SetDrawMode(GL_LINES);
        return mesh;
    }

}
#endif //PARTICLES_STATICMESH_H
