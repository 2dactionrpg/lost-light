#include "overlaySystem.hpp"

void OverlaySystem::update(entt::registry& registry, Overlay& m_overlay, vec2 light_source)
{
    auto overlay = registry.view<overlayComponent>();
    for (auto entity : overlay) {
        auto& ls = overlay.get(entity).light_source;
        ls = light_source;
    }
}
