#include "factory.hpp"

entt::entity makeCharacter(entt::registry& registry)
{

    const entt::entity entity = registry.create();
    registry.assign<characterComponent>(entity, true);
    auto& mo = registry.assign<motionComponent>(entity);
    // Setting initial values
    mo.position = c_init_pos;
    mo.radians = c_init_radians;
    mo.speed = c_init_speed;
    auto& ps = registry.assign<physicsScaleComponent>(entity);
    ps.scale = c_init_scale;
    registry.assign<inputKeyboard>(entity, false, false, false, false, false);
    registry.assign<inputMouse>(entity, 0.0, 0.0);
    return entity;
}

entt::entity makeShield(entt::registry& registry)
{
    const entt::entity entity = registry.create();
    registry.assign<shieldComponent>(entity, false, 0.f, 0.f);
    auto& mo = registry.assign<motionComponent>(entity);
    // Setting initial values
    mo.position = s_init_pos;
    mo.radians = s_init_radians;
    mo.speed = s_init_speed;
    registry.assign<inputMouse>(entity, 0.0, 0.0);
    auto& ps = registry.assign<physicsScaleComponent>(entity);
    ps.scale = s_init_scale;
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
    em.health = 1;
    em.shoot_delay_ms = 3000.f;
    auto& mo = registry.assign<motionComponent>(entity);
    // Setting initial values
    float xpos = randomFloat(50.f, 1000.f);
    float ypos = randomFloat(50.f, 500.f);
    mo.position = { xpos, ypos };
    mo.radians = 20.f;
    mo.speed = 200.f;
    auto& ps = registry.assign<physicsScaleComponent>(entity);
    ps.scale = { 0.08f, 0.08f };
    return entity;
}

entt::entity makeBoss(entt::registry& registry, int id)
{
    const entt::entity entity = registry.create();
    auto& em = registry.assign<enemyComponent>(entity);
    em.id = id;
    em.is_alive = true;
    em.health = 5;
    em.shoot_delay_ms = 3000.f;
    auto& mo = registry.assign<motionComponent>(entity);
    // Setting initial values
    float xpos = randomFloat(50.f, 1000.f);
    float ypos = randomFloat(50.f, 500.f);
    mo.position = { xpos, ypos };
    mo.radians = 20.f;
    mo.speed = 200.f;
    auto& ps = registry.assign<physicsScaleComponent>(entity);
    ps.scale = { 0.2f, 0.2f };
    return entity;
}

entt::entity makeProjectile(entt::registry& registry, int id, vec2 pos, vec2 dir, float rad)
{
    const entt::entity entity = registry.create();
    registry.assign<projectileComponent>(entity, id, false, true);
    auto& mo = registry.assign<motionComponent>(entity);
    mo.position = pos;
    mo.direction = dir;
    mo.radians = rad;
    mo.speed = 1000.f;
    auto& ps = registry.assign<physicsScaleComponent>(entity);
    ps.scale = { 0.22f, 0.22f };
    return entity;
}

entt::entity makeMenu(entt::registry& registry)
{
    const entt::entity entity = registry.create();
    registry.assign<menuComponent>(entity, STATE_START, STATE_START);
    return entity;
}
