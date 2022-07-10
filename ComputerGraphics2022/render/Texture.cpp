#include "Texture.h"


Texture::Texture() {
    glGenTextures(1, &id_.id_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::bind(int slot) const {
    _ASSERT(GL_TEXTURE0 + slot < GL_TEXTURE31);
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, id_.id_);
}

void Texture::unbind(int slot) const{
    glBindTexture(GL_TEXTURE_2D, slot);
}