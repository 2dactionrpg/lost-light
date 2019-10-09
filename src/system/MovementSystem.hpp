#pragma once

#include <entt/entity/registry.hpp>
#include <cmath>
#include "../common.hpp"
#include "../character.hpp"
#include "../shield.hpp"
#include "../components/inputMouse.hpp"
#include "../components/inputKeyboard.hpp"
#include "../components/motionComponent.hpp"
#include "../components/characterComponent.hpp"
#include "../components/shieldComponent.hpp"

class MovementSystem
{
private:
public:
    void update(entt::registry &registry, Character &m_character, Shield &m_shield);
    void sync(entt::registry &registry, float elapsed_ms);
    void move(vec2 &pos, vec2 off);
    void rotate(float &radians, float newRadians);
    float lengthVec2(vec2 v);
    void setCharacterUnmovable(entt::registry &registry);
    void resetCharacter(entt::registry &registry);
};
