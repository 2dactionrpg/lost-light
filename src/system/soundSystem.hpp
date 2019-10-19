#pragma once

#include "../common.hpp"

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_mixer.h>

using namespace std;

enum soundType {
    C_DEAD,
    C_REFLECT,
};

class SoundSystem {
private:
    inline static Mix_Music* m_background_music;
    inline static Mix_Chunk* m_character_dead_sound;
    inline static Mix_Chunk* m_character_reflect_sound;

public:
    bool init();
    void destroy();
    void play_background_music();
    void play_sound(int type, int channel = -1, int loops = 0);
};
