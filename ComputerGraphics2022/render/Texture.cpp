#include "Texture.h"


Texture::Texture() {
    glGenTextures(1, &id_.id_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, id_.id_);
}

void Texture::unbind() const{
    glBindTexture(GL_TEXTURE_2D, 0);
}