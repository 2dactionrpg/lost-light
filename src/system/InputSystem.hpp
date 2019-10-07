#pragma once

#include <entt/entity/registry.hpp>
#include "../components/inputMouse.hpp"
#include "../components/inputKeyboard.hpp"
#include "../common.hpp"

struct InputSystem
{
    void on_key(entt::registry &registry, int key, int action, int mod);
    void on_mouse(entt::registry &registry, double xpos, double ypos);
};