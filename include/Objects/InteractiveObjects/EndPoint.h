#pragma once


#pragma once
#include "InteractiveObject.h"
#include "../../System/Interface.h"
struct EndpointData {
    Vector2 position;
    Vector2 size;
    int targetLevel = -1; // -1 means go to level selector, positive values go to specific level
    bool requiresCompletion = false; // Whether level must be completed to use this endpoint
    json customData; // For additional properties

    // Serialization
    json toJson() const {
        return {
            {"position", {position.x, position.y}},
            {"size", {size.x, size.y}},
            {"targetLevel", targetLevel},
            {"requiresCompletion", requiresCompletion},
            {"customData", customData}
        };
    }

    void fromJson(const json& j) {
        position = { j["position"][0], j["position"][1] };
        size = { j["size"][0], j["size"][1] };
        targetLevel = j.value("targetLevel", -1);
        requiresCompletion = j.value("requiresCompletion", false);
        customData = j.value("customData", json::object());
    }
};

class Endpoint : public InteractiveObject, public IOnCollisionExit {
private:
    EndpointData data;
    std::unordered_map<Object*, int> activeContacts;
    bool isActivated = false;
    float activationTimer = 0.0f;
    const float activationTime = 1.0f; // Time needed to activate

public:
    Endpoint(Vector2 position, Vector2 size, EndpointData data);
    Endpoint(Vector2 position, Vector2 size);
    ~Endpoint();

    virtual void update(float deltaTime) override;
    virtual void draw() override;
    void onCollision(std::shared_ptr<Object> other, Direction direction) override;
    void onCollisionExit(std::shared_ptr<Object> other, Direction direction) override;
    ObjectType getObjectType() const override;

    Vector2 getSize() const override { return size; };

    // Serialization
    json toJson() const override;
    void fromJson(const json& data) override;

    // Activation methods
    void activate();
    void deactivate();
    bool isActive() const { return isActivated; }

    // Getters
    EndpointData getEndpointData() const { return data; }
    int getTargetLevel() const { return data.targetLevel; }
};