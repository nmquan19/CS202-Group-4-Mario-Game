#include "../../include/UI/SoundEffect.h"
#include <string>
#include <raylib.h>

AudioManager::AudioManager() {
    LoadSoundEffect("click", "./assets/sound/click.wav");
    LoadSoundEffect("gain2", "./assets/sound/gain2.wav");
    LoadSoundEffect("jump", "./assets/sound/jump.wav");
    LoadSoundEffect("jump2", "./assets/sound/jump2.wav");
    LoadSoundEffect("level up", "./assets/sound/level up.wav");
    LoadSoundEffect("movement1", "./assets/sound/movement1.wav");
    LoadSoundEffect("movement2", "./assets/sound/movement2.wav");
    LoadSoundEffect("next1", "./assets/sound/next1.wav");
    LoadSoundEffect("next2", "./assets/sound/next2.wav");
    LoadSoundEffect("next3", "./assets/sound/next3.wav");
    LoadSoundEffect("notice1", "./assets/sound/notice1.wav");
    LoadSoundEffect("notice2", "./assets/sound/notice2.wav");
    LoadSoundEffect("point", "./assets/sound/point.wav");
    LoadSoundEffect("strike", "./assets/sound/strike.wav");

    LoadBackgroundMusic("theme1", "./assets/sound/theme1.wav");
}

AudioManager& AudioManager::getInstance() {
    static AudioManager instance;
    return instance;
}

AudioManager::~AudioManager() {
    for (auto& sound : sounds) {
        UnloadSound(sound.second);
    }

    for (auto& music : backgroundMusics) {
        UnloadMusicStream(music.second);
    }
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

void AudioManager::SetSoundEffectVolume(float volumePercent) {
    float volume = volumePercent / 100.0f;
    if (volume < 0.0f) volume = 0.0f;
    if (volume > 1.0f) volume = 1.0f;

    for (auto& sound : sounds) {
        SetSoundVolume(sound.second, volume);
    }
}

void AudioManager::LoadBackgroundMusic(const std::string& key, const std::string& filePath) {
    Music music = LoadMusicStream(filePath.c_str());
    backgroundMusics[key] = music;
    isMusicPlaying[key] = false;
}

void AudioManager::PlayBackgroundMusic(const std::string& key) {
    auto it = backgroundMusics.find(key);

    if (it != backgroundMusics.end()) {
        std::cout << "Found music key: " << key << "\n";

        if (!isMusicPlaying[key]) {
            std::cout << "Playing: " << key << "\n";
            PlayMusicStream(it->second);
            isMusicPlaying[key] = true;
        }
    }
    else {
        std::cout << "Music key not found: " << key << "\n";
    }
}

void AudioManager::StopBackgroundMusic(const std::string& key) {
    auto it = backgroundMusics.find(key);

    if (it != backgroundMusics.end() && isMusicPlaying[key]) {
        StopMusicStream(it->second);
        isMusicPlaying[key] = false;
    }
}

void AudioManager::UpdateBackgroundMusic(const std::string& key) {
    auto it = backgroundMusics.find(key);

    if (it != backgroundMusics.end() && isMusicPlaying[key]) {
        UpdateMusicStream(it->second);
        //std::cout << "Updating " << key << "\n";
    }
}

void AudioManager::SetBackgroundMusicVolume(float volumePercent) {
    float volume = volumePercent / 100.0f;

    if (volume < 0.0f) volume = 0.0f;
    if (volume > 1.0f) volume = 1.0f;
    for (auto& music : backgroundMusics) {
        SetMusicVolume(music.second, volume);
    }
}

bool AudioManager::isPlaying() {
    for (auto& playing : isMusicPlaying) {
        if (playing.second) return true;
    }
    return false;
}

void AudioManager::StopAllBackgroundMusic() {
    for (auto& playing : isMusicPlaying) {
        if (playing.second) {
            StopBackgroundMusic(playing.first);
        }
    }
}