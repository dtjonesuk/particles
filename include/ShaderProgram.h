//
// Created by David on 30/05/2025.
//

#ifndef PARTICLES_SHADERPROGRAM_H
#define PARTICLES_SHADERPROGRAM_H

#include <memory>
#include "Shader.h"

class ShaderProgram {
public:
    template<typename ...Shaders>
    explicit ShaderProgram(Shaders &...shaders) {
        handle = glCreateProgram();

        (glAttachShader(handle, shaders.handle), ...);

        glLinkProgram(handle);
    }

    virtual ~ShaderProgram() {
        if (handle)
            glDeleteProgram(handle);
    }
    
    void UseProgram() { glUseProgram(handle); }

protected:
    GLuint handle{0};

};


#endif //PARTICLES_SHADERPROGRAM_H
