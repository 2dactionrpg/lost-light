#include "factory.hpp"

entt::entity makePlayer(entt::registry& registry)
{

    const entt::entity entity = registry.create();
    registry.assign<player>(entity, true);
    auto& mo = registry.assign<motion>(entity);
    // Setting initial values
    mo.position = { 50.f, 300.f };
    mo.radians = 20.f;
    mo.speed = 200.f;
    registry.assign<input>(entity, false, false, false, false, false, 0.0, 0.0);
    return entity;
}
