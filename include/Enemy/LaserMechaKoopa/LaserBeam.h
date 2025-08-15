#pragma once
#pragma once
#include "../Enemy.h"
#include "../../../include/Objects/Projectile/Projectile.h"

class LaserBeam : public Projectile{
public:
    enum State {
        CHARGING,
        FIRING,
        DONE
    };
    LaserBeam(Vector2 startPos, Vector2 dir, float chargeTime, float beamDuration, float beamMaxLength);
    LaserBeam(Vector2 startPos, Vector2 size);
    ObjectType getObjectType() const override{ return ProjectileType::LASER_BEAM; }
    void update(float dt) override;
    void draw() override;
    bool isDone() const { return state == DONE; }
    void setChargeTime(float t) {
        chargeTime = t;
    }
    void setBeamDuration(float t)
    {
        beamDuration = t;
    }
    void setMaxLength(float t)
    {
        beamMaxLength = t; 
    }
    void setDirection(Vector2 dir)
    {
        this->direction = dir;
    }
private:
    void updateCharge(float dt);
    void updateFire(float dt);
    void performRaycast();
    void onCollision(std::shared_ptr<Object> other, Direction direction) override;
    Vector2 startPoint;
    Vector2 direction;
    float chargeTime;
    float beamDuration;
    float beamMaxLength;

    float timer;
    float beamLength;
    State state;

    // Visual tweak parameters
    float chargeStartWidth;
    float chargeEndWidth;
    Color chargeColor;
    Color beamColor;

    // Optional: hitpoint effect location
    Vector2 impactPoint;
    bool hasImpact;
};
