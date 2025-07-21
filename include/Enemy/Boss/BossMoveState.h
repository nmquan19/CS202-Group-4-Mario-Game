#pragma once
#include <string>
class Boss;

class BossMoveState {
public:
    virtual ~BossMoveState() = default;
    virtual void enter(Boss* boss) = 0;
    virtual void update(Boss* boss, float dt) = 0;
    virtual void exit(Boss* boss) = 0;
    virtual bool isFinished() const = 0;
    virtual bool canBeInterrupted() { return false; } 
    virtual std::string getName() const = 0;
};

class TimedCastMoveState : public BossMoveState {
protected:
    float castDuration = 0.2f;
    float elapsed = 0.0f;
    bool castComplete = false;

public:
    void enter(Boss* boss) override {
        /*elapsed = 0.0f;
        castComplete = false;
        boss->setVelocity({ 0, 0 });
        boss->setAnimation(getCastAnimation());*/
    }

    void update(Boss* boss, float dt) override {
        elapsed += dt;
        if (!castComplete && elapsed >= castDuration) {
            castComplete = true;
           /* boss->setAnimation(getExecutionAnimation());
            applyExecution(boss);*/
        }
    }

    bool isInCastPhase() const  { return !castComplete; }
    float getCastDuration() const { return castDuration; }

    virtual std::string getCastAnimation() const = 0;
    virtual std::string getExecutionAnimation() const = 0;
    virtual void applyExecution(Boss* boss) = 0;
};

