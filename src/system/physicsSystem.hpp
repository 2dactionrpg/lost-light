#pragma once

#include "../character.hpp"
#include "../common.hpp"
#include "../components/characterComponent.hpp"
#include "../components/enemyComponent.hpp"
#include "../components/inputKeyboard.hpp"
#include "../components/inputMouse.hpp"
#include "../components/motionComponent.hpp"
#include "../components/physicsScaleComponent.hpp"
#include "../components/potionComponent.hpp"
#include "../components/projectileComponent.hpp"
#include "../components/shieldComponent.hpp"
#include "../data.hpp"
#include "../enemy.hpp"
#include "../potion.hpp"
#include "../projectile.hpp"
#include "../shield.hpp"
#include "soundSystem.hpp"
#include <cmath>
#include <entt/entity/registry.hpp>

using namespace std;

class PhysicsSystem {
private:
    SoundSystem soundSystem;

public:
    void update(entt::registry& registry, Character& m_character, Shield& m_shield, vector<Enemy>& m_enemies, vector<Projectile>& m_projectiles, Potion& m_potion);
    void sync(entt::registry& registry, float elapsed_ms);
    void move(vec2& pos, vec2 off, bool is_bounded);
    void rotate(float& radians, float xpos, float ypos, vec2 position);
    float lengthVec2(vec2 v);
    void set_shield_scale_multiplier(entt::registry& registry, float x, float y);
    void set_character_unmovable(entt::registry& registry);
    void reset_character(entt::registry& registry);
    void reflect_projectile(entt::registry& registry, int m_id, vec2 angle);
    void consume_potion(entt::registry& registry, int m_id);
};