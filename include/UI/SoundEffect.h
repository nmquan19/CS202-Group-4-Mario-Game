#pragma once

#include "raylib.h"
#include <string>
#include <unordered_map>
#include <iostream>
using namespace std;
class AudioManager {
private:
    std::unordered_map<std::string, Sound> sounds;
    std::unordered_map<std::string, Music> backgroundMusics;
    std::unordered_map<std::string, bool> isMusicPlaying;

    AudioManager();

    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

public:
    static AudioManager& getInstance();
    ~AudioManager();

    void LoadSoundEffect(const std::string& key, const std::string& filePath);
    void PlaySoundEffect(const std::string& key);
    void SetSoundEffectVolume(float volumePercent);

    void LoadBackgroundMusic(const std::string& key, const std::string& filePath);
    void PlayBackgroundMusic(const std::string& key);
    void StopBackgroundMusic(const std::string& key);
    void UpdateBackgroundMusic(const std::string& key);
    void SetBackgroundMusicVolume(float volumePercent);

    bool isPlaying();
    void StopAllBackgroundMusic();
};