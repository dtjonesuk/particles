//
// Created by David on 03/06/2025.
//

#ifndef PARTICLES_RESOURCE_H
#define PARTICLES_RESOURCE_H
#include <type_traits>
#include <utility>

namespace framework {
    template<typename THandle, typename = std::enable_if_t<std::is_integral_v<THandle>>>
    class Resource {
    public:
        /* Copy constructor / assignment */
        Resource(const Resource& other) = delete;
        Resource& operator=(const Resource &other) = delete;
         
         /* Move constructor /assignment */
        Resource(Resource&& other) noexcept {
            if (other.HasHandle()) {
                _handle = std::exchange(other._handle, 0);
            } else {
                _handle = 0;
            }
        }
        
        Resource& operator=(Resource &&other) noexcept {
            if (other.HasHandle()) {
                _handle = std::exchange(other._handle, 0);
            } else {
                _handle = 0;
            }
            return *this;
        }
        
        THandle Handle() const { return _handle; }
        
        bool HasHandle() const { return _handle != 0; }

        virtual ~Resource() = default;

    protected:
        Resource() : _handle(0) {}

        THandle _handle;

        void InitializeHandle(THandle handle) {
            _handle = handle;
        }
        
        void SetHandle(THandle handle) {
            if (HasHandle())
                FreeHandle();
            _handle = handle;
        }
        
        virtual void FreeHandle() = 0;
    };
}
#endif //PARTICLES_RESOURCE_H
