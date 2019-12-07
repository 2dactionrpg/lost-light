#pragma once

#include "../character.hpp"
#include "../common.hpp"
#include "../components/characterComponent.hpp"
#include "../components/enemyComponent.hpp"
#include "../components/inputKeyboard.hpp"
#include "../components/inputMouse.hpp"
#include "../components/motionComponent.hpp"
#include "../components/physicsScaleComponent.hpp"
#include "../components/projectileComponent.hpp"
#include "../components/shieldComponent.hpp"
#include "../enemy.hpp"
#include "../zombie.hpp"
#include "../projectile.hpp"
#include "../shield.hpp"
#include "levelSystem.hpp"
#include "soundSystem.hpp"
#include <cmath>
#include <entt/entity/registry.hpp>

using namespace std;

class HealthSystem
{
private:
    LevelSystem levelSystem;
    SoundSystem soundSystem;

public:
    void update(entt::registry &registry, vector<Enemy> &m_enemies, vector<Zombie> &m_zombies);
    void damage(entt::registry &registry, int m_id);
};
