#include "factory.hpp"

entt::entity makeCharacter(entt::registry& registry)
{

    const entt::entity entity = registry.create();
    registry.assign<characterComponent>(entity, true);
    auto& mo = registry.assign<motionComponent>(entity);
    // Setting initial values
    mo.position = { 50.f, 300.f };
    mo.radians = 20.f;
    mo.speed = 200.f;
    registry.assign<inputKeyboard>(entity, false, false, false, false, false);
    registry.assign<inputMouse>(entity, 0.0, 0.0);
    return entity;
}

entt::entity makeShield(entt::registry& registry)
{
    const entt::entity entity = registry.create();
    registry.assign<shieldComponent>(entity);
    auto& mo = registry.assign<motionComponent>(entity);
    // Setting initial values
    mo.position = { 50.f, 300.f };
    mo.radians = 20.f;
    mo.speed = 200.f;
    registry.assign<inputMouse>(entity, 0.0, 0.0);
    return entity;
}