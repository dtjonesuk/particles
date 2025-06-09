//
// Created by David on 03/06/2025.
//

#ifndef PARTICLES_GLRESOURCE_H
#define PARTICLES_GLRESOURCE_H

#include <type_traits>
#include <utility>
#include <stdexcept>

namespace framework {

    enum class GlResourceType {
        Shader,
        ShaderProgram,
        Texture2D,
        Buffer,
        VertexArray
    };

    template<GlResourceType T>
    class GlResourceOps {
    public:
        using THandle = GLuint;

        static void Generate(THandle *handle) {
            throw std::runtime_error("Resource generate method not implemented!");
        }
        static void Bind(THandle handle) {
            throw std::runtime_error("Resource bind method not implemented!");
        }
        static void Bind(GLuint target, THandle handle) {
            throw std::runtime_error("Resource bind method not implemented!");
        }
        static void Unbind(THandle handle) {
            throw std::runtime_error("Resource unbind method not implemented!");
        }
        
        static void Delete(THandle handle) {
            throw std::runtime_error("Resource delete method not implemented!");
        }
    };


    template<GlResourceType TResourceType, typename HandleOps = GlResourceOps<TResourceType>>
    class GlResource {
    public:
        using THandle = GLuint;

        // Default constructor
        GlResource() : _handle(0) {}
        
        // Converting constructor
        GlResource(THandle handle) : _handle(handle) {}
        
        /* Copy constructor / assignment */
        GlResource(const GlResource &other) = delete;
        GlResource &operator=(const GlResource &other) = delete;

        /* Move constructor /assignment */
        GlResource(GlResource &&other) noexcept {
            if (other.HasHandle()) {
                _handle = std::exchange(other._handle, 0);
            } else {
                _handle = 0;
            }
        }

        GlResource &operator=(GlResource &&other) noexcept {
            if (other.HasHandle()) {
                _handle = std::exchange(other._handle, 0);
            } else {
                _handle = 0;
            }
            return *this;
        }

        virtual ~GlResource() { if (HasHandle()) FreeHandle(); };

        THandle Handle() const { return _handle; }
        
        void SetHandle(THandle handle) {
            if (HasHandle())
                FreeHandle();
            _handle = handle;
        }

        void Reset() { if (HasHandle()) FreeHandle(); }

        bool HasHandle() const { return _handle != 0; }

        operator bool() const { return HasHandle(); }
        
        void Generate() {
            Reset();
            HandleOps::Generate(&_handle);
        }
        void Bind() const { HandleOps::Bind(Handle()); }
        
        void Bind(GLuint target) const { HandleOps::Bind(target, Handle()); }
        
        void Unbind() const { HandleOps:: Unbind(Handle()); }

    protected:
        THandle _handle;

        void FreeHandle() {
            HandleOps::Delete(Handle());
            _handle = 0;
        };
    };

    typedef GlResource<GlResourceType::Shader> GlShader;
    typedef GlResource<GlResourceType::ShaderProgram> GlShaderProgram;
    typedef GlResource<GlResourceType::Texture2D> GlTexture2d;
    typedef GlResource<GlResourceType::Buffer> GlBuffer;
    typedef GlResource<GlResourceType::VertexArray> GlVertexArray;
    
    /*** Shader ***/
    template<>
    inline void GlResourceOps<GlResourceType::Shader>::Delete(THandle handle) {
        glDeleteShader(handle);
    }

    /*** Shader Program ***/
    template<>
    inline void GlResourceOps<GlResourceType::ShaderProgram>::Bind(THandle handle) {
        glUseProgram(handle);
    }

    template<>
    inline void GlResourceOps<GlResourceType::ShaderProgram>::Unbind(THandle handle) {
        glUseProgram(0);
    }

    template<>
    inline void GlResourceOps<GlResourceType::ShaderProgram>::Delete(THandle handle) {
        glDeleteProgram(handle);
    }

    /*** Texture ***/
    template<>
    inline void GlResourceOps<GlResourceType::Texture2D>::Generate(THandle *handle) {
        glGenTextures(1, handle);
    }
    
    template<>
    inline void GlResourceOps<GlResourceType::Texture2D>::Bind(THandle handle) {
        glBindTexture(GL_TEXTURE_2D, handle);
    }

    template<>
    inline void GlResourceOps<GlResourceType::Texture2D>::Unbind(THandle handle) {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    template<>
    inline void GlResourceOps<GlResourceType::Texture2D>::Delete(THandle handle) {
        glDeleteTextures(1, &handle);
    }

    /*** Vertex Array ***/
    template<>
    inline void GlResourceOps<GlResourceType::VertexArray>::Generate(THandle *handle) {
        glGenVertexArrays(1, handle);
    }
    template<>
    inline void GlResourceOps<GlResourceType::VertexArray>::Bind(THandle handle) {
        glBindVertexArray(handle);
    }

    template<>
    inline void GlResourceOps<GlResourceType::VertexArray>::Unbind(THandle handle) {
        glBindVertexArray(0);
    }

    template<>
    inline void GlResourceOps<GlResourceType::VertexArray>::Delete(THandle handle) {
        glDeleteVertexArrays(1, &handle);
    }

    /*** Buffer ***/
    template<>
    inline void GlResourceOps<GlResourceType::Buffer>::Generate(THandle *handle) {
        glGenBuffers(1, handle);
    }
    template<>
    inline void GlResourceOps<GlResourceType::Buffer>::Bind(GLuint target, THandle handle) {
        glBindBuffer(target, handle);
    }
    template<>
    inline void GlResourceOps<GlResourceType::Buffer>::Delete(THandle handle) {
        glDeleteBuffers(1, &handle);
    }
}
#endif //PARTICLES_GLRESOURCE_H
