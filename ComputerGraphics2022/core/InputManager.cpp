#include "InputManager.h"
#include "../core/Core.h"

bool InputManager::IsKeyPressed(sf::Keyboard::Key key) const {
    return sf::Keyboard::isKeyPressed(key);
}

std::pair<int, int> InputManager::GetMousePosition() const {
    auto [x, y] = sf::Mouse::getPosition();
    return { x, y };
}

void InputManager::Process() {
	sf::Event windowEvent;
	while (Core::Singleton().GetRender().getWnd()->pollEvent(windowEvent)) {

		handleMouse(windowEvent);
		handleKeyBoard(windowEvent);
	}

}

void InputManager::handleKeyBoard(sf::Event& ev) {
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
}

void InputManager::handleMouse(sf::Event& ev) {
	if (keys[sf::Keyboard::Escape]) {
		const auto [x, y] = sf::Mouse::getPosition();
		lastx = x, lasty = y;
		sf::Mouse::setPosition(sf::Vector2i(1920 / 2.f, 1080.f / 2), *Core::Singleton().GetRender().getWnd());
	}
}
