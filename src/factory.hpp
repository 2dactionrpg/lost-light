#pragma once

#include "./components/characterComponent.hpp"
#include "./components/enemyComponent.hpp"
#include "./components/groundComponent.hpp"
#include "./components/inputKeyboard.hpp"
#include "./components/inputMouse.hpp"
#include "./components/levelComponent.hpp"
#include "./components/menuComponent.hpp"
#include "./components/motionComponent.hpp"
#include "./components/physicsScaleComponent.hpp"
#include "./components/potionComponent.hpp"
#include "./components/projectileComponent.hpp"
#include "./components/shieldComponent.hpp"
#include "data.hpp"
#include <entt/entity/registry.hpp>

entt::entity makeCharacter(entt::registry& registry);
entt::entity makeShield(entt::registry& registry);
entt::entity makeMinion(entt::registry& registry, int id, vec2 pos, bool is_movable = false, bool alert = false);
entt::entity makeBoss(entt::registry& registry, int id, vec2 pos, bool is_movable = false);
entt::entity makePotion(entt::registry& registry, int id);
entt::entity makeGround(entt::registry& registry, int id);
entt::entity makeProjectile(entt::registry& registry, int id, vec2 pos, vec2 dir, float rad);
entt::entity makeMenu(entt::registry& registry);
entt::entity makeLevel(entt::registry& registry);