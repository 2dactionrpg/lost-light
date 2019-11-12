#pragma once

#include "../character.hpp"
#include "../common.hpp"
#include "../components/characterComponent.hpp"
#include "../components/enemyComponent.hpp"
#include "../components/groundComponent.hpp"
#include "../components/inputKeyboard.hpp"
#include "../components/inputMouse.hpp"
#include "../components/levelComponent.hpp"
#include "../components/motionComponent.hpp"
#include "../components/physicsScaleComponent.hpp"
#include "../components/potionComponent.hpp"
#include "../components/projectileComponent.hpp"
#include "../components/shieldComponent.hpp"
#include "../data.hpp"
#include "../enemy.hpp"
#include "../ground.hpp"
#include "../potion.hpp"
#include "../projectile.hpp"
#include "../shield.hpp"
#include "../system/menuSystem.hpp"
#include "../wall.hpp"
//#include "collisionSystem.hpp"
#include "soundSystem.hpp"
#include <cmath>
#include <entt/entity/registry.hpp>

using namespace std;

class PhysicsSystem {
private:
    SoundSystem soundSystem;
public:
    void update(entt::registry& registry, Character& m_character, Shield& m_shield, vector<Enemy>& m_enemies, vector<Projectile>& m_projectiles, Potion& m_potion, Ground& m_ground);
    void sync(entt::registry& registry, float elapsed_ms, vector<Wall>& walls);
    void move(vec2& pos, vec2 off, bool is_bounded);
    void rotate(float& radians, float xpos, float ypos, vec2 position);
    float lengthVec2(vec2 v);
    void set_shield_scale_multiplier(entt::registry& registry, float x, float y);
    void set_character_unmovable(entt::registry& registry);
    void reset_character(entt::registry& registry);
    void reset_enemy(entt::registry& registry);
    void reset_potion(entt::registry& registry);
    void reflect_projectile(entt::registry& registry, int m_id, vec2 angle);
    void consume_potion(entt::registry& registry, int m_id);
    vec2 getOffset(bool isBoss, vec2 position, vec2 offset, vector<Wall> &walls);

};
