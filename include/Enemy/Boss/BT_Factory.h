// BTFactory.h
#pragma once

#include <memory>
#include <unordered_map>
#include <string>
#include "../../System/Interface.h"
#include "BehaviorTree.h"
#include "../../System/json.hpp"
class BTFactory {
public:
    static BTFactory& getInstance();

    //void loadFromJson(const std::string& filename);
    std::shared_ptr<BehaviorTreeNode> createTree(EnemyType type);
    nlohmann::json to_json(const std::shared_ptr<BehaviorTreeNode>& node);
private:
    std::shared_ptr<BehaviorTreeNode> buildTree(const nlohmann::json& nodeJson);
};
