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
    auto& ps = registry.assign<physicsScaleComponent>(entity);
    ps.scale = { 0.5f, 0.5f };
    registry.assign<inputKeyboard>(entity, false, false, false, false, false);
    registry.assign<inputMouse>(entity, 0.0, 0.0);
    return entity;
}

entt::entity makeShield(entt::registry& registry)
{
    const entt::entity entity = registry.create();
    registry.assign<shieldComponent>(entity, true);
    auto& mo = registry.assign<motionComponent>(entity);
    // Setting initial values3
    mo.position = { 50.f, 300.f };
    mo.radians = 20.f;
    mo.speed = 200.f;
    registry.assign<inputMouse>(entity, 0.0, 0.0);
    auto& ps = registry.assign<physicsScaleComponent>(entity);
    ps.scale = { 0.5f, 0.05f };
    return entity;
}

float randomFloat(float a, float b)
{
    float random = ((float)rand()) / (float)RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

entt::entity makeEnemy(entt::registry& registry, int id)
{
    const entt::entity entity = registry.create();
    auto& em = registry.assign<enemyComponent>(entity);
    em.id = id;
    em.is_alive = true;
    em.shoot_delay_ms = 3000.f;
    auto& mo = registry.assign<motionComponent>(entity);
    // Setting initial values
    float xpos = randomFloat(50.f, 1000.f);
    float ypos = randomFloat(50.f, 500.f);
    mo.position = { xpos, ypos };
    mo.radians = 20.f;
    mo.speed = 200.f;
    auto& ps = registry.assign<physicsScaleComponent>(entity);
    ps.scale = { 0.6f, 0.6f };
    return entity;
}

entt::entity makeBoss(entt::registry& registry, int id)
{
    const entt::entity entity = registry.create();
    auto& em = registry.assign<enemyComponent>(entity);
    em.id = id;
    em.is_alive = true;
    em.shoot_delay_ms = 3000.f;
    auto& mo = registry.assign<motionComponent>(entity);
    // Setting initial values
    float xpos = randomFloat(50.f, 1000.f);
    float ypos = randomFloat(50.f, 500.f);
    mo.position = { xpos, ypos };
    mo.radians = 20.f;
    mo.speed = 200.f;
    auto& ps = registry.assign<physicsScaleComponent>(entity);
    ps.scale = { 0.3f, 0.3f };
    return entity;
}
