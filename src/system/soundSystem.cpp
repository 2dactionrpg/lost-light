#include "soundSystem.hpp"

bool SoundSystem::init()
{
    //-------------------------------------------------------------------------
    // Loading music and sounds
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "Failed to initialize SDL Audio");
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
        fprintf(stderr, "Failed to open audio device");
        return false;
    }

    m_background_music = Mix_LoadMUS(audio_path("music.wav"));
    m_character_dead_sound = Mix_LoadWAV(audio_path("character_dead.wav"));
    m_character_reflect_sound = Mix_LoadWAV(audio_path("character_reflect.wav"));

    if (m_background_music == nullptr || m_character_dead_sound == nullptr || m_character_reflect_sound == nullptr) {
        return false;
    }

    return true;
}

void SoundSystem::play_background_music()
{
    Mix_PlayMusic(m_background_music, -1);
}

void SoundSystem::play_sound(int type, int channel, int loops)
{
    switch (type) {
    case 0:
        Mix_PlayChannel(channel, m_character_dead_sound, loops);
        break;
    case 1:
        Mix_PlayChannel(channel, m_character_reflect_sound, loops);
        break;
    }
}

void SoundSystem::destroy()
{
    if (m_background_music != nullptr)
        Mix_FreeMusic(m_background_music);
    if (m_character_dead_sound != nullptr)
        Mix_FreeChunk(m_character_dead_sound);
    if (m_character_reflect_sound != nullptr)
        Mix_FreeChunk(m_character_reflect_sound);

    Mix_CloseAudio();
}