#include "factory.hpp"

#include <cmath>

entt::entity makeCharacter(entt::registry &registry)
{

    const entt::entity entity = registry.create();
    auto &c = registry.assign<characterComponent>(entity);
    c.is_movable = true;
    c.is_dashable = true;
    c.dash_speed = c_init_dash_speed;
    c.dash_direction = c_init_dash_direction;
    c.dash_duration = 0.f;
    c.dash_cooldown = 0.f;
    auto &mo = registry.assign<motionComponent>(entity);
    // Setting initial values
    mo.position = c_init_pos;
    mo.radians = c_init_radians;
    mo.speed = c_init_speed;
    auto &ps = registry.assign<physicsScaleComponent>(entity);
    ps.scale = c_init_scale;
    ps.distortion = c_init_distortion;
    ps.sheer = c_init_sheer;
    registry.assign<inputKeyboard>(entity, false, false, false, false, false, false, false);
    registry.assign<inputMouse>(entity, 0.0, 0.0);
    return entity;
}

entt::entity makeShield(entt::registry &registry)
{
    const entt::entity entity = registry.create();
    registry.assign<shieldComponent>(entity, false, 0.f, 0.f);
    auto &mo = registry.assign<motionComponent>(entity);
    // Setting initial values
    mo.position = s_init_pos;
    mo.radians = s_init_radians;
    mo.speed = s_init_speed;
    registry.assign<inputMouse>(entity, 0.0, 0.0);
    auto &ps = registry.assign<physicsScaleComponent>(entity);
    ps.scale = s_init_scale;
    return entity;
}

entt::entity makeMinion(entt::registry &registry, int id, vec2 pos, bool is_movable, bool alert)
{
    const entt::entity entity = registry.create();
    auto &em = registry.assign<enemyComponent>(entity);
    em.id = id;
    em.enemy_type = MINION;
    em.es.alert = alert;
    em.es.enemy_on_sight = false;
    em.es.line_of_sight = false;
    em.es.action = IDLE;
    em.is_alive = true;
    em.health = 1;
    em.shoot_frequency = e_minion_init_shoot_frequency;
    em.shoot_cooldown = e_minion_init_shoot_frequency;
    em.is_movable = is_movable;
    auto &mo = registry.assign<motionComponent>(entity);
    // Setting initial values
    // float xpos = randomFloat(50.f, 1000.f);
    // float ypos = randomFloat(50.f, 500.f);
    mo.position = {pos.x, pos.y};
    mo.radians = M_PI + M_PI / 2;
    mo.speed = 200.f;
    auto &ps = registry.assign<physicsScaleComponent>(entity);
    ps.scale = {0.08f, 0.08f};
    return entity;
}

entt::entity makeBoss(entt::registry &registry, int id, vec2 pos, bool is_movable)
{
    const entt::entity entity = registry.create();
    auto &em = registry.assign<enemyComponent>(entity);
    em.id = id;
    em.enemy_type = BOSS;
    em.is_alive = true;
    em.health = 5;
    em.shoot_frequency = e_boss_init_shoot_frequency;
    em.shoot_cooldown = e_boss_init_shoot_frequency;
    em.is_movable = is_movable;
    auto &mo = registry.assign<motionComponent>(entity);
    // Setting initial values
    // float xpos = randomFloat(50.f, 1000.f);
    // float ypos = randomFloat(50.f, 500.f);
    mo.position = {pos.x, pos.y};
    mo.radians = 20.f;
    mo.speed = 200.f;
    auto &ps = registry.assign<physicsScaleComponent>(entity);
    ps.scale = {0.2f, 0.2f};
    return entity;
}

entt::entity makeProjectile(entt::registry &registry, int id, vec2 pos, vec2 dir, float rad)
{
    const entt::entity entity = registry.create();
    registry.assign<projectileComponent>(entity, id, false, true, 20.0f);
    auto &mo = registry.assign<motionComponent>(entity);
    mo.position = pos;
    mo.direction = dir;
    mo.radians = rad;
    mo.speed = 700.f;
    auto &ps = registry.assign<physicsScaleComponent>(entity);
    ps.scale = {0.22f, 0.22f};
    return entity;
}

entt::entity makePotion(entt::registry &registry, int id)
{
    const entt::entity entity = registry.create();
    registry.assign<potionComponent>(entity, id, false);
    auto &mo = registry.assign<motionComponent>(entity);
    mo.position = {250.f, 600.f};
    mo.radians = 0.f;
    mo.speed = 0.f;
    auto &ps = registry.assign<physicsScaleComponent>(entity);
    ps.scale = {0.07f, 0.07f};
    return entity;
}

entt::entity makeGround(entt::registry &registry, int id)
{
    const entt::entity entity = registry.create();
    registry.assign<groundComponent>(entity, id, false);
    auto &mo = registry.assign<motionComponent>(entity);
    mo.position = {600.f, 400.f};
    mo.radians = 0.f;
    mo.speed = 0.f;
    auto &ps = registry.assign<physicsScaleComponent>(entity);
    ps.scale = {0.8f, 0.8f};
    return entity;
}

entt::entity makeMenu(entt::registry &registry)
{
    const entt::entity entity = registry.create();
    registry.assign<menuComponent>(entity, STATE_START, STATE_START, false);
    return entity;
}

entt::entity makeLevel(entt::registry &registry)
{
    const entt::entity entity = registry.create();
    auto &level = registry.assign<levelComponent>(entity);
    level.minion_killed = 0;
    level.boss_killed = 0;
    return entity;
}
