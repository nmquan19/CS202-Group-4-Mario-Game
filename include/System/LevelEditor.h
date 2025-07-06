#pragma once

#include <memory>
#include "../System/TextureManager.h"
#include <map>
#include <utility>
#include "../Objects/ObjectFactory.h"
#include "Interface.h"
#include <raylib.h>

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
	void placeEnemy(EnemyType type, Vector2 gridCoord);
	void removeEnemy(Vector2 gridCoord);
    void toggleEditMode();
    bool isInEditMode() const;
private:
    static LevelEditor* instance;
    std::map<std::pair<int, int>, std::unique_ptr<Object>> gridBlocks;
	ObjectPalette palette;
    bool editMode = true;
    LevelEditor() = default;
};