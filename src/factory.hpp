#include "./components/characterComponent.hpp"
#include "./components/enemyComponent.hpp"
#include "./components/inputKeyboard.hpp"
#include "./components/inputMouse.hpp"
#include "./components/motionComponent.hpp"
#include "./components/physicsScaleComponent.hpp"
#include "./components/shieldComponent.hpp"
#include <entt/entity/registry.hpp>

entt::entity makeCharacter(entt::registry& registry);
entt::entity makeShield(entt::registry& registry);
entt::entity makeEnemy(entt::registry& registry, int id);
entt::entity makeBoss(entt::registry& registry, int id);
