#include "../../include/System/FrameData.h"
#include <fstream>
#include <string>
#include <unordered_map>
#include <utility>
#include "../../include/System/json.hpp"
#include <raylib.h>
FrameDatabase& FrameDatabase::getInstance()
{
    static FrameDatabase database;
    return database;
}
void FrameData::load(const nlohmann::json& j) {
    frameIndex = j.at("frame").get<int>();
    eventPayload = j.value("eventPayload", "");
    std::string eventStr = j.value("eventType", "changeVelocity");

    if (eventStr == "changeVelocity") eventType = EventType::ChangeVelocity;
    else if (eventStr == "setEasingVelocity") eventType = EventType::SetEasingVelocity;
    else if (eventStr == "playSound") eventType = EventType::PlaySound;

    if (j.contains("hitboxes")) {
        for (const auto& rectJson : j["hitboxes"]) {
            Rectangle r;
            r.x = rectJson["x"];
            r.y = rectJson["y"];
            r.width = rectJson["w"];
            r.height = rectJson["h"];
            hitboxes.push_back(r);
        }
    }
}

nlohmann::json FrameData::to_json() const {
    nlohmann::json j;
    j["frame"] = frameIndex;
    j["eventType"] =
        (eventType == EventType::ChangeVelocity) ? "changeVelocity" :
        (eventType == EventType::SetEasingVelocity) ? "setEasingVelocity" :
        "playSound";
    j["eventPayload"] = eventPayload;
    if (!hitboxes.empty()) {
        j["hitboxes"] = nlohmann::json::array();
        for (const auto& rect : hitboxes) {
            j["hitboxes"].push_back({
                {"x", rect.x}, {"y", rect.y},
                {"w", rect.width}, {"h", rect.height}
                });
        }
    }
    return j;
}



void FrameDatabase::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;

    nlohmann::json j;
    file >> j;

    for (const auto& [animName, frameList] : j.items()) {
        std::unordered_map<int, FrameData> frames;
        for (const auto& frameJson : frameList) {
            FrameData data;
            data.load(frameJson);
            frames[data.frameIndex] = data;
        }
        data[animName] = std::move(frames);
    }
}

const FrameData* FrameDatabase::getFrameData(const std::string& animName, int frame) const {
    auto animIt = data.find(animName);
    if (animIt != data.end()) {
        auto frameIt = animIt->second.find(frame);
        if (frameIt != animIt->second.end()) {
            return &frameIt->second;
        }
    }
    return nullptr;
}

const std::unordered_map<int, FrameData>* FrameDatabase::getAnimation(const std::string& animName) const {
    auto it = data.find(animName);
    if (it != data.end()) return &it->second;
    return nullptr;
}

const std::unordered_map<std::string, std::unordered_map<int, FrameData>>& FrameDatabase::getAll() const {
    return data;
}

