#include "InputSystem.hpp"

void InputSystem::on_key(entt::registry &registry, int key, int action, int mod)
{
    auto view = registry.view<motionComponent, inputKeyboard>();
    for (auto entity : view)
    {
        auto &[upKeyPressed, downKeyPressed, leftKeyPressed, rightKeyPressed, resetKeyPressed] = view.get<inputKeyboard>(entity);
        auto &[position, direction, radians, speed] = view.get<motionComponent>(entity);

        if (action == GLFW_PRESS && key == GLFW_KEY_R)
        {
            direction = {0.f, 0.f};
            resetKeyPressed = true;
        }
        if (action == GLFW_RELEASE && key == GLFW_KEY_R)
        {
            direction = {0.f, 0.f};
            resetKeyPressed = false;
        }

        if (action == GLFW_PRESS && (key == GLFW_KEY_UP || key == GLFW_KEY_W))
        {
            direction = add(direction, {0.f, -1.f});
            upKeyPressed = true;
        }
        if (action == GLFW_RELEASE && (key == GLFW_KEY_UP || key == GLFW_KEY_W))
        {
            if (direction.y == -1.f) {
                direction = add(direction, {0.f, 1.f});
            }
            upKeyPressed = false;
        }

        if (action == GLFW_PRESS && (key == GLFW_KEY_DOWN || key == GLFW_KEY_S))
        {
            direction = add(direction, {0.f, 1.f});
            downKeyPressed = true;
        }
        if (action == GLFW_RELEASE && (key == GLFW_KEY_DOWN || key == GLFW_KEY_S))
        {
            if (direction.y == 1.f) {
                direction = add(direction, {0.f, -1.f});
            }
            downKeyPressed = false;
        }

        if (action == GLFW_PRESS && (key == GLFW_KEY_LEFT || key == GLFW_KEY_A))
        {
            direction = add(direction, {-1.f, 0.f});
            leftKeyPressed = true;
        }
        if (action == GLFW_RELEASE && (key == GLFW_KEY_LEFT || key == GLFW_KEY_A))
        {
            if (direction.x == -1.f) {
                direction = add(direction, {1.f, 0.f});
            }
            leftKeyPressed = false;
        }

        if (action == GLFW_PRESS && (key == GLFW_KEY_RIGHT || key == GLFW_KEY_D))
        {
            direction = add(direction, {1.f, 0.f});
            rightKeyPressed = true;
        }
        if (action == GLFW_RELEASE && (key == GLFW_KEY_RIGHT || key == GLFW_KEY_D))
        {
            if (direction.x == 1.f) {
                direction = add(direction, {-1.f, 0.f});
            }
            rightKeyPressed = false;
        }
    }
}

void InputSystem::on_mouse(entt::registry &registry, double xpos_new, double ypos_new)
{
    auto view = registry.view<inputMouse>();
    for (auto entity : view)
    {
        auto &xpos = view.get(entity).xpos;
        auto &ypos = view.get(entity).ypos;

        xpos = xpos_new;
        ypos = ypos_new;
    }
}
