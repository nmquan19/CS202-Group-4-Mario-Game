#pragma once

#include <memory>
#include <map>
#include <utility>
#include <raylib.h>
#include <string>
#include <stack>
#include "../System/TextureManager.h"
#include "../Objects/ObjectFactory.h"
#include "Interface.h"
#include "json.hpp"

using json = nlohmann::json;

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
    ObjectPalette getObjectPalette() {
        return palette;
    }
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