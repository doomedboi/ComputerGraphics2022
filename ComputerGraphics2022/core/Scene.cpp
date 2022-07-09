#include "./Scene.h"

void Scene::addEntity(std::shared_ptr<ISceneObject> base) {
    Objects.push_back(base);
}

void Scene::deleteEntity(std::shared_ptr<ISceneObject> entity) {
    //Objects.erase(std::find(std::begin(Objects), std::end(Objects), []))
}



std::shared_ptr<ISceneObject> Scene::getEnity(const std::string& name) {
    for (auto elem : Objects) {
        if (elem->getName() == name)
            return elem;
    }
}

void Scene::updateStates() {
    for (auto elem : Objects) {
        elem->bind();
        elem->draw();
        elem->unbind();
    }
}
