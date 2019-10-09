#pragma once

#include "common.hpp"
#include "entity.hpp"

class Water : public Entity {
public:
    // Creates all the associated render resources and default transform
    bool init();

    // Releases all associated resources
    void destroy();

    // Renders the water
    void draw(const mat3& projection) override;

    // Salmon dead time getters and setters
    void set_salmon_dead();
    void reset_salmon_dead_time();
    float get_salmon_dead_time() const;

private:
    // When salmon is alive, the time is set to -1
    float m_dead_time;
};
