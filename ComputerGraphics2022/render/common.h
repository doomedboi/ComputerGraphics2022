#pragma once
#include <GL/glew.h>
#include "mat4x4.hpp"

struct ID {
    ID(size_t id = 0) : id_(id) {}
    GLuint id_;
};

namespace math {
using mat4 = glm::mat<4, 4, float, glm::defaultp>;
using vec3 = glm::vec3;
using vec4 = glm::vec4;    
}


