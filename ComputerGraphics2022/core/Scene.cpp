#include "./Scene.h"

void Scene::addEntity(std::shared_ptr<ISceneObject> base) {
    Objects.push_back(base);
}

void Scene::deleteEntity(std::shared_ptr<ISceneObject> entity) {
    auto pos = std::find(std::begin(Objects), std::end(Objects), entity);
    if (pos != std::end(Objects)) {
        Objects.erase(pos);
    }
}



std::shared_ptr<ISceneObject> Scene::getEnity(const std::string& name) {
    for (int i = 0; i < Objects.size(); i++) {
        if (Objects[i]->getName() == name)
            return Objects[i];
    }
}

void Scene::updateStates() {

    light.draw(shader);

    for (auto elem : Objects) {
        elem->bind();
        // update here model mat for every object to able to move scale etc separetely
        auto model = math::mat4(1.f);

        model = glm::scale(model, elem->getScale());
        model = glm::translate(model, elem->getPos());
                
        shader->setUniform("model", model);
        elem->draw();
        elem->unbind();
    }
}

void Scene::setLight(math::vec3 pos)
{
    light.setPose(pos);
}

void Scene::setLightShine(float sh)
{
    light.setShine(sh);
}

Light Scene::getLight()
{
    return light;
}
