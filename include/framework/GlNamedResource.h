//
// Created by David on 08/07/2025.
//

#ifndef PARTICLES_GLNAMEDRESOURCE_H
#define PARTICLES_GLNAMEDRESOURCE_H

#include <string>
#include <iostream>
#include <cassert>
#include <fstream>
#include <sstream>
#include <span>

#include "glad/glad.h"
#include "glm/glm.hpp"

namespace framework {
    template<typename GlNamedResourceDeleter>
    class GlNamedResource {
    public:
        using THandle = GLuint;

        // Default constructor
        GlNamedResource() : _handle(0) {}

        // Converting constructor
        GlNamedResource(THandle handle) : _handle(handle) {}

        /* Copy constructor / assignment */
        GlNamedResource(const GlNamedResource(&other)) = delete;

        GlNamedResource (&operator=(const GlNamedResource(&other))) = delete;

        /* Move constructor /assignment */
        GlNamedResource(GlNamedResource(&&other)) noexcept {
            if (other.HasHandle()) {
                _handle = std::exchange(other._handle, 0);
            } else {
                _handle = 0;
            }
        }

        GlNamedResource &operator=(GlNamedResource &&other) noexcept {
            if (other.HasHandle()) {
                _handle = std::exchange(other._handle, 0);
            } else {
                _handle = 0;
            }
            return *this;
        }

        virtual ~GlNamedResource() { if (HasHandle()) FreeHandle(); };

        THandle Handle() const { return _handle; }

        void SetHandle(THandle handle) {
            if (HasHandle())
                FreeHandle();
            _handle = handle;
        }

        void Reset() { if (HasHandle()) FreeHandle(); }

        bool HasHandle() const { return _handle != 0; }

        operator bool() const { return HasHandle(); }

    protected:
        THandle _handle;

        void FreeHandle() {
            GlNamedResourceDeleter::Delete(&_handle);
            _handle = 0;
        }
    };


    class GlNamedShader : public GlNamedResource<GlNamedShader> {
    public:
        GlNamedShader() = default;
        
        GlNamedShader(GLenum shaderType)  {
            Create(shaderType);
        }

        void Create(GLenum shaderType) {
            GLuint handle = glCreateShader(shaderType);
            SetHandle(handle);
        }

        void SetSource(std::string_view source) {
            const GLchar *text = source.data();
            glShaderSource(Handle(), 1, &text, nullptr);
        }

        bool Compile() {
            glCompileShader(Handle());
            return CompileStatus();
        }

        bool CompileStatus() const {
            GLint status;
            glGetShaderiv(Handle(), GL_COMPILE_STATUS, &status);
            return status;
        }

        std::string GetCompilationInfoLog() const {
            GLint infoLogLength;
            glGetShaderiv(Handle(), GL_INFO_LOG_LENGTH, &infoLogLength);
            char *strInfoLog = new char[infoLogLength + 1];
            glGetShaderInfoLog(Handle(), infoLogLength, nullptr, strInfoLog);
            std::string log{strInfoLog};
            delete[] strInfoLog;
            return log;
        }

        static void Delete(GLuint *handle) {
            glDeleteShader(*handle);
        }

    protected:
        
    };

    class GlNamedProgram : public GlNamedResource<GlNamedProgram> {
    public:
        GlNamedProgram() = default;

        void Create() {
            GLuint handle = glCreateProgram();
            SetHandle(handle);
        }

        void AttachShader(const GlNamedShader &shader) {
            glAttachShader(Handle(), shader.Handle());
        }

        bool Link() {
            glLinkProgram(Handle());
            return GetLinkStatus();
        }

        bool GetLinkStatus() const {
            GLint status;
            glGetProgramiv(Handle(), GL_LINK_STATUS, &status);
            return status;
        }

        std::string GetLinkInfoLog() const {
            GLint infoLogLength;
            glGetProgramiv(Handle(), GL_INFO_LOG_LENGTH, &infoLogLength);
            char *strInfoLog = new char[infoLogLength + 1];
            glGetProgramInfoLog(Handle(), infoLogLength, nullptr, strInfoLog);
            std::string msg{strInfoLog};
            delete[] strInfoLog;
            return msg;
        }

        template<typename T>
        void SetUniform(std::string_view location, const T &param) {
            static_assert(false, "Unsupported uniform type!");
        }

        template<>
        void SetUniform<GLint>(std::string_view location, const GLint &param) {
            GLuint loc = glGetUniformLocation(Handle(), location.data());
            glUniform1i(loc, param);
        }

        template<>
        void SetUniform<glm::ivec2>(std::string_view location, const glm::ivec2 &param) {
            GLuint loc = glGetUniformLocation(Handle(), location.data());
            glUniform2iv(loc, 1, &param[0]);
        }

        template<>
        void SetUniform<glm::ivec3>(std::string_view location, const glm::ivec3 &param) {
            GLuint loc = glGetUniformLocation(Handle(), location.data());
            glUniform3iv(loc, 1, &param[0]);
        }

        template<>
        void SetUniform<glm::ivec4>(std::string_view location, const glm::ivec4 &param) {
            GLuint loc = glGetUniformLocation(Handle(), location.data());
            glUniform4iv(loc, 1, &param[0]);
        }

        template<>
        void SetUniform<GLuint>(std::string_view location, const GLuint &param) {
            GLuint loc = glGetUniformLocation(Handle(), location.data());
            glUniform1ui(loc, param);
        }

        template<>
        void SetUniform<GLfloat>(std::string_view location, const GLfloat &param) {
            GLuint loc = glGetUniformLocation(Handle(), location.data());
            glUniform1f(loc, param);
        }

        template<>
        void SetUniform<glm::vec2>(std::string_view location, const glm::vec2 &param) {
            GLuint loc = glGetUniformLocation(Handle(), location.data());
            glUniform2fv(loc, 1, &param[0]);
        }

        template<>
        void SetUniform<glm::vec3>(std::string_view location, const glm::fvec3 &param) {
            GLuint loc = glGetUniformLocation(Handle(), location.data());
            glUniform3fv(loc, 1, &param[0]);
        }

        template<>
        void SetUniform<glm::vec4>(std::string_view location, const glm::vec4 &param) {
            GLuint loc = glGetUniformLocation(Handle(), location.data());
            glUniform4fv(loc, 1, &param[0]);
        }

        template<>
        void SetUniform<glm::mat2>(std::string_view location, const glm::mat2 &param) {
            GLuint loc = glGetUniformLocation(Handle(), location.data());
            glUniformMatrix2fv(loc, 1, GL_FALSE, &param[0][0]);
        }

        template<>
        void SetUniform<glm::mat3>(std::string_view location, const glm::mat3 &param) {
            GLuint loc = glGetUniformLocation(Handle(), location.data());
            glUniformMatrix3fv(loc, 1, GL_FALSE, &param[0][0]);
        }

        template<>
        void SetUniform<glm::mat4>(std::string_view location, const glm::mat4 &param) {
            GLuint loc = glGetUniformLocation(Handle(), location.data());
            glUniformMatrix4fv(loc, 1, GL_FALSE, &param[0][0]);
        }

        void Use() const {
            glUseProgram(Handle());
        }

        void UnUse() const {
            glUseProgram(0);
        }

        static void Delete(GLuint *handle) {
            glDeleteProgram(*handle);
        }

    protected:
    };

    class GlNamedVertexArray : public GlNamedResource<GlNamedVertexArray> {
    public:
        GlNamedVertexArray() {

        }

        void Create() {
            GLuint handle;
            glCreateVertexArrays(1, &handle);
            SetHandle(handle);
        }

        void Bind() {
            glBindVertexArray(Handle());
        }

        void UnBind() {
            glBindVertexArray(0);
        }

        static void Delete(GLuint *handle) {
            glDeleteVertexArrays(1, handle);
        }

    protected:
    };

    class GlNamedBuffer : public GlNamedResource<GlNamedBuffer> {
    public:
        GlNamedBuffer() {

        }

        void Create() {
            GLuint handle;
            glCreateBuffers(1, &handle);
            SetHandle(handle);
        }

        template<typename T>
        void Storage (std::span<T> span, GLbitfield flags) {
            Storage(span.size_bytes(), span.data(), flags);
        }
        
        void Storage(GLsizeiptr size, const void* data, GLbitfield flags) {
            glNamedBufferStorage(Handle(), size, data, flags);
        }
        
        void SubData(GLintptr offset, GLsizeiptr size, const void *data) {
            glNamedBufferSubData(Handle(), offset, size, data);
        }
        
        void BindToBindingPoint(GLenum bufferTarget, GLint bindingPoint) {
            Bind(bufferTarget);
            glBindBufferBase(bufferTarget, bindingPoint, Handle());
            UnBind(bufferTarget);
        }
        
        template<typename T>
        void SubData(std::span<T> span, GLintptr offset) {
            SubData(offset, span.size_bytes(), span.data());
        }
        
        void Bind(GLenum target) {
            glBindBuffer(target, Handle());
        }

        void UnBind(GLenum target) {
            glBindBuffer(target, 0);
        }

        static void Delete(GLuint *handle) {
            glDeleteBuffers(1, handle);
        }

    protected:
    };

}
#endif //PARTICLES_GLNAMEDRESOURCE_H
