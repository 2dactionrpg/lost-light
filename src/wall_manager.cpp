#include "wall_manager.hpp"

bool WallManager::init(std::vector<vec2> coords, vector<Wall> &walls, int lvl)
{
    level = lvl;

    bool retVal = true;
    for (vec2 coord : coords) {
        Wall w;
        float coef = 41.8f;
        retVal &= w.init(coord.x * coef,coord.y * coef);
        walls.emplace_back(w);
    }

    return retVal;
}

void WallManager::update(vector<Wall> &wall, int lvl, std::vector<vec2> orientation) {
    if(level != lvl) {
        level = lvl;
        wall.clear();
        init(orientation, wall, level);
    }
}

void WallManager::destroy(vector<Wall> &wall) {
    for ( auto w : wall) {
        w.destroy();
    }
    wall.clear();
}

void WallManager::draw(vector<Wall> &wall, mat3 projectile2D) {
    for ( auto w : wall ) {
        w.draw(projectile2D);
    }
}