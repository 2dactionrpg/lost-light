#pragma once

#include "entity.hpp"
#include "projectile.hpp"

class Wall : public Entity {
    // Shared between all walls, no need to load one for each instance
    static Texture wall_texture;

public:
    // Creates all the associated render resources and default transform
    bool init(float xPos, float yPosl);

    // Releases all the associated resources
    void destroy();

    // Update wall due to current
    // ms represents the number of milliseconds elapsed from the previous update() call
    void update(float ms);

    // Renders the wall
    // projection is the 2D orthographic projection matrix
    void draw(const mat3& projection) override;

    vec2 getDirection();

    void setDirection(vec2 direction);

    void setRotation(float rad);

    // Returns the current wall position
    vec2 get_position() const;

    // Sets the new wall position
    void set_position(vec2 position);

    // Returns the wall's bounding box for collision detection, called by collides_with()
    vec2 get_bounding_box() const;

    bool is_alive() const;

    bool m_is_alive;

    bool collides_with(const Projectile& projectile);

    void wall_offset(bool isBoss, vec2 position, vec2 offset, bool &right_moveable, bool &left_moveable, bool &up_moveable, bool &down_moveable);
};
