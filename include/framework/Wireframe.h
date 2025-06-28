//
// Created by David on 18/06/2025.
//

#ifndef PARTICLES_WIREFRAME_H
#define PARTICLES_WIREFRAME_H

#include "StaticMesh.h"

namespace framework {
    class Wireframe {
        Wireframe(const StaticMesh &mesh, const std::vector<uint16_t> indices) : _mesh(mesh), _indices(indices) {
            _ibo.Generate();
            _ibo.Bind(GL_ELEMENT_ARRAY_BUFFER);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size(), &indices[0], GL_STATIC_DRAW);
        }

        void Render() {
//            _mesh._vao.Bind();
            _ibo.Bind(GL_ELEMENT_ARRAY_BUFFER);

            glPolygonOffset(1, 1);
            glDrawElements(GL_LINES, this->_indices.size(), GL_UNSIGNED_SHORT, nullptr);
        }


    protected:
        GlBuffer _ibo;
        const StaticMesh &_mesh;
        std::vector<uint16_t> _indices;
    };
}
#endif //PARTICLES_WIREFRAME_H
