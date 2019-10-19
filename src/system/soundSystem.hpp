#pragma once

#include "../common.hpp"
#include "../data.hpp"

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_mixer.h>

using namespace std;

class SoundSystem {
private:
    inline static Mix_Music* m_background_music;
    inline static Mix_Chunk* m_character_dead_sound;
    inline static Mix_Chunk* m_shield_reflect_sound;
    inline static Mix_Chunk* m_menu_popup_sound;
    inline static Mix_Chunk* m_menu_close_sound;

public:
    bool init();
    void destroy();
    void play_background_music();
    void play_sound(int type, int channel = -1, int loops = 0);
};
