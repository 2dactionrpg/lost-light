#pragma once

#include "entity.hpp"
#include "projectile.hpp"
#include <vector>

class Projectile;

class Enemy : public Entity {
    static Texture enemy_texture;

public:
    // Creates all the associated render resources and default transform
    bool init(int id);

    // Releases all associated resources
    void destroy();

    // Update enemy position based on direction
    // ms represents the number of milliseconds elapsed from the previous update() call
    void update(float ms);

    // Renders the enemy
    void draw(const mat3& projection) override;

    // Returns the current enemy position
    vec2 get_position() const;

    void set_position(vec2 pos);

    // Moves the enemy's position by the specified offset
    void move(vec2 off);

    bool collides_with(const Projectile& Projectile);

    void set_target(vec2 character_pos);

    void set_scale(vec2 scale);

    // Set enemy rotation in radians
    void set_rotation(float radians);

    vec2 get_bounding_box() const;

    vec2 target;

    vec2 get_face_position();

    int get_id();

private:
    int enemy_id;

    std::vector<Vertex> m_vertices;
    std::vector<uint16_t> m_indices;
};
