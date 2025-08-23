#include "../../include/System/LightingSystem.h"
#include <raylib.h>
#include <raymath.h>
#include <memory>

#define MAX_LIGHTS 1000
#define MAX_DISTORTIONS 8

LightingManager& LightingManager::getInstance() {
    static LightingManager instance;
    return instance;
}

LightingManager::~LightingManager() {
    if (lightingShader.id != 0) {
        UnloadShader(lightingShader);
    }
    if (lightMap.id != 0) {
        UnloadRenderTexture(lightMap);
    }
}

int LightingManager::addLight(std::shared_ptr<LightSource> light) {
    if (lights.size() >= MAX_LIGHTS) return -1;
    lights.push_back(light);
    return static_cast<int>(lights.size() - 1);
}

void LightingManager::updateLight(std::shared_ptr<LightSource> light) {
    auto it = std::find(lights.begin(), lights.end(), light);
    if (it != lights.end()) {
        *it = light;
    }
}

void LightingManager::removeLight(std::shared_ptr<LightSource> light) {
    auto it = std::find(lights.begin(), lights.end(), light);
    if (it != lights.end()) {
        lights.erase(it);
    }
}
void LightingManager::loadShader(const std::string& path) {
    if (lightingShader.id != 0) {
        UnloadShader(lightingShader);
    }
    lightingShader = LoadShader(0, path.c_str());
    if (lightMap.id != 0) {
        UnloadRenderTexture(lightMap);
    }
    lightMap = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
}

void LightingManager::switchShader(const std::string& path) {
    if (lightingShader.id != 0) {
        UnloadShader(lightingShader);
    }
    lightingShader = LoadShader(0, path.c_str());
}

int LightingManager::addDistortion(std::shared_ptr<DistortionEffect> effect) {
    if (distortions.size() >= MAX_DISTORTIONS) return -1;
    distortions.push_back(effect);
    return static_cast<int>(distortions.size() - 1);
}

void LightingManager::updateDistortion(std::shared_ptr<DistortionEffect> effect) {
    auto it = std::find(distortions.begin(), distortions.end(), effect);
    if (it != distortions.end()) {
        *it = effect;
    }
}

void LightingManager::removeDistortion(std::shared_ptr<DistortionEffect> effect) {
    auto it = std::find(distortions.begin(), distortions.end(), effect);
    if (it != distortions.end()) {
        distortions.erase(it);
    }
}
void LightingManager::clearDistortions() {
    distortions.clear();
}

void LightingManager::setAmbientColor(Color color) {
    ambientColor = color;
}

void LightingManager::prepareShader(Camera2D cam) {
    Vector2 screenSize = {
        static_cast<float>(GetScreenWidth()),
        static_cast<float>(GetScreenHeight())
    };
    SetShaderValue(lightingShader,
        GetShaderLocation(lightingShader, "screenSize"),
        &screenSize,
        SHADER_UNIFORM_VEC2);

    float ambient[3] = {
        ambientColor.r / 255.0f,
        ambientColor.g / 255.0f,
        ambientColor.b / 255.0f
    };
    SetShaderValue(lightingShader,
        GetShaderLocation(lightingShader, "ambientColor"),
        ambient,
        SHADER_UNIFORM_VEC3);

    sendLightsToShader(cam);
    sendDistortionsToShader(cam);
}

void LightingManager::sendLightsToShader(Camera2D cam) {
    Vector2 positions[MAX_LIGHTS];
    float radii[MAX_LIGHTS];
    float brightness[MAX_LIGHTS];
    Vector3 innerColors[MAX_LIGHTS];
    Vector3 outerColors[MAX_LIGHTS];
    Vector3 ambient = Vector3{ ambientColor.r / 255.0f, ambientColor.g / 255.0f, ambientColor.b / 255.0f };

    for (size_t i = 0; i < lights.size(); i++) {
        positions[i] = GetWorldToScreen2D(lights[i]->position, cam);
        positions[i].y = GetScreenHeight() - positions[i].y;
        radii[i] = lights[i]->radius;
        brightness[i] = lights[i]->brightness;
        innerColors[i] = Vector3{
            lights[i]->innerColor.r / 255.0f,
            lights[i]->innerColor.g / 255.0f,
            lights[i]->innerColor.b / 255.0f
        };
        outerColors[i] = Vector3{
            lights[i]->outerColor.r / 255.0f,
            lights[i]->outerColor.g / 255.0f,
            lights[i]->outerColor.b / 255.0f
        };
    }

    int count = static_cast<int>(lights.size());
    SetShaderValue(lightingShader, GetShaderLocation(lightingShader, "numLights"), &count, SHADER_UNIFORM_INT);
    SetShaderValueV(lightingShader, GetShaderLocation(lightingShader, "lightPositions"), positions, SHADER_UNIFORM_VEC2, count);
    SetShaderValueV(lightingShader, GetShaderLocation(lightingShader, "lightRadii"), radii, SHADER_UNIFORM_FLOAT, count);
    SetShaderValueV(lightingShader, GetShaderLocation(lightingShader, "lightBrightness"), brightness, SHADER_UNIFORM_FLOAT, count);
    SetShaderValueV(lightingShader, GetShaderLocation(lightingShader, "innerColors"), innerColors, SHADER_UNIFORM_VEC3, count);
    SetShaderValueV(lightingShader, GetShaderLocation(lightingShader, "outerColors"), outerColors, SHADER_UNIFORM_VEC3, count);
    SetShaderValue(lightingShader, GetShaderLocation(lightingShader, "ambientColor"), &ambient, SHADER_UNIFORM_VEC3);
}
#include <iostream>
void LightingManager::sendDistortionsToShader(Camera2D cam) {
    Vector2 centers[MAX_DISTORTIONS];
    float times[MAX_DISTORTIONS];
    float timers[MAX_DISTORTIONS];
    float strengths[MAX_DISTORTIONS];
    float radii[MAX_DISTORTIONS];
    int types[MAX_DISTORTIONS];

    Vector2 screenSize = { (float)GetScreenWidth(), (float)GetScreenHeight() };

    for (size_t i = 0; i < distortions.size(); i++) {
        // Convert pixel position to UV space for shader
        centers[i] = GetWorldToScreen2D(distortions[i]->center, cam);
        centers[i].y = GetScreenHeight() - centers[i].y;
        times[i] = distortions[i]->time;      // total duration
        timers[i] = distortions[i]->timer;    // elapsed time
        strengths[i] = distortions[i]->strength;
        radii[i] = distortions[i]->radius;    // max radius
        types[i] = distortions[i]->type;
        
    }
    int count = static_cast<int>(distortions.size());
    SetShaderValue(lightingShader, GetShaderLocation(lightingShader, "numDistortions"), &count, SHADER_UNIFORM_INT);
    SetShaderValueV(lightingShader, GetShaderLocation(lightingShader, "distortionCenters"), centers, SHADER_UNIFORM_VEC2, count);
    SetShaderValueV(lightingShader, GetShaderLocation(lightingShader, "distortionTimes"), times, SHADER_UNIFORM_FLOAT, count);
    SetShaderValueV(lightingShader, GetShaderLocation(lightingShader, "distortionTimers"), timers, SHADER_UNIFORM_FLOAT, count);
    SetShaderValueV(lightingShader, GetShaderLocation(lightingShader, "distortionStrengths"), strengths, SHADER_UNIFORM_FLOAT, count);
    SetShaderValueV(lightingShader, GetShaderLocation(lightingShader, "distortionRadius"), radii, SHADER_UNIFORM_FLOAT, count);
    SetShaderValueV(lightingShader, GetShaderLocation(lightingShader, "distortionTypes"), types, SHADER_UNIFORM_INT, count);
}

void LightingManager::update(float deltaTime)
{
    for (auto& dist : distortions)
    {
        if (!dist->isActive)
            continue;

        // Advance time since creation
        dist->timer += deltaTime;

     
        // Deactivate when done
        if (dist->timer >= dist->time || dist->strength <= 0.01f)
        {
            dist->isActive = false;
        }
    }

    // Remove inactive distortions from the list
    distortions.erase(
        std::remove_if(distortions.begin(), distortions.end(),
            [](std::shared_ptr<DistortionEffect> d) { return !d->isActive; }),
        distortions.end()
    );
    lights.erase(
        std::remove_if(lights.begin(), lights.end(),
            [](std::shared_ptr<LightSource> d) { return !d->isActive; }),
        lights.end()
    );
}
