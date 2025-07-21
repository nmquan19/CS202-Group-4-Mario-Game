#pragma once
#include <string>
#include <memory>
#include "SimulateState.h"
#include "WorldState.h"
#include "BossMoveState.h"

class BaseBossAction {
public:
    virtual ~BaseBossAction() = default;

    virtual std::string getName() const = 0;
    virtual bool checkPreconditions(const WorldState& ws) const = 0;
    virtual float getCost() const = 0;

    virtual BaseSimState simulate(const BaseSimState& state) const = 0;
    virtual void applyEffects(WorldState& ws, const BaseSimState& result) const = 0;

    virtual std::shared_ptr<BossMoveState> toMoveState() const = 0;
};
