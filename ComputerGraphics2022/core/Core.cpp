#include "Core.h"

bool Core::getLaunchedState() {
    return launched;
}
void Core::setLaunchedState(bool state)
{
    launched = state;
}
void Core::shutdown() {
    ImGui::SFML::Shutdown();
}