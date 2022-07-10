#pragma once
#include "Texture.h"
#include "../3rd_party/stb/stb_image.h"
#include <string>

class TextureLoader {
public:
    Texture load(const std::string& path) {
        stbi_set_flip_vertically_on_load(true);
        
        int w, h, cChannels;
        auto* textureRowData = stbi_load(path.c_str(), &w, &h, &cChannels, 0);
        if (!textureRowData) {
            _ASSERT("Error obtaining texture data");
        }

        auto texture = Texture();
        texture.bind(0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, textureRowData);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(textureRowData);

        texture.unbind(0);

        return texture;
    }
};