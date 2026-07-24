#include "include/sound.h"

SoundManager::SoundManager()
    : currentMusic{}, musicVolume(1.0f), sfxVolume(1.0f), musicPlaying(false)
{
}

SoundManager::~SoundManager()
{
    if (musicPlaying) StopMusic();
    CloseAudioDevice();
}

void SoundManager::Init()
{
    InitAudioDevice();
}

void SoundManager::PlayMusic(const char* filepath)
{
    if (musicPlaying) StopMusic();
    currentMusic = LoadMusicStream(filepath);
    PlayMusicStream(currentMusic);
    ::SetMusicVolume(currentMusic, musicVolume);
    musicPlaying = true;
}

void SoundManager::StopMusic()
{
    if (musicPlaying)
    {
        StopMusicStream(currentMusic);
        UnloadMusicStream(currentMusic);
        musicPlaying = false;
    }
}

void SoundManager::PlaySoundEffect(const char* filepath)
{
    Sound sfx = LoadSound(filepath);
    PlaySound(sfx);
}

void SoundManager::SetMusicVolume(float volume)
{
    musicVolume = volume;
    if (musicPlaying) ::SetMusicVolume(currentMusic, volume);
}

void SoundManager::SetSFXVolume(float volume)
{
    sfxVolume = volume;
}
