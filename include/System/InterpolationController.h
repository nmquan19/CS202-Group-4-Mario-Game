#pragma once
#pragma once
#include <cmath>
#include <algorithm>
#include <functional>
#include <vector>
#include <raylib.h>
#include <string>
#pragma once
#include <cmath>
#include <algorithm>
#include <functional>
#include <unordered_map>
#include <vector>
#include <raylib.h>
#include <string>
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
        : hitboxes(hitboxes), eventType(eventType), eventPayload(payload) {}
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

class InterpolatedAnimationController {
private:
    int startFrame;
    int endFrame;
    float duration;
    float elapsed;
    bool isLoop;
    bool reverse;
    std::function<float(float)> easingFunction;

public:
    InterpolatedAnimationController();
    InterpolatedAnimationController(int start, int end, float durationSeconds,
        std::function<float(float)> easing = [](float t) { return t; }, bool rev =false, bool loop =false);

    void reset();
    void update(float dt);
    void set(int start, int end, float durationSeconds,
        std::function<float(float)> easing = [](float t) { return t; }, bool rev = false, bool loop = false);

    int getCurrentFrame() const;
    bool isFinished() const;
    float getProgress() const;
    void setEasing(std::function<float(float)> easing);
    void setLoop(bool loop);
    void setReverse(bool rev);
};

class FrameInterpolatedVelocityController {
private:
    int startFrame;
    int endFrame;
    float fromVelocity;
    float toVelocity;
    std::function<float(float)> easingFunction;
    bool active = false;

public:
    FrameInterpolatedVelocityController()
        : startFrame(0), endFrame(0), fromVelocity(0.0f), toVelocity(0.0f),
        easingFunction([](float t) { return t; }) {
    }

    void set(int fromFrame, int toFrame, float fromV, float toV,
        std::function<float(float)> easing = [](float t) { return t; }) {
        startFrame = fromFrame;
        endFrame = toFrame;
        fromVelocity = fromV;
        toVelocity = toV;
        easingFunction = easing;
        active = true;
    }

    float getVelocityAtFrame(int currentFrame) const {
        if (!active || currentFrame < startFrame || currentFrame > endFrame)
            return fromVelocity;

        float t = static_cast<float>(currentFrame - startFrame) / (endFrame - startFrame);
        float eased = easingFunction(std::clamp(t, 0.0f, 1.0f));
        return fromVelocity + (toVelocity - fromVelocity) * eased;
    }

    bool isActiveAtFrame(int currentFrame) const {
        return active && currentFrame >= startFrame && currentFrame <= endFrame;
    }

    void deactivate() { active = false; }
};






namespace Easing {
    inline float linear(float t) { return t; }
    inline float easeIn(float t) { return t * t; }
    inline float easeOut(float t) { return 1 - (1 - t) * (1 - t); }
    inline float easeInOut(float t) {
        return t < 0.5f ? 2 * t * t : 1 - std::pow(-2 * t + 2, 2) / 2;
    }
}


