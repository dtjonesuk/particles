//
// Created by David on 07/07/2025.
//

#ifndef PARTICLES_INSTANCEDMESH_H
#define PARTICLES_INSTANCEDMESH_H

#include "Mesh.h"
#include "StaticMesh.h"


namespace framework {
    class InstancedMesh : public StaticMesh {
    public:
        InstancedMesh() : StaticMesh() {}

        static InstancedMesh CreateFromIndices(const std::vector<VertexUV> &points, const std::vector<uint16_t> &indices) {
            InstancedMesh mesh;
            mesh.CreateVertexBuffer(points);
            mesh.CreateVertexArray();
            mesh.CreateIndexBuffer(indices);
            mesh.AddAttrib(3, GL_FLOAT, offsetof(VertexUV, position));
            mesh.AddAttrib(3, GL_FLOAT, offsetof(VertexUV, normal));
            mesh.AddAttrib(2, GL_FLOAT, offsetof(VertexUV, uv));
            mesh._vao.Unbind();
            return mesh;
        }

        static InstancedMesh CreateFromPoints(const std::vector<VertexUV> &points) {
            InstancedMesh mesh;
            mesh.CreateVertexBuffer(points);
            mesh.CreateVertexArray();
            mesh.AddAttrib(3, GL_FLOAT, offsetof(VertexUV, position));
            mesh.AddAttrib(3, GL_FLOAT, offsetof(VertexUV, normal));
            mesh.AddAttrib(2, GL_FLOAT, offsetof(VertexUV, uv));
            mesh._vao.Unbind();
            return mesh;
        }

    protected:
        void RenderGeometry() override {
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
#endif //PARTICLES_INSTANCEDMESH_H
