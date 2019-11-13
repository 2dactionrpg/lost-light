#pragma once

#include "entity.hpp"
#include "projectile.hpp"
#include <vector>
#include "triangle.hpp"
#include "path.hpp"
#include "data.hpp"

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
    void draw(const mat3& projection, bool debug) override;

    // Returns the current enemy position
    vec2 get_position() const;

    void set_position(vec2 pos);

    // Moves the enemy's position by the specified offset
    void move(vec2 off);

    bool collides_with(const Projectile& Projectile);

    bool collides_with_point(Dot d);

    void set_scale(vec2 scale);

    // Set enemy rotation in radians
    void set_rotation(float radians);

    vec2 get_bounding_box() const;

    vec2 get_face_position();

    int get_id();

    void update_triangle();

    bool on_sight(vec2 target);

    bool set_line(vec2 target, std::vector<Enemy>& m_enemies);
    void unset_line();

    void alert();
    void search();
    void idle();

private:
    int enemy_id;
    bool has_path;

    Triangle trig;
    Path path;

    std::vector<Vertex> m_vertices;
    std::vector<uint16_t> m_indices;
};
