#pragma once
#pragma once
#include <cmath>
#include <algorithm>
#include <functional>
#include <raylib.h>
#include <string>
#pragma once
#include <cmath>
#include <algorithm>
#include <functional>
#include <vector>
#include <raylib.h>
#include <string>
#include <raymath.h>
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
        std::function<float(float)> easing = [](float t) { return t; }, bool rev = false, bool loop = false);

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
    Vector2 fromVelocity;
    Vector2 toVelocity;
    std::function<float(float)> easingFunction;
    bool active = false;

public:
    FrameInterpolatedVelocityController()
        : startFrame(0), endFrame(0), fromVelocity(0.0f), toVelocity(0.0f),
        easingFunction([](float t) { return t; }) {
    }
    void set(int fromFrame, int toFrame, Vector2 fromV, Vector2 toV,
        std::function<float(float)> easing = [](float t) { return t; }) {
        startFrame = fromFrame;
        endFrame = toFrame;
        fromVelocity = fromV;
        toVelocity = toV;
        easingFunction = easing;
        active = true;
    }

    Vector2 getVelocityAtFrame(int currentFrame) const {
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
    inline std::function<float(float)> getEasingFunction(const std::string& ease) {
        if (ease == "linear") return linear;
        else if (ease == "easeIn") return easeIn;
        else if (ease == "easeOut") return easeOut;
        else if (ease == "easeInOut") return easeInOut;
        return linear;
    }
}