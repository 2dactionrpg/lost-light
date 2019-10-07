#include <entt/entity/registry.hpp>
#include "./components/characterComponent.hpp"
#include "./components/shieldComponent.hpp"
#include "./components/motionComponent.hpp"
#include "./components/inputKeyboard.hpp"
#include "./components/inputMouse.hpp"

entt::entity makeCharacter(entt::registry &registry);
entt::entity makeShield(entt::registry &registry);