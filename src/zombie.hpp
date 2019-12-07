#pragma once

#include "entity.hpp"
#include <vector>
#include "data.hpp"

#include "projectile.hpp"
#include "path.hpp"
#include "triangle.hpp"

class Projectile;

class Zombie : public Entity
{
    static Texture zombie_texture;

public:
    bool init(int id);
    void destroy();

    void draw(const mat3 &projection) override;
    void draw(const mat3 &projection, bool debug) override;

    int get_id();
    vec2 get_position() const;
    void set_position(vec2 pos);

    bool collides_with(const Projectile &projectile);
    bool collides_with_point(Dot d);

    void set_scale(vec2 scale);
    void set_rotation(float radians);
    vec2 get_bounding_box() const;
    vec2 get_face_position();

    void update_triangle();
    bool on_sight(vec2 target);
    // bool set_line(vec2 target, std::vector<Enemy> &m_enemies);
    // void unset_line()

    void alert();
    void search();
    void idle();

private:
    int zombie_id;
    bool has_path;

    Triangle trig;
    Path path;

    std::vector<Vertex> m_vertices;
    std::vector<uint16_t> m_indices;
};