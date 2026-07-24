#pragma once

#include "raylib.h"

class SoundManager {
public:
    SoundManager();
    ~SoundManager();

    void Init();
    void PlayMusic(const char* filepath);
    void StopMusic();
    void PlaySoundEffect(const char* filepath);
    void SetMusicVolume(float volume);
    void SetSFXVolume(float volume);

private:
    Music currentMusic;
    float musicVolume;
    float sfxVolume;
    bool musicPlaying;
};
