//
// Created by David on 07/06/2025.
//

#ifndef PARTICLES_TEXTURE_H
#define PARTICLES_TEXTURE_H

#include <filesystem>
#include <stb_image.h>
#include "glad/glad.h"

#include "GlResource.h"

namespace framework {
    class Texture  {
    public:
        Texture() = default;

        Texture(Texture &&other) {
            std::swap(texture, other.texture);
        }

        Texture &operator=(Texture &&other) {
            texture.Reset();
            std::swap(texture, other.texture);
            return *this;
        }
        
        operator bool() const {
            return texture.HasHandle();
        }

        void Bind() {
            texture.Bind();
        }
        
        void Unbind() {
            texture.Unbind();
        }

        static Texture CreateFromFile(const std::filesystem::path &path) {
            Texture tex;
            
            // load image from file
            int width, height, nChannels;
            stbi_set_flip_vertically_on_load(true);

            unsigned char *data = stbi_load(path.string().c_str(), &width, &height, &nChannels, 0);
            std::cout << "nr channels: " << nChannels << std::endl;

            // create texture object
//            GLuint handle;
            tex.texture.Generate();
//            glGenTextures(1, &handle);
//            texture.SetHandle(handle);

            // bind texture to context
            tex.texture.Bind();
            //glBindTexture(GL_TEXTURE_2D, handle);

            // set the texture wrapping/filtering options (on the currently bound texture object)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // set the texture image data
            if (nChannels == 3) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            } else if (nChannels == 4) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            } else {
                std::cerr << "Texture: unsupported nr of channels (" << nChannels << ")." << std::endl;
            }
            glGenerateMipmap(GL_TEXTURE_2D);

            // free stbi image data
            stbi_image_free(data);
            
            return tex;
        }
    protected:
        GlTexture2d texture;
    };
}
#endif //PARTICLES_TEXTURE_H
