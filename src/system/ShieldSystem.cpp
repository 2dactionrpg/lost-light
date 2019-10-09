#include "ShieldSystem.hpp"
#include <iostream>

void ShieldSystem::sync(entt::registry &registry, float ms)
{
}

void ShieldSystem::update(entt::registry &registry, Shield &m_shield)
{

    auto shield = registry.view<shieldComponent, physicsScaleComponent>();

    for (auto entity : shield)
    {
        auto &is_reflectable = shield.get<shieldComponent>(entity).is_reflectable;
        auto &scale = shield.get<physicsScaleComponent>(entity).scale;
        if (is_reflectable) {
            m_shield.show(scale.x, scale.y);
        } else {
            m_shield.hide();
        }
    }
}