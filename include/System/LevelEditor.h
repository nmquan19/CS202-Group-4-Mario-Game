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

class LevelEditor {
public:
    LevelEditor(const LevelEditor&) = delete;
    LevelEditor& operator=(const LevelEditor&) = delete;

    static LevelEditor& getInstance();
    static void cleanup();

    ~LevelEditor();

    void update();
    void draw();
    void handleMouseInput();
    void placeObject(ObjectType type, Vector2 gridCoord);
    void removeObject(Vector2 gridCoord);
    void toggleEditMode();
    bool isInEditMode() const;

    void saveLevel(const std::string& filename);
    void loadLevel(const std::string& filename);
    void clearLevel();
private:
    static LevelEditor* instance;
    std::map<std::pair<int, int>, std::stack<std::unique_ptr<Object>>> gridBlocks;
	ObjectPalette palette;
    bool editMode = true;
    bool clearingLevel = false;
    bool loadingLevel = false;
    LevelEditor() = default;
    std::string pendingLoadFile = "";
    std::string objectTypeToString(const ObjectType& type);
    ObjectType stringToObjectType(const std::string& typeStr);
    void performLoad(const std::string& filename);
};