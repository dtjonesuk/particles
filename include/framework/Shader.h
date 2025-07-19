//
// Created by David on 30/05/2025.
//

#ifndef PARTICLES_SHADER_H
#define PARTICLES_SHADER_H



#include "GlNamedResource.h"

namespace framework {
    namespace Shader {
        void CompileShader(GlNamedShader &shader, std::string_view sourceCode);

        GlNamedShader FromFile(GLenum shaderType, std::string_view path);
    }
}
#endif //PARTICLES_SHADER_H
