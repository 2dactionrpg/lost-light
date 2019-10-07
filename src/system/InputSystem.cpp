#include "InputSystem.hpp"

void InputSystem::on_key(entt::registry& registry, int key, int action, int mod)
{
    auto view = registry.view<inputKeyboard>();
    for (auto entity : view) {
        auto& upKeyPressed = view.get(entity).upKeyPressed;
        auto& downKeyPressed = view.get(entity).downKeyPressed;
        auto& leftKeyPressed = view.get(entity).leftKeyPressed;
        auto& rightKeyPressed = view.get(entity).rightKeyPressed;
        auto& resetKeyPressed = view.get(entity).resetKeyPressed;

        if (action == GLFW_PRESS && key == GLFW_KEY_R) {
            resetKeyPressed = true;
        }
        if (action == GLFW_RELEASE && key == GLFW_KEY_R) {
            resetKeyPressed = false;
        }

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

void InputSystem::on_mouse(entt::registry& registry, double xpos_new, double ypos_new)
{
    auto view = registry.view<inputMouse>();
    for (auto entity : view) {
        auto& xpos = view.get(entity).xpos;
        auto& ypos = view.get(entity).ypos;

        xpos = xpos_new;
        ypos = ypos_new;
    }
}
