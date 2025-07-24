#include "../../include/System/FrameData.h"
#include <string>
#include <unordered_map>
#include <utility>
#include "../../include/System/json.hpp"
#include <raylib.h>
#include <functional>
#include "../../include/Enemy/Enemy.h"
#include "../../include/System/InterpolationController.h"
#include <sstream>
#include <stdexcept>
#include <tuple>
#include <vector>
#include <fstream>
#include "../../include/UI/SoundEffect.h"
#include "../../include/System/Interface.h"
using json = nlohmann::json;

std::tuple<Vector2, Vector2, int, int, std::string> parseVelocityPayload(const std::string& payload) {
    std::istringstream ss(payload);
    std::string token;
    std::vector<std::string> tokens;
    while (std::getline(ss, token, ',')) {
        tokens.push_back(token);
    }
    if (tokens.size() != 7) {
        throw std::runtime_error("Invalid payload format: expected 7 values");
    }
    Vector2 fromV, toV; 
    fromV.x = std::stof(tokens[0]);
    fromV.y = std::stof(tokens[1]);
	
    toV.x = std::stof(tokens[2]);
	toV.y = std::stof(tokens[3]);

    int fromF = std::stoi(tokens[4]);
    int toF = std::stoi(tokens[5]);
    std::string easingStr = tokens[6];
    return { fromV, toV, fromF, toF, easingStr };
}

namespace FrameEventHandlers {

   
    std::function<void()> createChangeVelocityHandler(const std::string& payload, Enemy* enemy) {
        std::istringstream ss(payload);
        float vx = 0.0f, vy = 0.0f;
        char comma;
        ss >> vx >> comma >> vy;
		Vector2 direction = enemy->getDirection();  
        Vector2 newVelocity = {vx*direction.x, vy*direction.y };
        return [enemy, newVelocity]() {
            enemy->setVelocity(newVelocity);
            };
    }
    std::function<void()> createIncrementVelocityHandler(const std::string& payload, Enemy* enemy) {
        std::istringstream ss(payload);
        float vx = 0.0f, vy = 0.0f;
        char comma;
        ss >> vx >> comma >> vy;
        Vector2 direction = enemy->getDirection();
        Vector2 newVelocity = { vx * direction.x, vy * direction.y };
        return [enemy, newVelocity]() {
            enemy->setVelocity(enemy->getVelocity()+newVelocity);
            };
    }
    std::function<void()> createSetEasingVelocityHandler(const std::string& payload, Enemy* enemy) {
        // Example payload: "0,300,10,20,easeInOut"
        // You’ll need to parse it
        auto [fromV, toV, fromF, toF, easingStr] = parseVelocityPayload(payload);
        auto easingFunc = Easing::getEasingFunction(easingStr);
        return [enemy, fromV, toV, fromF, toF, easingFunc]() {
            enemy->getVelocityController().set(fromF, toF, fromV, toV, easingFunc);
            };
    }

    std::function<void()> createPlaySoundHandler(const std::string& payload) {
        // payload = "roar.wav"
        return [payload]() {
            AudioManager::getInstance().PlaySoundEffect(payload);
            };
    }


    std::function<void()> bind(const FrameEvent& event, Enemy* enemy) {
        switch (event.eventType) {
        case EventType::IncrementVelocity:
			return createIncrementVelocityHandler(event.payload, enemy);
        case EventType::ChangeVelocity:
            return createChangeVelocityHandler(event.payload, enemy);
        case EventType::SetEasingVelocity:
            return createSetEasingVelocityHandler(event.payload, enemy);
        case EventType::PlaySound:
            return createPlaySoundHandler(event.payload);
        default:
            return nullptr;
        }
    }
}

void FrameEvent::load(const nlohmann::json& j) {
    if (j.contains("eventType")) {
        std::string eventTypeStr = j.at("eventType").get<std::string>();
        if (eventTypeStr == "ChangeVelocity") {
            eventType = EventType::ChangeVelocity;
        } else if (eventTypeStr == "SetEasingVelocity") {
            eventType = EventType::SetEasingVelocity;
        } else if (eventTypeStr == "PlaySound") {
            eventType = EventType::PlaySound;
        } else if(eventTypeStr == "IncrementVelocity") {
            eventType = EventType::IncrementVelocity;
		}
        else {
            eventType = EventType::None;
        }
    } else {
        eventType = EventType::None;
    }
    payload = j.value("eventPayload", "");
}
nlohmann::json FrameEvent::to_json() const {
    nlohmann::json j;
    j["eventType"] =
        (eventType == EventType::ChangeVelocity) ? "ChangeVelocity" :
        (eventType == EventType::SetEasingVelocity) ? "SetEasingVelocity" :
        (eventType == EventType::PlaySound) ? "PlaySound" : 
        (eventType == EventType::IncrementVelocity)?"IncrementVelocity" : "None";
    j["eventPayload"] = payload;
    return j;
}
nlohmann::json FrameData::to_json() const {
    nlohmann::json j;
    j["frame"] = frameIndex;
    if(!events.empty()) {
        j["events"] = nlohmann::json::array();
        for (const auto& event : events) {
            j["events"].push_back(event.to_json());
		}
    } 
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


void FrameData::load(const nlohmann::json& j) {
    frameIndex = j.at("frame").get<int>();
    if(j.contains("events")) {
        for (const auto& eventJson : j["events"]) {
            FrameEvent event;
            event.load(eventJson);
            events.push_back(event);
        }
	}
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
std::unordered_map<std::string, std::unordered_map<int, FrameData>> FrameLoader::loadFromFile(const std::string& path) {
    std::unordered_map<std::string, std::unordered_map<int, FrameData>> result;
    std::ifstream file(path);
    if (!file.is_open()) return result;

    nlohmann::json j;
    file >> j;

    for (const auto& [animName, frameList] : j.items()) {
        std::unordered_map<int, FrameData> frames;
        for (const auto& frameJson : frameList) {
            FrameData data;
            data.load(frameJson);
            frames[data.frameIndex] = data;
        }
        result[animName] = std::move(frames);
    }

    return result;
}

FrameDatabase& FrameDatabase::getInstance() {
    static FrameDatabase instance;
    return instance;
}
std::string getEnemyName(EnemyType type) {
    switch (type) {
    case EnemyType::GOOMBA: return "Goomba";
    case EnemyType::RED_KOOPA: return "Red_Koopa";
    case EnemyType::GREEN_KOOPA: return "Green_Koopa";
    case EnemyType::DRY_BOWSER: return "DryBowser";
    default: throw std::runtime_error("Unknown enemy type");
    }
}
void FrameDatabase::loadEnemyTypeInternal(EnemyType type) {
    std::string name = getEnemyName(type);
    std::string path = "assets/enemy/FrameData/" + name + ".json";
    auto frames = FrameLoader::loadFromFile(path);
    enemyData[type] = std::move(frames);
    loadedEnemyTypes[type] = true;
}

void FrameDatabase::loadEnemyType(EnemyType type) {
    if (!loadedEnemyTypes[type]) {
        loadEnemyTypeInternal(type);
    }
}

const FrameData* FrameDatabase::getFrameData(EnemyType type, const std::string& animName, int frame)  {
    auto animIt = enemyData.find(type);
    if (animIt == enemyData.end()) {
        loadEnemyType(type);
        animIt = enemyData.find(type);
        if (animIt == enemyData.end()) {
            return nullptr;
			DrawText("Error: Animation data not found for enemy type", 200, 200, 20, RED);
        }
    }
	DrawText(animName.c_str(), 100, 100, 20, RED);
    const auto& animMap = animIt->second;
    auto frameIt = animMap.find(animName);
    if(frameIt != animMap.end()) {
            auto frameDataIt = frameIt->second.find(frame);
            if (frameDataIt != frameIt->second.end()) {
                DrawText("Animation data found for enemy type", 200, 200, 20, RED);
                return &frameDataIt->second;
            }
    }
    return nullptr;
}

const std::unordered_map<int, FrameData>* FrameDatabase::getAnimation(EnemyType type, const std::string& animName) const {
    auto animIt = enemyData.find(type);
    if (animIt != enemyData.end()) {
        auto animMap = animIt->second;
        auto frameIt = animMap.find(animName);
        if (frameIt != animMap.end()) {
            return &frameIt->second;
        }
    }
    return nullptr;
}

const std::unordered_map<std::string, std::unordered_map<int, FrameData>>& FrameDatabase::getAll(EnemyType type) const {
    static std::unordered_map<std::string, std::unordered_map<int, FrameData>> empty;
    auto it = enemyData.find(type);
    return (it != enemyData.end()) ? it->second : empty;
}


