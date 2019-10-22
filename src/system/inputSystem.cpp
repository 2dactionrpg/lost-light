#include "inputSystem.hpp"

void InputSystem::on_key(entt::registry& registry, int key, int action, int mod)
{

    auto menu = registry.view<menuComponent>();
    for (auto entity : menu) {
        auto& state = menu.get(entity).state;
        if (action == GLFW_PRESS && key == GLFW_KEY_SPACE && (state == STATE_TUTORIAL || state == STATE_PAUSE))
            state = STATE_PLAYING;

        if (action == GLFW_PRESS && key == GLFW_KEY_SPACE && state == STATE_START)
            state = STATE_TUTORIAL;

        if (action == GLFW_PRESS && key == GLFW_KEY_Q && state != STATE_PLAYING)
            state = STATE_TERMINATE;

        if (action == GLFW_PRESS && key == GLFW_KEY_T && state == STATE_PAUSE)
            state = STATE_TUTORIAL;

        if (action == GLFW_PRESS && key == GLFW_KEY_R && (state == STATE_GAMEOVER || state == STATE_WIN))
            state = STATE_PLAYING;

        if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE && state == STATE_PLAYING)
            state = STATE_PAUSE;
    }

    auto view = registry.view<motionComponent, inputKeyboard>();
    for (auto entity : view) {
        auto& [upKeyPressed, downKeyPressed, leftKeyPressed, rightKeyPressed, resetKeyPressed] = view.get<inputKeyboard>(entity);
        auto& [position, direction, radians, speed] = view.get<motionComponent>(entity);

        direction = { 0.f, 0.f };

        if (action == GLFW_PRESS && key == GLFW_KEY_R) {
            direction = { 0.f, 0.f };
            resetKeyPressed = true;
        }
        if (action == GLFW_RELEASE && key == GLFW_KEY_R) {
            direction = { 0.f, 0.f };
            resetKeyPressed = false;
        }

        if (action == GLFW_PRESS && (key == GLFW_KEY_UP || key == GLFW_KEY_W)) {
            upKeyPressed = true;
        }
        if (action == GLFW_RELEASE && (key == GLFW_KEY_UP || key == GLFW_KEY_W)) {
            upKeyPressed = false;
        }

        if (action == GLFW_PRESS && (key == GLFW_KEY_DOWN || key == GLFW_KEY_S)) {
            downKeyPressed = true;
        }
        if (action == GLFW_RELEASE && (key == GLFW_KEY_DOWN || key == GLFW_KEY_S)) {
            downKeyPressed = false;
        }

        if (action == GLFW_PRESS && (key == GLFW_KEY_LEFT || key == GLFW_KEY_A)) {
            leftKeyPressed = true;
        }
        if (action == GLFW_RELEASE && (key == GLFW_KEY_LEFT || key == GLFW_KEY_A)) {
            leftKeyPressed = false;
        }

        if (action == GLFW_PRESS && (key == GLFW_KEY_RIGHT || key == GLFW_KEY_D)) {
            rightKeyPressed = true;
        }
        if (action == GLFW_RELEASE && (key == GLFW_KEY_RIGHT || key == GLFW_KEY_D)) {
            rightKeyPressed = false;
        }

        if (upKeyPressed) {
            direction = add(direction, { 0.f, -1.f });
        }
        if (downKeyPressed) {
            direction = add(direction, { 0.f, 1.f });
        }
        if (leftKeyPressed) {
            direction = add(direction, { -1.f, 0.f });
        }
        if (rightKeyPressed) {
            direction = add(direction, { 1.f, 0.f });
        }
    }
}

void InputSystem::on_mouse_key(entt::registry& registry, int key, int action, int mod)
{
    auto shield = registry.view<shieldComponent>();

    for (auto entity : shield) {
        auto& [is_reflectable, duration, cooldown] = shield.get(entity);
        if (action == GLFW_PRESS && key == GLFW_MOUSE_BUTTON_LEFT) {
            if (cooldown < 0.f) {
                soundSystem.play_sound(S_REFLECT, -1, 0);
                is_reflectable = true;
                duration = s_init_duration;
                cooldown = duration + s_init_cooldown;
            }
        }
    }
}

void InputSystem::on_mouse_move(entt::registry& registry, double xpos_new, double ypos_new)
{
    auto view = registry.view<inputMouse>();
    for (auto entity : view) {
        auto& xpos = view.get(entity).xpos;
        auto& ypos = view.get(entity).ypos;

        xpos = xpos_new;
        ypos = ypos_new;
    }
}
