#pragma once

#include "../render/Primitive.h"
#include "../scene/ISceneObject.h"
#include <vector>
#include <memory>
#include ".//Scene.h"
#include "../render/ShaderLoader.h"
#include "../render/Shader.h"
#include "gtc/matrix_transform.hpp"
#include "../render/Light.h"

class Scene {
public:
    Scene(std::shared_ptr<Shader> sh) : shader(sh) {}
    void addEntity(std::shared_ptr<ISceneObject>);
    void deleteEntity(std::shared_ptr<ISceneObject>);
    std::shared_ptr<ISceneObject> getEnity(const std::string& name);
    //std::shared_ptr<ISceneObject> getEnity(const int ID);
    void updateStates();
    auto getObjects() { return Objects; }
    void setLight(math::vec3 pos);
    void setLightShine(float);
    Light getLight();
private:
    std::vector<std::shared_ptr<ISceneObject>> Objects;
    std::shared_ptr<Shader> shader;
    Light light;
};