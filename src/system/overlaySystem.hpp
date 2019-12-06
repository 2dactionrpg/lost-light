#pragma once

#include "../common.hpp"
#include "../components/overlayComponent.hpp"
#include "../data.hpp"
#include "../overlay.hpp"
#include "soundSystem.hpp"
#include <entt/entity/registry.hpp>

using namespace std;

class OverlaySystem {
private:
    SoundSystem soundSystem;

public:
    void update(entt::registry& registry, Overlay& m_overlay);
    void sync(entt::registry& registry, int m_state);
    int get_state(entt::registry& registry);
    bool get_debug_mode(entt::registry& registry);
};
