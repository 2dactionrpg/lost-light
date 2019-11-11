#pragma once

#include "common.hpp"
#include <vector>

#include "../character.hpp"
#include "../enemy.hpp"
#include "../potion.hpp"
#include "../projectile.hpp"
#include "../shield.hpp"
#include "../wall.hpp"
#include "enemyAISystem.hpp"
#include "healthSystem.hpp"
#include "inputSystem.hpp"
#include "menuSystem.hpp"
#include "physicsSystem.hpp"
#include "soundSystem.hpp"

using namespace std;

class CollisionSystem {
private:
    PhysicsSystem physicsSystem;
    EnemyAISystem enemyAI;
    HealthSystem healthSystem;
    MenuSystem menuSystem;
    SoundSystem soundSystem;

public:
    // void update(entt::registry& registry, Menu& m_menu);
    void update(entt::registry& registry, Character& m_character, Shield& m_shield, vector<Enemy>& m_enemies, vector<Projectile>& m_projectiles, Potion& m_potion, unsigned int& m_points, vector<Wall>& m_walls);
};
