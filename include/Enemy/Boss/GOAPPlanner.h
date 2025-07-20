#pragma once
#pragma once
#include <vector>
#include <memory>
#include "WorldState.h"
#include "BossAction.h"

class GOAPPlanner {
public:
    std::vector<std::shared_ptr<BaseBossAction>> plan(
        const WorldState& currentState,
        const WorldState& goalState,
        const std::vector<std::shared_ptr<BaseBossAction>>& actions);
};  
