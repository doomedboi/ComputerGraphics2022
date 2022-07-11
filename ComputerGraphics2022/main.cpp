


#include "core/Core.h"
#include "render/Render.h"


#include "3rd_party/stb/stb_image.h"

#include "render/TextureLoader.h"
#include "core/InputManager.h"

using namespace std;
static bool openMenu = true;


#include "./scene/SceneObject.h"

#include "./core/Scene.h"


int main()
{
    Core::Singleton().InitRender(1900, 1080, 32, "XXXxxXX");
    Core::Singleton().InitCamera(math::vec3(0, 0, 3.f), math::vec3(0, 0.f, -1.f), math::vec3(0, 1.f, 0));

    auto shaderProgram = ShaderLoader::singleton().load("./res/shaders/shader");

    using namespace math;

    auto im = InputManager();

    Scene scene(std::make_shared<Shader>(shaderProgram));

    auto obj = std::make_shared<SceneObject>(); obj->setName("obj0");
    auto obj1 = std::make_shared<SceneObject>(); obj1->setName("obj1");
    obj1->loadTexture("./res/pistol_colt/colt_low_polyfbx_m1911_BaseColor.png");
    obj1->loadModel(std::string("./res/pistol_colt/colt low polyfbx.fbx"));

    scene.addEntity(obj);
    scene.addEntity(obj1);

    if (Core::Singleton().GetRender().getWnd()->hasFocus()) {
        sf::Mouse::setPosition(sf::Vector2i(1920.f / 2, 1080.f / 2), *Core::Singleton().GetRender().getWnd());
    }

    while (Core::Singleton().getLaunchedState()) {
        im.Process();


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //отчистка экрана

        shaderProgram.bind();

        auto viewMat = Core::Singleton().GetViewCamera().GetViewMatrix();

        glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 800 / 600.f, 0.1f, 100.0f);

        shaderProgram.setUniform("viewPos", Core::Singleton().GetViewCamera().GetPosition());

        shaderProgram.setUniform("view", viewMat);
        shaderProgram.setUniform("projection", Projection);

        scene.updateStates();

        shaderProgram.unbind();


        Core::Singleton().GetRender().DrawMenu(openMenu, scene);

        if (im.IsKeyPressed(sf::Keyboard::Home))
            openMenu = !openMenu;

    }


    Core::Singleton().shutdown();
    return 0;
}
