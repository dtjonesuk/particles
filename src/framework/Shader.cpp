//
// Created by David on 11/07/2025.
//
#include "framework/Shader.h"

namespace framework {
    namespace Shader {
        void CompileShader(GlNamedShader &shader, std::string_view sourceCode) {
            assert(sourceCode.data());

            shader.SetSource(sourceCode);

            bool compiled = shader.Compile();

            // Check compilation status and report error
            if (!compiled) {
                std::string msg("Shader Compilation Error: ");

                msg += shader.GetCompilationInfoLog();

                std::cerr << msg << std::endl;

                throw std::runtime_error(msg);
            }
        }

        GlNamedShader FromFile(GLenum shaderType, std::string_view path) {
            std::ifstream file{path.data()};
            std::stringstream ss;
            ss << file.rdbuf();

            GlNamedShader shader(shaderType);
            CompileShader(shader, ss.str());
            return shader;
        }

    }
}