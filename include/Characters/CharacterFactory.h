#pragma once
#include "Character.h"
#include <memory>

class CharacterFactory {
public:
    static std::unique_ptr<Character> createCharacter(CharacterType type, Vector2 startPosition, float scale = 2.0f);

private:
    static std::unique_ptr<Character> createMario(Vector2 startPosition, float scale);
    static std::unique_ptr<Character> createLuigi(Vector2 startPosition, float scale);

    static CharacterStats getMarioStats();
    static CharacterStats getLuigiStats();

    static const char* getMarioSpritePath();
    static const char* getLuigiSpritePath();

    static std::vector<std::vector<Rectangle>> getMarioFrameData();
    static std::vector<std::vector<Rectangle>> getLuigiFrameData();
};