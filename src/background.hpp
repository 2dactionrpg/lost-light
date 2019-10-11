#pragma once

// #include "common.hpp"
#include "entity.hpp"

class Background : public Entity {
public:
    // Creates all the associated render resources and default transform
    bool init();

    // Releases all associated resources
    void destroy();

    // Renders the background
    void draw(const mat3& projection) override;

    // Character dead time getters and setters
    void set_character_dead();
    void reset_character_dead_time();
    float get_character_dead_time() const;

private:
    // When character is alive, the time is set to -1
    float m_dead_time;
};
