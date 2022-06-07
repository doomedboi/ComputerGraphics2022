#pragma once
#include "common.h"


class Texture {
public:
    Texture();
    void bind() const;
    void unbind() const;
private:
    ID id_;
};