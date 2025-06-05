#pragma once

#include "raylib.h"
#include <string>
#include <unordered_map>

class AudioManager {
private:
    std::unordered_map<std::string, Sound> sounds;
    //Music backgroundMusic;
    bool isMusicPlaying = false;

public:

    AudioManager();
    ~AudioManager();

    void LoadSoundEffect(const std::string& key, const std::string& filePath);
    void PlaySoundEffect(const std::string& key);
    /*
    void LoadBackgroundMusic(const std::string& filePath);
    void PlayBackgroundMusic();
    void StopBackgroundMusic();
    void UpdateBackgroundMusic();
    */
};
