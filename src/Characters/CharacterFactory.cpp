#include "../../include/Characters/CharacterFactory.h"

std::unique_ptr<Character> CharacterFactory::createCharacter(CharacterType type, Vector2 startPosition, float scale) {
    switch(type) {
        case CharacterType::MARIO:
            return createMario(startPosition, scale);
        case CharacterType::LUIGI:
            return createLuigi(startPosition, scale);
    }
}

std::unique_ptr<Character> CharacterFactory::createMario(Vector2 startPosition, float scale) {
    CharacterStats stats = getMarioStats();
    std::vector<std::vector<Rectangle>> stateFrameData = getMarioFrameData();
    const char* spritePath = getMarioSpritePath();

    std::unique_ptr<Character> mario = std::make_unique<Character>(startPosition, stats, stateFrameData, CharacterType::MARIO, scale);
    return mario;
}

std::unique_ptr<Character> CharacterFactory::createLuigi(Vector2 startPosition, float scale) {
    CharacterStats stats = getLuigiStats();
    std::vector<std::vector<Rectangle>> stateFrameData = getLuigiFrameData();
    const char* spritePath = getLuigiSpritePath();

    std::unique_ptr<Character> luigi = std::make_unique<Character>(startPosition, stats, stateFrameData, CharacterType::LUIGI, scale);
    return luigi;
}

CharacterStats CharacterFactory::getMarioStats() {
    return {220.0f, 550.0f, 980.0f};
}

CharacterStats CharacterFactory::getLuigiStats() {
    return {175.0f, 730.0f, 980.0f};
}

const char* CharacterFactory::getMarioSpritePath() {
    return "assets/mario_sprites.png";
}

const char* CharacterFactory::getLuigiSpritePath() {
    return "assets/luigi_sprites.png";
}

std::vector<std::vector<Rectangle>> CharacterFactory::getMarioFrameData() {
    return {
        {{31, 21, 17, 43}, {111, 21, 17, 43}, {191, 20, 17, 44}, {271, 20, 17, 44}, {351, 20, 17, 44}}, // idle state
        {{31, 85, 19, 43}, {112, 85, 16, 43}, {193, 84, 15, 44}, {272, 84, 16, 44}, {351, 85, 18, 43}, {432, 85, 16, 43}, {512, 84, 16, 44}, {592, 84, 16, 44}}, // moving state
        {{30, 209, 19, 46}, {110, 209, 19, 46}, {190, 209, 19, 46}, {270, 209, 19, 46}} // jumping state
    };
}

std::vector<std::vector<Rectangle>> CharacterFactory::getLuigiFrameData() {
    return {
        {{31, 21, 17, 43}, {111, 21, 17, 43}, {191, 20, 17, 44}, {271, 20, 17, 44}, {351, 20, 17, 44}}, // idle state
        {{31, 85, 19, 43}, {112, 85, 16, 43}, {193, 84, 15, 44}, {272, 84, 16, 44}, {351, 85, 18, 43}, {432, 85, 16, 43}, {512, 84, 16, 44}, {592, 84, 16, 44}}, // moving state
        {{30, 209, 19, 46}, {110, 209, 19, 46}, {190, 209, 19, 46}, {270, 209, 19, 46}} // jumping state
    };
}