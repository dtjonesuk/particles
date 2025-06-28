//
// Created by David on 28/06/2025.
//

#ifndef PARTICLES_BUFFER_H
#define PARTICLES_BUFFER_H
#include <glad/glad.h>
#include "GlResource.h"

namespace framework {
    template<GLenum BufferTarget>
    class Buffer {
        
    public:
        Buffer() = default;
        void Generate() { _buffer.Generate(); }
        void Bind() const { _buffer.Bind(BufferTarget); }
        void Unbind() const { _buffer.Unbind(BufferTarget); }
        
        void BufferData(GLsizeiptr size, const void* data, GLenum usage) {
            glBufferData(BufferTarget, size, data, usage);
        }
        
        void BufferSubData(GLintptr offset, GLsizeiptr size, const void* data) {
            glBufferSubData(BufferTarget, offset, size, data);
        }
        
        void BindToBindingPoint(GLuint bindingPoint) {
            glBindBufferBase(BufferTarget, bindingPoint, _buffer.Handle());
        }
        
        [[nodiscard]] GlBuffer::THandle Handle() const { return _buffer.Handle(); }
    protected:
        GlBuffer _buffer;
    };
}
#endif //PARTICLES_BUFFER_H
