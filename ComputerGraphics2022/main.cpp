
#include <iostream>

//glew
#include <GL/glew.h>
#include "3rd_party/glm/gtc/matrix_transform.hpp"

//sfml
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "imgui-SFML.h"
#include "imgui.h"


#include <glfw3.h>




#include "./utils/assimpParser.h"

#include "render/Render.h"
#include "render/ShaderLoader.h"

#include <glm.hpp>
#include "math.h"
#include "3rd_party/stb/stb_image.h"

#include "render/TextureLoader.h"
#include "core/InputManager.h"





#include "core/Core.h"
using namespace std;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
bool keys[1024];
bool cameraMove = true;
static float lastx, lasty;

void processMove() {
	GLfloat cameraSpeed = 0.001f;
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

#include "./render/Model.h"
#include "./render/Mesh.h"

#include "./core/Scene.h"



static int times;
static float rotate = 0.f;


#include "./scene/SceneObject.h"


int main()
{
	Core::Singleton().InitRender(1900, 1080, 32, "XXXxxXX");
	Core::Singleton().InitCamera(math::vec3(0, 0, 3.f), math::vec3(0, 0.f, -1.f), math::vec3(0, 1.f, 0));

	auto shaderProgram = ShaderLoader::singleton().load("./res/shaders/shader");

	

	using namespace math;
		
	auto lightPos = math::vec3(0.0f, 0.0f, 5.0f);
	auto lightAmb = vec3(0.2f, 0.2f, 0.2f);
	auto lightDiff = vec3(0.5f, 0.5f, 0.5f);
	auto lightSpec = vec3(1.0f, 1.0f, 1.0f);

	auto materialSpec = vec3(0.5f, 0.5f, 0.5f);
	auto materialShine = 32.0f;

	math::vec3 translate_vec(0.0f);
	float deg = 0.0f;
	math::vec3 spin_vec(0.0f);

	sf::Clock deltaClock;
	sf::Color bgColor;
	float color[3] = { 0.f, 0.f, 0.f };
	char windowTitle[255] = "ImGui + SFML = <3";

	bool isGo = true;

	
	auto im = InputManager();

	Scene scene;
	
	auto obj = std::make_shared<SceneObject>();
	auto obj1 = std::make_shared<SceneObject>();
	obj1->loadTexture("./res/models/rp_eric_rigged_001_dif.jpg");
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
				Core::Singleton().GetViewCamera().SetAngles(diffX, diffy, false);
				break;
			}
			default:
				break;
			}
		}

		
		
		processMove();
		glClearColor(0.1f, 0.6f, 0.3f, 1.0f);				//отчистка экрана
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //отчистка экрана

		
		shaderProgram.bind();
		
		auto model = math::mat4(1.f);
		
		model = glm::scale(model, glm::vec3(0.005, 0.005, 0.005));

		auto viewMat = Core::Singleton().GetViewCamera().GetViewMatrix();

		glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 800 / 600.f, 0.1f, 100.0f);


		shaderProgram.setUniform("light.ambient", lightAmb);
		shaderProgram.setUniform("light.diffuse", lightDiff);
		shaderProgram.setUniform("light.specular", lightSpec);
		shaderProgram.setUniform("light.position", lightPos);

		shaderProgram.setUniform("material.diffuse", 0);
		shaderProgram.setUniform("material.normalMap", 1);
		shaderProgram.setUniform("material.specular", materialSpec);
		shaderProgram.setUniform("material.shininess", materialShine);

		shaderProgram.setUniform("viewPos", glm::vec3(3, 3, 3));

		shaderProgram.setUniform("model", model);
		shaderProgram.setUniform("view", viewMat);
		shaderProgram.setUniform("projection", Projection);

		scene.updateStates();
		
		shaderProgram.unbind();					

		Core::Singleton().GetRender().DrawMenu();

	}


	ImGui::SFML::Shutdown();
	return 0;
}
