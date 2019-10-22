#pragma once

#include "entity.hpp"

class Potion : public Entity {
    // Shared between all potions, no need to load one for each instance
    static Texture potion_texture;

public:
    // Creates all the associated render resources and default transform
    bool init(int id);

    // Releases all the associated resources
    void destroy();

    // Renders the potion
    // projection is the 2D orthographic projection matrix
    void draw(const mat3& projection) override;

    vec2 get_direction();

    void set_direction(vec2 direction);

    void set_rotation(float rad);

    void set_scale(vec2 scale);

    // Returns the current potion position
    vec2 get_position() const;

    // Sets the new potion position
    void set_position(vec2 position);

    // Returns the potion' bounding box for collision detection, called by collides_with()
    vec2 get_bounding_box() const;

    bool is_consumed();

    void set_is_consumed(bool is_consumed);

    int get_id();

private:
    int potion_id;
    bool m_is_consumed;
};
