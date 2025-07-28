#pragma once
#include <memory>
#include <string>
class Boss;
class BossMoveState;
class BossAction;
class BossPhaseState {
public:
    virtual ~BossPhaseState() = default;
    virtual void enter(Boss* boss) = 0;
    virtual void update(Boss* boss, float dt) = 0;
    virtual void exit(Boss* boss) = 0;
    virtual void changeMoveState(Boss* boss, std::shared_ptr<BossMoveState> moveState) = 0; 
    virtual std::string getCurMove() const = 0; 
    virtual bool isMoveFinished() const = 0;
};
