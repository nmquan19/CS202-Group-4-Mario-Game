#pragma once

#include <memory>
#include <map>
#include <utility>
#include <raylib.h>
#include <string>
#include <stack>
#include "../System/TextureManager.h"
#include "../Objects/ObjectFactory.h"
#include "../UI/UI.h"
#include "Interface.h"
#include "json.hpp"
using json = nlohmann::json;
class Background {
private:
    static Background* instance;
    Texture2D Airship_night_1;
    Texture2D Airship_night_2;
    Texture2D Airship_night_3;
    Texture2D Airship_night_4;
    Texture2D Airship_night_5;
    Texture2D Airship_night_6;
    Texture2D Forest_1;
    Texture2D Forest_2;
    Texture2D Ghost_house_1;
    Texture2D Ghost_house_2;
    Texture2D Ghost_house_3;
    Texture2D Snow_night_1;
    Texture2D Snow_night_2;
    Background() {
        Airship_night_1 = LoadTexture("./assets/Airship_night_1.png");
        Airship_night_2 = LoadTexture("./assets/Airship_night_2.png");
        Airship_night_3 = LoadTexture("./assets/Airship_night_3.png");
        Airship_night_4 = LoadTexture("./assets/Airship_night_4.png");
        Airship_night_5 = LoadTexture("./assets/Airship_night_5.png");
        Airship_night_6 = LoadTexture("./assets/Airship_night_6.png");
        Forest_1 = LoadTexture("./assets/Forest_1.png");
        Forest_2 = LoadTexture("./assets/Forest_2.png");
        Ghost_house_1 = LoadTexture("./assets/Ghost_house_1.png");
        Ghost_house_2 = LoadTexture("./assets/Ghost_house_2.png");
        Ghost_house_3 = LoadTexture("./assets/Ghost_house_3.png");
        Snow_night_1 = LoadTexture("./assets/Snow_night_1.png");
        Snow_night_2 = LoadTexture("./assets/Snow_night_2.png");
    }
public:
    Background(const Background&) = delete;
    Background& operator=(const Background&) = delete;
    static Background& getInstance();

    ~Background() {
        UnloadTexture(Airship_night_1);
        UnloadTexture(Airship_night_2);
        UnloadTexture(Airship_night_3);
        UnloadTexture(Airship_night_4);
        UnloadTexture(Airship_night_5);
        UnloadTexture(Airship_night_6);
        UnloadTexture(Forest_1);
        UnloadTexture(Forest_2);
        UnloadTexture(Ghost_house_1);
        UnloadTexture(Ghost_house_2);
        UnloadTexture(Ghost_house_3);
        UnloadTexture(Snow_night_1);
        UnloadTexture(Snow_night_2);
    }

    void draw(std::string s, Vector2 position) {
        Texture2D background;
        if (s == "Airship_night_1") background = Airship_night_1;
        else if (s == "Airship_night_2") background = Airship_night_2;
        else if (s == "Airship_night_3") background = Airship_night_3;
        else if (s == "Airship_night_4") background = Airship_night_4;
        else if (s == "Airship_night_5") background = Airship_night_5;
        else if (s == "Airship_night_6") background = Airship_night_6;
        else if (s == "Forest_1") background = Forest_1;
        else if (s == "Forest_2") background = Forest_2;
        else if (s == "Ghost_house_1") background = Ghost_house_1;
        else if (s == "Ghost_house_2") background = Ghost_house_2;
        else if (s == "Ghost_house_3") background = Ghost_house_3;
        else if (s == "Snow_night_1") background = Snow_night_1;
        else background = Snow_night_2;
        for (int i = 0; i <= Constants::WORLDBOUNDS_WIDTH / background.width; i++) {
            DrawTextureEx(background, {position.x + i * background.width, position.y}, 0, 2.0f, WHITE);
        }
    }
};
class LevelEditor {
public:
    LevelEditor(const LevelEditor&) = delete;
    LevelEditor& operator=(const LevelEditor&) = delete;

    static LevelEditor& getInstance();
    static void cleanup();

    ~LevelEditor();

    void update(float deltaTime);
    void draw();
    void handleMouseInput();
    void placeObject(ObjectType type, Vector2 gridCoord);
    void removeObject(Vector2 gridCoord, std::shared_ptr<Object>& target);
    void removeObject(Vector2 gridCoord);
    void setEditMode(bool flag);
    bool isInEditMode() const;

    void saveLevel(const std::string& filename);
    void loadLevel(const std::string& filename);
    void clearLevel();
    bool isBlock(std::pair<int, int> coord);
    int mapSelect = 1;
    ObjectPalette getObjectPalette() {
        return palette;
    }

    std::shared_ptr<Object> findQuestionBlock(Vector2 gridPos);
private:
    static LevelEditor* instance;
    std::map<std::pair<int, int>, std::stack<std::shared_ptr<Object>>> gridBlocks;
	ObjectPalette palette;
    bool editMode = true;
    LevelEditor() = default;
    std::string objectTypeToString(const ObjectType& type);
    ObjectType stringToObjectType(const std::string& typeStr);
    bool clearing = false;
};