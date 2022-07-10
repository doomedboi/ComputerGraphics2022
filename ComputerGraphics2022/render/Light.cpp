#include "Light.h"

math::vec3 Light::getPose() {
    return lightPos;
}

float Light::getShine()
{
    return materialShine;
}

void Light::setPose(const math::vec3& pos)
{
    lightPos = pos;
}

void Light::setShine(float shine)
{
    materialShine = shine;
}

void Light::draw(std::shared_ptr<Shader> shader) {
    shader->setUniform("material.diffuse", 0);
    shader->setUniform("material.normalMap", 1);
    shader->setUniform("material.specular", materialSpec);
    shader->setUniform("material.shininess", materialShine);

    shader->setUniform("light.ambient", lightAmb);
    shader->setUniform("light.diffuse", lightDiff);
    shader->setUniform("light.specular", lightSpec);
    shader->setUniform("light.position", lightPos);
}
