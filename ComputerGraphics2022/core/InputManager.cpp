#include "InputManager.h"

bool InputManager::IsKeyPressed(sf::Keyboard::Key key) const {
    return sf::Keyboard::isKeyPressed(key);
}

std::pair<int, int> InputManager::GetMousePosition() const {
    auto [x, y] = sf::Mouse::getPosition();
    return { x, y };
}