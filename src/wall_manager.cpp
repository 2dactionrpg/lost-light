#include "wall_manager.hpp"

bool WallManager::init(std::vector<vec2> coords, vector<Wall> &walls, int lvl)
{
    level = lvl;
    bool retVal = true;
    for (vec2 coord : coords) {

        Wall w;
        // TODO: findout the correct coeffitient 
        float coef = 45.f;
        retVal &= w.init(coord.x * coef,coord.y * coef);
        walls.emplace_back(w);
    }

    return retVal;
    /*float wall_length = 85.f;
    bool retVal = true;
    switch (level) {
        case 1:
            float init_pos_x = 450.f;
            float init_pos_y = 250.f;
            // L 
            Wall w1;
            retVal &= w1.init(init_pos_x,init_pos_y,true);
            wall.emplace_back(w1);
            init_pos_y += wall_length;
            Wall w2;
            retVal &= w2.init(init_pos_x,init_pos_y,true);
            wall.emplace_back(w2);
            init_pos_y += wall_length;
            Wall w3;
            retVal &= w3.init(init_pos_x,init_pos_y,true);
            wall.emplace_back(w3);
            init_pos_y += wall_length-20.f;
            init_pos_x += 22.f;
            Wall w4;
            retVal &= w4.init(init_pos_x,init_pos_y,false);
            wall.emplace_back(w4);
            init_pos_x += 30.f;
            Wall w5;
            retVal &= w5.init(init_pos_x,init_pos_y,false);
            wall.emplace_back(w5);

            // 1
            init_pos_x = 680.f;
            init_pos_y = 250.f;

            Wall w6;
            retVal &= w6.init(init_pos_x,init_pos_y,true);
            wall.emplace_back(w6);
            init_pos_y += wall_length-30;
            Wall w7;
            retVal &= w7.init(init_pos_x,init_pos_y,true);
            wall.emplace_back(w7);
            init_pos_y += wall_length+20;
            Wall w8;
            retVal &= w8.init(init_pos_x,init_pos_y,true);
            wall.emplace_back(w8);
            init_pos_y += wall_length-30;
            Wall w9;
            retVal &= w9.init(init_pos_x,init_pos_y,true);
            wall.emplace_back(w9);
            break;
    }
    
    return retVal;*/
}

void WallManager::update(vector<Wall> &wall, int lvl, std::vector<vec2> orientation) {
    if(level != lvl) {
        level = lvl;
        destroy(wall);
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