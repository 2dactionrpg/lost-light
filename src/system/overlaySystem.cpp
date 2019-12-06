#include "overlaySystem.hpp"

void OverlaySystem::update(entt::registry& registry, Overlay& m_overlay)
{
    auto overlay = registry.view<overlayComponent>();
    // auto position = registry.view<motionComponent>();

    // for (auto entity : overlay) {
    //     auto& state = overlay.get(entity).state;
    //     auto& old_state = overlay.get(entity).old_state;
    //     if (state != old_state) {
    //         if (state == STATE_PAUSE) {
    //             soundSystem.play_sound(OVERLAY_POPUP);
    //         }
    //         if (old_state == STATE_PAUSE && state == STATE_PLAYING) {
    //             soundSystem.play_sound(OVERLAY_CLOSE);
    //         }
    //         m_overlay.load_texture(state);
    //         old_state = state;
    //     }
    // }
}

// void OverlaySystem::sync(entt::registry& registry, int m_state)
// {
//     auto overlay = registry.view<overlayComponent>();

//     for (auto entity : overlay) {
//         auto& state = overlay.get(entity).state;
//         auto& old_state = overlay.get(entity).old_state;
//         if (state != m_state) {
//             state = m_state;
//         }
//     }
// }

// int OverlaySystem::get_state(entt::registry& registry)
// {
//     auto overlay = registry.view<overlayComponent>();

//     int m_state = -1;
//     for (auto entity : overlay) {
//         auto& state = overlay.get(entity).state;
//         m_state = state;
//     }
//     return m_state;
// }

// bool OverlaySystem::get_debug_mode(entt::registry& registry)
// {
//     auto overlay = registry.view<overlayComponent>();

//     bool m_debug = false;
//     for (auto entity : overlay) {
//         auto& debug = overlay.get(entity).debug;
//         m_debug = debug;
//     }
//     return m_debug;
// }
