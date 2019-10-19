#pragma once

#include "../common.hpp"
#include "../components/inputKeyboard.hpp"
#include "../components/inputMouse.hpp"
#include "../components/motionComponent.hpp"
#include "../components/shieldComponent.hpp"
#include <entt/entity/registry.hpp>

struct InputSystem {
    void on_key(entt::registry& registry, int key, int action, int mod);
    void on_mouse_key(entt::registry& registry, int key, int action, int mod);
    void on_mouse_move(entt::registry& registry, double xpos, double ypos);
};
