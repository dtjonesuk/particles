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
        
        // Check for linking errors:
        GLint params;
        glGetProgramiv( handle, GL_LINK_STATUS, &params );

        // Print the linking log:
        if ( GL_TRUE != params ) {
            int max_length    = 2048, actual_length = 0;
            char plog[2048];
            glGetProgramInfoLog( handle, max_length, &actual_length, plog );
            std::cerr << "ERROR: Could not link shader program GL index " << handle << ".\n" <<  plog << std::endl;
            
            // TODO: fail with shader link error
        }        
    }

    virtual ~ShaderProgram() {
        if (handle)
            glDeleteProgram(handle);
    }
    
    void UseProgram() const { glUseProgram(handle); }

    GLint GetUniformLocation(std::string_view name) const {
        GLint loc = glGetUniformLocation(handle, name.data());
        return loc;
    }

    void SetUniform(GLint uniform, float value) const {
        glUniform1f(uniform, value);
    }

    void SetUniform(GLint uniform, glm::vec3 value) const {
        glUniform3fv(uniform,3, &value[0]);
    }
protected:
    GLuint handle{0};

};


#endif //PARTICLES_SHADERPROGRAM_H
