#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <functional>
#include <raylib.h>
#include "json.hpp"
#include "Interface.h"
class Enemy; 

enum class EventType
{
    None,
    IncrementVelocity,
    ChangeVelocity,
    SetEasingVelocity,
    PlaySound
};
struct FrameEvent
{
    EventType eventType;
    std::function<void()> triggerFunction;
    std::string payload;
    FrameEvent(EventType eventType = EventType::None, const std::string& eventPayload = "")
        : eventType(eventType), payload(eventPayload), triggerFunction(nullptr) { }
    void load(const nlohmann::json& j);
	nlohmann::json to_json() const;
};
struct FrameData {
    int frameIndex;
	std::vector<FrameEvent> events;
    std::vector<Rectangle> hitboxes;

    FrameData() : frameIndex(0) {}
    void load(const nlohmann::json& j);

    nlohmann::json to_json() const;
};
class FrameLoader {
public:
    static std::unordered_map<std::string, std::unordered_map<int, FrameData>> loadFromFile(const std::string& path);
};
class FrameDatabase {
    std::unordered_map<EnemyType, std::unordered_map<std::string, std::unordered_map<int, FrameData>>> enemyData;
    std::unordered_map<EnemyType, bool> loadedEnemyTypes;

    FrameDatabase() = default;

    void loadEnemyTypeInternal(EnemyType type);



public:
    static FrameDatabase& getInstance();

    void loadFromFile(const std::string& filename);
    void loadEnemyType(EnemyType type);
    const FrameData* getFrameData(EnemyType type, const std::string& animName, int frame) ;
    const std::unordered_map<int, FrameData>* getAnimation(EnemyType type, const std::string& animName) const;
    const std::unordered_map<std::string, std::unordered_map<int, FrameData>>& getAll(EnemyType type) const;
};

namespace FrameEventHandlers {
    std::function<void()> createChangeVelocityHandler(const std::string& payload, Enemy* enemy);
    std::function<void()> createIncrementVelocityHandler(const std::string& payload, Enemy* enemy);
    std::function<void()> createSetEasingVelocityHandler(const std::string& payload, Enemy* enemy);
    std::function<void()> createPlaySoundHandler(const std::string& payload);
    std::function<void()> bind(const FrameEvent& event, Enemy* enemy); // Optional utility
}