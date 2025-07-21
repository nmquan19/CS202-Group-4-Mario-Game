#include "../../include/System/InterpolationController.h"
#include <algorithm>
#include <cmath>
#include <functional>



InterpolatedAnimationController::InterpolatedAnimationController()
    : startFrame(0), endFrame(0), duration(1.0f), elapsed(0.0f),
    isLoop(false), reverse(false),
    easingFunction([](float t) { return t; }) {
}

InterpolatedAnimationController::InterpolatedAnimationController(
    int start, int end, float durationSeconds,
    std::function<float(float)> easing, bool rev, bool loop)
    : startFrame(start), endFrame(end), duration(durationSeconds), elapsed(0.0f),
    easingFunction(easing),reverse(rev), isLoop(loop) {
}

void InterpolatedAnimationController::reset() {
    elapsed = 0.0f;
}

void InterpolatedAnimationController::update(float dt) {
    elapsed += dt;
    if (isLoop && elapsed >= duration) {
        elapsed = std::fmod(elapsed, duration);
    }
    else {
        elapsed = std::min(elapsed, duration);
    }
}

void InterpolatedAnimationController::set(int start, int end, float durationSeconds,
    std::function<float(float)> easing, bool rev, bool loop) {
    startFrame = start;
    endFrame = end;
    duration = durationSeconds;
    elapsed = 0.0f;
    easingFunction = easing;
    reverse = rev; 
    isLoop = loop;
}

int InterpolatedAnimationController::getCurrentFrame() const {
    float t = std::clamp(elapsed / duration, 0.0f, 1.0f);
    float easedT = easingFunction(t);

    int totalFrames = std::abs(endFrame - startFrame) + 1;
    int frameOffset = static_cast<int>(easedT * totalFrames);

    if (reverse)
        return std::max(startFrame - frameOffset, endFrame);
    else
        return std::min(startFrame + frameOffset, endFrame);
}

bool InterpolatedAnimationController::isFinished() const {
    return !isLoop && elapsed >= duration;
}

float InterpolatedAnimationController::getProgress() const {
    return std::clamp(elapsed / duration, 0.0f, 1.0f);
}

void InterpolatedAnimationController::setEasing(std::function<float(float)> easing) {
    easingFunction = easing;
}

void InterpolatedAnimationController::setLoop(bool loop) {
    isLoop = loop;
}

void InterpolatedAnimationController::setReverse(bool rev) {
    reverse = rev;
}

