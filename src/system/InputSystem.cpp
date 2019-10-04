#include "InputSystem.hpp"

void InputSystem::update(entt::registry &registry, int key, int action, int mod)
{
    auto view = registry.view<input>();
    for (auto entity : view)
    {
        auto &upKeyPressed = view.get(entity).upKeyPressed;
        auto &downKeyPressed = view.get(entity).downKeyPressed;
        auto &leftKeyPressed = view.get(entity).leftKeyPressed;
        auto &rightKeyPressed = view.get(entity).rightKeyPressed;

        if (action == GLFW_PRESS && (key == GLFW_KEY_UP || key == GLFW_KEY_W))
            upKeyPressed = true;
        if (action == GLFW_RELEASE && (key == GLFW_KEY_UP || key == GLFW_KEY_W))
            upKeyPressed = false;

        if (action == GLFW_PRESS && (key == GLFW_KEY_DOWN || key == GLFW_KEY_S))
            downKeyPressed = true;
        if (action == GLFW_RELEASE && (key == GLFW_KEY_DOWN || key == GLFW_KEY_S))
            downKeyPressed = false;

        if (action == GLFW_PRESS && (key == GLFW_KEY_LEFT || key == GLFW_KEY_A))
            leftKeyPressed = true;
        if (action == GLFW_RELEASE && (key == GLFW_KEY_LEFT || key == GLFW_KEY_A))
            leftKeyPressed = false;

        if (action == GLFW_PRESS && (key == GLFW_KEY_RIGHT || key == GLFW_KEY_D))
            rightKeyPressed = true;
        if (action == GLFW_RELEASE && (key == GLFW_KEY_RIGHT || key == GLFW_KEY_D))
            rightKeyPressed = false;
    }
}