#pragma once
#include "../3rd_party/glm/matrix.hpp"
#include <GL/glew.h>
#include <glm.hpp>
#include <string>
#include "common.h"

class Shader {
public:
    Shader(ID id);
    void bind();
    void unbind();
    void setUniform(const std::string& fileName, const glm::mat4& mat);

private:
    ID id_;
};