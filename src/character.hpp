#pragma once

#include "entity.hpp"
#include "potion.hpp"
#include "zombie.hpp"
#include "projectile.hpp"
#include <entt/entity/registry.hpp>
#include <vector>

class Projectile;

class Character : public Entity
{
    static Texture character_texture;

public:
    // Creates all the associated render resources and default transform
    bool init();

    // Releases all associated resources
    void destroy();

    // Renders the character
    void draw(const mat3 &projection) override;

    // Returns the current character position
    vec2 get_position() const;

    float get_rotation() const;

    vec2 get_scale() const;

    bool collides_with(const Projectile &Projectile);

    bool collides_with(const Potion &potion);

    bool collides_with(const Zombie &zombie);

    // Set character rotation in radians
    void set_rotation(float radians);

    void set_position(vec2 position);

    void set_scale(vec2 scale);

    void set_distortion(vec2 distortion);

    void set_sheer(float sheer);

    vec2 get_bounding_box() const;

    void take_damage();

    int get_health() const;

    void restart_health();

private:
    float ms_cant_take_damage_cooldown;
    int m_health;
    std::vector<Vertex> m_vertices;
    std::vector<uint16_t> m_indices;
    entt::registry registry;
};
