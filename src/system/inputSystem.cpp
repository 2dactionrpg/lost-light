#include "inputSystem.hpp"
#include "data.hpp"

void InputSystem::on_key(entt::registry& registry, int key, int action, int mod)
{
    auto menu = registry.view<menuComponent>();
    for (auto entity : menu)
    {
        auto &state = menu.get(entity).state;
        auto &debug = menu.get(entity).debug;
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

        if (action == GLFW_PRESS && key == GLFW_KEY_M && state == STATE_PLAYING)
            debug = !debug;
    }

    auto view = registry.view<characterComponent, motionComponent, inputKeyboard>();
    for (auto entity : view) {
        auto&[upKeyPressed, downKeyPressed, leftKeyPressed, rightKeyPressed, resetKeyPressed, saveKeyPressed, loadKeyPressed] = view.get<inputKeyboard>(
                entity);
        auto&[position, direction, radians, speed] = view.get<motionComponent>(entity);
        auto &is_dashable = view.get<characterComponent>(entity).is_dashable;
        auto &dash_duration = view.get<characterComponent>(entity).dash_duration;
        auto &dash_cooldown = view.get<characterComponent>(entity).dash_cooldown;
        auto &dash_direction = view.get<characterComponent>(entity).dash_direction;

        direction = { 0.f, 0.f };

        if (action == GLFW_PRESS && key == GLFW_KEY_R) {
            direction = { 0.f, 0.f };
            resetKeyPressed = true;
        }
        if (action == GLFW_RELEASE && key == GLFW_KEY_R) {
            direction = { 0.f, 0.f };
            resetKeyPressed = false;
        }

        if (action == GLFW_RELEASE && key == GLFW_KEY_LEFT_BRACKET) {
            saveKeyPressed = true;
        }

        if (action == GLFW_RELEASE && key == GLFW_KEY_RIGHT_BRACKET) {
            loadKeyPressed = true;
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

        if (action == GLFW_PRESS && key == GLFW_KEY_SPACE) {
            if (dash_cooldown <= 0.f) {
                // soundSystem.play_sound(S_REFLECT, -1, 0);
                // is_dashable = true;
                dash_direction = direction;
                dash_duration = c_init_dash_duration;
                dash_cooldown = c_init_dash_cooldown;
            }
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
