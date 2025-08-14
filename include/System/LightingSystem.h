#pragma once
#include <memory>
#include <raylib.h>
#include <vector>
#include <string>
enum DistortionType {
    ShockWave
};
struct LightSource {
    Vector2 position;
    float radius;
    float brightness;
    Color innerColor;
    Color outerColor;
    bool isActive =true; 
};
struct DistortionEffect {
    Vector2 center;
    float time = 0;
    float timer = 0; 
    float strength;
    float radius;
    int type;
    bool isActive =true;
};

class LightingManager {
public:
    static LightingManager& getInstance();

    ~LightingManager();
    void update(float deltaTime);
    // Light source management
    int addLight(std::shared_ptr<LightSource> light);
    void updateLight(std::shared_ptr<LightSource> light);
    void removeLight(std::shared_ptr<LightSource> light);

    // Shader loading/switching
    void loadShader(const std::string& path);
    void switchShader(const std::string& path);

    // Post-processing effects
    int addDistortion(std::shared_ptr<DistortionEffect> effect);
    void updateDistortion( std::shared_ptr<DistortionEffect> effect);
    void removeDistortion(std::shared_ptr<DistortionEffect> effect);
    void clearDistortions();

    // Lighting pipeline
    void setAmbientColor(Color color);
    Shader& getShader() { return lightingShader; }
    const std::vector<std::shared_ptr<LightSource>>& getLights() { return lights; }
    void prepareShader(Camera2D cam);
    RenderTexture2D& getLightMap() { return lightMap; }
private:
    Shader lightingShader;
    RenderTexture2D lightMap;
    std::vector<std::shared_ptr<LightSource>> lights;
    std::vector<std::shared_ptr<DistortionEffect>> distortions;
    Color ambientColor;

    void sendLightsToShader(Camera2D cam);
    void sendDistortionsToShader(Camera2D cam);
};
