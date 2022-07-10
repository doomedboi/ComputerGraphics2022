#pragma once
#include "./common.h"
#include <memory>
#include "Shader.h"

class Light {
public:
    Light() {
        materialShine = 0.5;
        lightPos = math::vec3(0.0f, 0.0f, 5.0f);
        lightAmb = math::vec3(0.2f, 0.2f, 0.2f);
        lightDiff = math::vec3(0.5f, 0.5f, 0.5f);
        lightSpec = math::vec3(1.0f, 1.0f, 1.0f);
    }
    math::vec3 getPose();
    float getShine();
    void setPose(const math::vec3& pos);
    void setShine(float shine);
    void draw(std::shared_ptr<Shader>);
private:
    math::vec3 lightPos;
    math::vec3 lightAmb;
    math::vec3 lightDiff;
    math::vec3 lightSpec;

    math::vec3 materialSpec = math::vec3(1);
    float materialShine;
};
