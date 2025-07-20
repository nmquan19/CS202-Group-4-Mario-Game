#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <functional>
#include <raylib.h>
#include "json.hpp"
enum class EventType
{
    None,
    ChangeVelocity,
    SetEasingVelocity,
    PlaySound
};
struct FrameData {
    int frameIndex;
    EventType eventType = EventType::None;
    std::function<void()> triggerFunction;
    std::vector<Rectangle> hitboxes;
    std::string eventPayload;

    FrameData() : frameIndex(0), triggerFunction(nullptr) {}
    FrameData(std::vector<Rectangle> hitboxes,
        EventType eventType = EventType::None,
        const std::string& payload = "")
        : hitboxes(hitboxes), eventType(eventType), eventPayload(payload) {
    }
    void load(const nlohmann::json& j);

    nlohmann::json to_json() const;
};
class FrameDatabase {
    std::unordered_map<std::string, std::unordered_map<int, FrameData>> data;
public:
    static FrameDatabase& getInstance();
    void loadFromFile(const std::string& filename);
    const FrameData* getFrameData(const std::string& animName, int frame) const;
    const std::unordered_map<int, FrameData>* getAnimation(const std::string& animName) const;
    const std::unordered_map<std::string, std::unordered_map<int, FrameData>>& getAll() const;
};