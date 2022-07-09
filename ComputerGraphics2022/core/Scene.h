#pragma once

#include <string>
#include <vector>
#include "../render/Model.h"
#include "../render/Primitive.h"
#include <memory>
#include "../scene/ISceneObject.h"

class Scene {
public:
    void addEntity(std::shared_ptr<ISceneObject>);
    void deleteEntity(std::shared_ptr<ISceneObject>);
    std::shared_ptr<ISceneObject> getEnity(const std::string& name);
    //std::shared_ptr<ISceneObject> getEnity(const int ID);
    void updateStates();

private:
    std::vector<std::shared_ptr<ISceneObject>> Objects;
};