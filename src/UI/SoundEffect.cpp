#include "../../include/UI/SoundEffect.h"
#include <string>
#include <raylib.h>

AudioManager::AudioManager() {
    isMusicPlaying = false;
    
}
AudioManager& AudioManager::getInstance() {
    static AudioManager instance;
    return instance;
}
AudioManager::~AudioManager() {
    for (auto& sound : sounds) {
        UnloadSound(sound.second);
    }
    //UnloadMusicStream(backgroundMusic);
    
}

void AudioManager::LoadSoundEffect(const std::string& key, const std::string& filePath) {
    Sound sound = LoadSound(filePath.c_str());
    sounds[key] = sound;
}

void AudioManager::PlaySoundEffect(const std::string& key) {
    auto it = sounds.find(key);
    if (it != sounds.end()) {
        PlaySound(it->second);
    }
}
/*
void AudioManager::LoadBackgroundMusic(const std::string& filePath) {
    backgroundMusic = LoadMusicStream(filePath.c_str());
}

void AudioManager::PlayBackgroundMusic() {
    if (!isMusicPlaying) {
        PlayMusicStream(backgroundMusic);
        isMusicPlaying = true;
    }
}

void AudioManager::StopBackgroundMusic() {
    if (isMusicPlaying) {
        StopMusicStream(backgroundMusic);
        isMusicPlaying = false;
    }
}

void AudioManager::UpdateBackgroundMusic() {
    if (isMusicPlaying) {
        UpdateMusicStream(backgroundMusic);
    }
}
*/