#pragma once
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <functional>


/* Note for me: it's class that just listens and updates it's state.
 In case of realisation I must just make class that trigerring on the input.
 Do sfml's functuunality already implement this? After that I hafta add posibility 
 to listen changes. As I assume */
class InputManager final {
public:
    bool IsKeyPressed(sf::Keyboard::Key key) const;

    bool IsKeyReleased(sf::Keyboard::Key p_key) const;
    bool IsMouseButtonPressed(sf::Mouse::Button p_button) const;
    bool IsMouseButtonReleased(sf::Mouse::Button p_button) const;
    std::pair<int, int> GetMousePosition() const;
    void ClearEvents();
    void Process();
private:
    float lastx, lasty;
    bool keys[1024];
    void handleKeyBoard(sf::Event& sf);
    void handleMouse(sf::Event&);
};