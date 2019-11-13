#pragma once

#include <vector>

#include "common.hpp"
#include "entity.hpp"

//
//
//

struct Dot {
    bool alive;
    vec2 position;
    float radius;
};

class Path : public Entity
{
    public:
        bool init(int id);
        void destroy();
        std::vector<Dot> form(vec2 start,vec2 self_box, vec2 end);
        void draw(const mat3& projection) override;
        void spawn_dot(vec2 position);
        void blocked();

    private:
        int path_id;
        int steps;
        float in_color[3];
        bool is_blocked;
        GLuint m_instance_vbo;
        std::vector<Dot> m_dots;
};