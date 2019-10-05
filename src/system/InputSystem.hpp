#pragma once

#include <entt/entity/registry.hpp>
#include "../components/input.hpp"
#include "../components/player.hpp"
#include "../components/motion.hpp"
#include "../common.hpp"

struct InputSystem
{
    void on_key(entt::registry &registry, int key, int action, int mod);
    void on_mouse(entt::registry &registry, double xpos, double ypos);
};