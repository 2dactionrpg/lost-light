#pragma once

#include "../character.hpp"
#include "../common.hpp"
#include "../components/input.hpp"
#include "../components/motion.hpp"
#include "../components/player.hpp"
#include "../shield.hpp"
#include <cmath>
#include <entt/entity/registry.hpp>

class PlayerMovementSystem {
private:
public:
    void update(entt::registry& registry, float elapsed_ms, Character& m_character, Shield& m_shield);
    void move(vec2& pos, vec2 off);
    void rotate(float& radians, float newRadians);
    float lengthVec2(vec2 v);
    void setPlayerDead(entt::registry& registry, Character& character);
};
