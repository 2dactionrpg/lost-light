#include "physicsSystem.hpp"

void PhysicsSystem::sync(entt::registry& registry, float ms)
{
    // Character Physics Sync
    auto view = registry.view<characterComponent, motionComponent, inputKeyboard, inputMouse>();
    for (auto entity : view) {
        auto& [position, direction, radians, speed] = view.get<motionComponent>(entity);
        auto& [is_movable, is_dashable, cooldown] = view.get<characterComponent>(entity);
        auto& resetKeyPressed = view.get<inputKeyboard>(entity).resetKeyPressed;
        auto& [xpos, ypos] = view.get<inputMouse>(entity);

        float step = speed * (ms / 1000);
        vec2 offset = mul(normalize(direction), step);

        if (is_dashable) {
            vec2 dash_distance = mul(normalize(direction), c_init_dash_distance);
            offset = add(offset, dash_distance);
            is_dashable = false;
        }

        cooldown -= 1.f;

        if (resetKeyPressed) {
            reset_character(registry);
        }

        if (is_movable) {
            move(position, offset, true);
            rotate(radians, xpos, ypos, position);
        }
    }

    // Shield Physics Sync
    auto viewCharacter = registry.view<characterComponent, motionComponent>();
    auto viewShield = registry.view<shieldComponent, motionComponent, inputMouse>();
    for (auto character : viewCharacter) {
        auto& position = viewCharacter.get<motionComponent>(character).position;
        auto& is_movable = viewCharacter.get<characterComponent>(character).is_movable;
        if (is_movable) {
            for (auto shield : viewShield) {
                auto& [is_reflectable, duration, cooldown] = viewShield.get<shieldComponent>(shield);
                auto& radians = viewShield.get<motionComponent>(shield).radians;
                auto& [xpos, ypos] = viewShield.get<inputMouse>(shield);

                if (is_reflectable) {
                    duration -= 1.f;
                }

                if (duration < 0.f) {
                    is_reflectable = false;
                }

                cooldown -= 1.f;

                rotate(radians, xpos, ypos, position);
            }
        }
    }

    // Enemy Physics Sync
    auto viewEnemy = registry.view<enemyComponent, motionComponent>();
    for (auto enemy : viewEnemy) {
        auto& [position, direction, radians, speed] = viewEnemy.get<motionComponent>(enemy);
        // auto &destination = viewEnemy.get<enemyComponent>(enemy).is_alive;
        auto& is_alive = viewEnemy.get<enemyComponent>(enemy).is_alive;
        auto& destination = viewEnemy.get<enemyComponent>(enemy).destination;
        float step = speed * (ms / 1000);
        if (is_alive) {
            if (direction.x > 0) {
                if (position.x + direction.x > destination.x) {
                    direction.x = 0.f;
                }
            } else if (direction.x < 0) {
                if (position.x + direction.x < destination.x) {
                    direction.x = 0.f;
                }
            }
            if (direction.y > 0) {
                if (position.y + direction.y > destination.y) {
                    direction.y = 0.f;
                }
            } else if (direction.y < 0) {
                if (position.y + direction.y < destination.y) {
                    direction.y = 0.f;
                }
            }
            vec2 offset = mul(normalize(direction), step);
            move(position, offset, true);
        }
    }

    // Sync Projectile physics
    auto viewProjectile = registry.view<projectileComponent, motionComponent>();
    for (auto projectile : viewProjectile) {
        auto& [position, direction, radians, speed] = viewProjectile.get<motionComponent>(projectile);
        auto& is_alive = viewProjectile.get<projectileComponent>(projectile).is_alive;
        float step = speed * (ms / 1000);
        if (is_alive) {
            vec2 offset = mul(normalize(direction), step);
            move(position, offset, false);
        }
    }
}

void PhysicsSystem::update(entt::registry& registry, Character& m_character, Shield& m_shield, vector<Enemy>& m_enemies, vector<Projectile>& m_projectiles, Potion& m_potion)
{
    // Character Physics Update
    auto character = registry.view<characterComponent, motionComponent, physicsScaleComponent>();
    for (auto entity : character) {
        auto& position = character.get<motionComponent>(entity).position;
        auto& radians = character.get<motionComponent>(entity).radians;
        auto& scale = character.get<physicsScaleComponent>(entity).scale;

        m_character.set_position(position);
        m_character.set_rotation(radians);
        m_character.set_scale(scale);
    }

    // Shield Physics Update
    auto shield = registry.view<shieldComponent, motionComponent, physicsScaleComponent>();

    for (auto entity : shield) {
        auto& radians = shield.get<motionComponent>(entity).radians;
        auto& [is_reflectable, duration, cooldown] = shield.get<shieldComponent>(entity);
        auto& scale = shield.get<physicsScaleComponent>(entity).scale;

        m_shield.set_position(m_character.get_position());
        m_shield.set_rotation(radians);
        if (is_reflectable) {
            m_shield.set_scale(scale);
        } else {
            m_shield.hide();
        }
    }

    // Enemy Physics Update
    auto enemy = registry.view<enemyComponent, motionComponent, physicsScaleComponent>();

    for (auto entity : enemy) {
        auto& position = enemy.get<motionComponent>(entity).position;
        auto& radians = enemy.get<motionComponent>(entity).radians;
        auto& id = enemy.get<enemyComponent>(entity).id;
        auto& scale = enemy.get<physicsScaleComponent>(entity).scale;
        for (auto& m_enemy : m_enemies) {
            if (id == m_enemy.get_id()) {
                m_enemy.set_position(position);
                m_enemy.set_rotation(radians);
                m_enemy.set_scale(scale);
            }
        }
    }

    // Projectile Physics Update
    auto projectile = registry.view<projectileComponent, motionComponent, physicsScaleComponent>();

    for (auto entity : projectile) {
        auto& [position, direction, radians, speed] = projectile.get<motionComponent>(entity);
        auto& id = projectile.get<projectileComponent>(entity).id;
        auto& scale = projectile.get<physicsScaleComponent>(entity).scale;
        for (auto& m_projectile : m_projectiles) {
            if (id == m_projectile.get_id()) {
                m_projectile.set_position(position);
                m_projectile.set_rotation(radians);
                m_projectile.set_direction(direction);
                m_projectile.set_scale(scale);
            }
        }
    }

    // Potion Physics Update
    auto potion = registry.view<potionComponent, motionComponent, physicsScaleComponent>();

    for (auto entity : potion) {
        auto& [position, direction, radians, speed] = potion.get<motionComponent>(entity);
        auto& id = potion.get<potionComponent>(entity).id;
        auto& is_consumed = potion.get<potionComponent>(entity).is_consumed;
        auto& scale = potion.get<physicsScaleComponent>(entity).scale;
        m_potion.set_position(position);
        m_potion.set_scale(scale);
        m_potion.set_is_consumed(is_consumed);
    }
}

void PhysicsSystem::move(vec2& pos, vec2 off, bool is_bounded)
{
    float C_FRAME_X_MAX = 1150;
    float C_FRAME_X_MIN = 50;
    float C_FRAME_Y_MAX = 720;
    float C_FRAME_Y_MIN = 50;

    pos.x += off.x;
    pos.y += off.y;

    if (is_bounded) {
        if (pos.x > C_FRAME_X_MAX) {
            pos.x = C_FRAME_X_MAX;
        } else if (pos.x < C_FRAME_X_MIN) {
            pos.x = C_FRAME_X_MIN;
        }
        if (pos.y > C_FRAME_Y_MAX) {
            pos.y = C_FRAME_Y_MAX;
        } else if (pos.y < C_FRAME_Y_MIN) {
            pos.y = C_FRAME_Y_MIN;
        }
    }
}

void PhysicsSystem::set_shield_scale_multiplier(entt::registry& registry, float x, float y)
{
    auto view = registry.view<shieldComponent, physicsScaleComponent>();
    for (auto entity : view) {
        auto& scale = view.get<physicsScaleComponent>(entity).scale;
        scale = { scale.x * x, scale.y * y };
    }
}

void PhysicsSystem::consume_potion(entt::registry& registry, int m_id)
{
    auto view = registry.view<potionComponent, physicsScaleComponent>();
    for (auto entity : view) {
        auto& id = view.get<potionComponent>(entity).id;
        auto& is_consumed = view.get<potionComponent>(entity).is_consumed;
        auto& scale = view.get<physicsScaleComponent>(entity).scale;
        if (id == m_id) {
            scale = { 0.f, 0.f };
            is_consumed = true;
        }
    }
}

void PhysicsSystem::set_character_unmovable(entt::registry& registry)
{
    auto view = registry.view<characterComponent>();
    for (auto entity : view) {
        auto& is_movable = view.get(entity).is_movable;
        is_movable = false;
    }
}

void PhysicsSystem::reset_character(entt::registry& registry)
{
    auto viewCharacter = registry.view<characterComponent, motionComponent, physicsScaleComponent>();
    for (auto entity : viewCharacter) {
        auto& [is_movable, is_dashable, cooldown] = viewCharacter.get<characterComponent>(entity);
        is_movable = true;
        is_dashable = false;
        cooldown = 0.f;

        auto& position
            = viewCharacter.get<motionComponent>(entity).position;
        position = c_init_pos;

        auto& scale = viewCharacter.get<physicsScaleComponent>(entity).scale;
        scale = c_init_scale;
    }

    auto viewShield = registry.view<shieldComponent, physicsScaleComponent>();
    for (auto entity : viewShield) {
        auto& [is_reflectable, duration, cooldown] = viewShield.get<shieldComponent>(entity);
        is_reflectable = false;
        duration = 0.f;
        cooldown = 0.f;

        auto& scale = viewShield.get<physicsScaleComponent>(entity).scale;
        scale = s_init_scale;
    }
}

void PhysicsSystem::rotate(float& radians, float xpos, float ypos, vec2 position)
{
    vec2 entityVector = { 0.f, 1.f };
    vec2 mouseVec = {
        (float)xpos - position.x,
        (float)ypos - position.y
    };

    int factor = 1;
    if (mouseVec.x > 0.f)
        factor = -1;

    float angle = acos(dot(mouseVec, entityVector) / (lengthVec2(mouseVec) * lengthVec2(entityVector)));
    radians = factor * angle;
}

// Calculates the length of a vec2 vector
float PhysicsSystem::lengthVec2(vec2 v)
{
    return sqrt(pow(v.x, 2.f) + pow(v.y, 2.f));
}

void PhysicsSystem::reflect_projectile(entt::registry& registry, int m_id, vec2 angle)
{
    // Projectile Physics Update
    auto projectile = registry.view<projectileComponent, motionComponent>();

    for (auto entity : projectile) {
        auto& [position, direction, radians, speed] = projectile.get<motionComponent>(entity);
        auto& id = projectile.get<projectileComponent>(entity).id;
        if (id == m_id) {
            direction = angle;
        }
    }
}