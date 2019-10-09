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
#include "./components/physicsScaleComponent.hpp"
#include "../components/shieldComponent.hpp"

class ShieldSystem
{
private:
public:
    void update(entt::registry &registry, Shield &m_shield);
    void sync(entt::registry &registry, float elapsed_ms);
};
