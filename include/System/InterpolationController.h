#pragma once
#pragma once
#include <cmath>
#include <algorithm>
#include <functional>

class InterpolatedAnimationController {
private:
    int startFrame;
    int endFrame;
    float duration;
    float elapsed;
    std::function<float(float)> easingFunction; 
public:
    InterpolatedAnimationController(int start, int end, float durationSeconds,
        std::function<float(float)> easing = [](float t) { return t; })
        : startFrame(start), endFrame(end), duration(durationSeconds), elapsed(0.0f), easingFunction(easing) {
    }

    void reset() {
        elapsed = 0.0f;
    }

    void update(float dt) {
        elapsed = std::min(elapsed + dt, duration);
    }
    
    void set(int start, int end, float durationSeconds, std::function<float(float)> easing = [](float t) { return t; }){
		startFrame = start;
        endFrame = end;
        duration = durationSeconds;
        elapsed = 0.0f;
		easingFunction = easing;
    }

    int getCurrentFrame() const {
        float t = std::clamp(elapsed / duration, 0.0f, 1.0f);
        float easedT = easingFunction(t);

        int totalFrames = std::abs(endFrame - startFrame) + 1;
        int frameOffset = static_cast<int>(easedT * totalFrames);

        if (startFrame <= endFrame) {
            return std::min(startFrame + frameOffset, endFrame);
        }
        else {
            return std::max(startFrame - frameOffset, endFrame);
        }
    }


    bool isFinished() const {
        return elapsed >= duration;
    }

    float getProgress() const {
        return std::clamp(elapsed / duration, 0.0f, 1.0f);
    }

    void setEasing(std::function<float(float)> easing) {
        easingFunction = easing;
    }
};

namespace Easing {
    inline float linear(float t) { return t; }
    inline float easeIn(float t) { return t * t; }
    inline float easeOut(float t) { return 1 - (1 - t) * (1 - t); }
    inline float easeInOut(float t) {
        return t < 0.5f ? 2 * t * t : 1 - std::pow(-2 * t + 2, 2) / 2;
    }
}



