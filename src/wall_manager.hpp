#pragma once

#include "common.hpp"
#include "wall.hpp"

using namespace std;

class WallManager {
public:
    // Creates all the associated render resources and default transform
    bool init(vector<vec2> coords, vector<Wall> &wall, int lvl);
    void destroy(vector<Wall> &wall);
    void draw(vector<Wall> &wall, mat3 projectile2D);
    void update(vector<Wall> &wall, int lvl , std::vector<vec2> orientation);
private:
    int level;
};