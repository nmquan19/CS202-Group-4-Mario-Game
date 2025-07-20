#pragma once
#include <memory>
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
};
