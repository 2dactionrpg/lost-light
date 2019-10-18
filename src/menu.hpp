#pragma once

#include "entity.hpp"

class Menu : public Entity {
    // Shared between all menus, no need to load one for each instance
    static Texture menu_texture;

public:
    // Creates all the associated render resources and default transform
    bool init();

    // Releases all the associated resources
    void destroy();

    // Update menu due to current
    // ms represents the number of milliseconds elapsed from the previous update() call
    void update(float ms, int state);

    // Renders the menu
    // projection is the 2D orthographic projection matrix
    void draw(const mat3& projection) override;

    vec2 getDirection();

    void setDirection(vec2 direction);

    void setRotation(float rad);

    // Returns the current menu position
    vec2 get_position() const;

    // Sets the new menu position
    void set_position(vec2 position);

    // Returns the menu' bounding box for collision detection, called by collides_with()
    vec2 get_bounding_box() const;

    bool is_alive() const;

    bool m_is_alive;

    int oldState;
};