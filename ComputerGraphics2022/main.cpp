


#include "core/Core.h"
#include "render/Render.h"


#include "3rd_party/stb/stb_image.h"

#include "render/TextureLoader.h"
#include "core/InputManager.h"

using namespace std;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
bool keys[1024];
bool cameraMove = true;
static float lastx, lasty;
static bool openMenu = true;

void processMove() {
	
	auto cameraSpeed = Core::Singleton().GetViewCamera().getCameraSpeed();
	if (keys[sf::Keyboard::W]) {
		Core::Singleton().GetViewCamera().GetPosition() +=
			Core::Singleton().GetViewCamera().GetFront() * cameraSpeed;
	}
	if (keys[sf::Keyboard::S]) {
		Core::Singleton().GetViewCamera().GetPosition() -=
			Core::Singleton().GetViewCamera().GetFront() * cameraSpeed;
	}
	if (keys[sf::Keyboard::A]) {
		Core::Singleton().GetViewCamera().GetPosition() -=
			glm::normalize(glm::cross(Core::Singleton().GetViewCamera().GetFront(),
				Core::Singleton().GetViewCamera().GetUp())) * cameraSpeed;
	}
	if (keys[sf::Keyboard::D]) {
		Core::Singleton().GetViewCamera().GetPosition() +=
			glm::normalize(glm::cross(Core::Singleton().GetViewCamera().GetFront(),
				Core::Singleton().GetViewCamera().GetUp())) * cameraSpeed;
	}
	if (keys[sf::Keyboard::Escape]) {
		const auto [x, y] = sf::Mouse::getPosition();
		lastx = x, lasty = y;
		sf::Mouse::setPosition(sf::Vector2i(1920 / 2.f, 1080.f / 2), *Core::Singleton().GetRender().getWnd());
	}
}


static int times;
static float rotate = 0.f;


#include "./scene/SceneObject.h"

#include "./core/Scene.h"


int main()
{
	Core::Singleton().InitRender(1900, 1080, 32, "XXXxxXX");
	Core::Singleton().InitCamera(math::vec3(0, 0, 3.f), math::vec3(0, 0.f, -1.f), math::vec3(0, 1.f, 0));

	auto shaderProgram = ShaderLoader::singleton().load("./res/shaders/shader");
	
	

	using namespace math;
	

	math::vec3 translate_vec(0.0f);
	float deg = 0.0f;
	math::vec3 spin_vec(0.0f);

	sf::Clock deltaClock;
	sf::Color bgColor;
	float color[3] = { 0.f, 0.f, 0.f };
	char windowTitle[255] = "ImGui + SFML = <3";

	bool isGo = true;

	
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
		lastx = 1920.f / 2, lasty = 1080 / 2.f;
	}

	while (isGo) {
		sf::Event windowEvent;


		while (Core::Singleton().GetRender().getWnd()->pollEvent(windowEvent)) { // обработка ивентов
			Core::Singleton().GetRender().ProcessIvents(windowEvent);
			switch (windowEvent.type) {
			case sf::Event::Closed:
				isGo = false;
				break;
			case sf::Event::KeyPressed:
				keys[windowEvent.key.code] = true;
				break;
			case sf::Event::KeyReleased:
				keys[windowEvent.key.code] = false;
				break;
			case sf::Event::MouseMoved: {
				const auto [x, y] = sf::Mouse::getPosition();
				float diffX = x - lastx;  float diffy = lasty - y;
				lasty = y, lastx = x;
				if (!openMenu)
					Core::Singleton().GetViewCamera().SetAngles(diffX, diffy, false);
				break;
			}
			default:
				break;
			}
		}

		
		
		processMove();
						//отчистка экрана
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

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Home))
			openMenu = !openMenu;

	}


	ImGui::SFML::Shutdown();
	return 0;
}
