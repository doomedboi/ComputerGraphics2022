#pragma once
#include "common.h"


class Texture {
public:
    Texture();
    void bind(int slot) const;
    void unbind() const;
private:
    ID id_;
};