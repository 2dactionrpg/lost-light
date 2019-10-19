#pragma once

#include <entt/entity/registry.hpp>
#include <cmath>
#include "../common.hpp"
#include "../character.hpp"
#include "../shield.hpp"
#include "../enemy.hpp"
#include "../projectile.hpp"
#include "../components/inputMouse.hpp"
#include "../components/inputKeyboard.hpp"
#include "../components/motionComponent.hpp"
#include "../components/physicsScaleComponent.hpp"
#include "../components/characterComponent.hpp"
#include "../components/shieldComponent.hpp"
#include "../components/enemyComponent.hpp"
#include "../components/projectileComponent.hpp"


using namespace std;

class PhysicsSystem
{
private:
public:
    void update(entt::registry &registry, Character &m_character, Shield &m_shield, vector<Enemy> &m_enemies, vector<Projectile> &m_projectiles);
    void sync(entt::registry &registry, float elapsed_ms);
    void move(vec2 &pos, vec2 off, bool is_bounded);
    void rotate(float &radians, float newRadians);
    float lengthVec2(vec2 v);
    void setShieldScaleMultiplier(entt::registry &registry, float x, float y);
    void setCharacterUnmovable(entt::registry &registry);
    void resetCharacter(entt::registry &registry);
    void reflect_projectile(entt::registry &registry, int m_id, vec2 angle);
};
