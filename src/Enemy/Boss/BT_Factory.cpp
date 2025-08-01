#include "../../../include/Enemy/Boss/BehaviorTree.h"
#include "../../../include/Enemy/Boss/BT_Factory.h"
#include <fstream>
#include <unordered_map>
#include "../../../include/System/json.hpp"
#include "../../../include/System/Interface.h"
using json = nlohmann::json;

BTFactory& BTFactory::getInstance() {
    static BTFactory instance;
    return instance;
}
std::shared_ptr<BehaviorTreeNode> BTFactory::buildTree(const json& nodeData) {
    std::string type = nodeData.at("type");

    if (type == "Sequence") {
        auto node = std::make_shared<SequenceNode>();
        for (const auto& child : nodeData.at("children")) {
            node->addChild(buildTree(child));
        }
        return node;
    }
    else if (type == "Selector") {
        auto node = std::make_shared<SelectorNode>();
        for (const auto& child : nodeData.at("children")) {
            node->addChild(buildTree(child));
        }
        return node;
    }
    else if (type == "ReactiveFallBack") {
        auto node = std::make_shared<ReactiveFallBackNode>();
        for (const auto& child : nodeData.at("children")) {
            node->addChild(buildTree(child));
        }
        return node;
    }
    else if (type == "Repeat") {
        auto node = std::make_shared<RepeatNode>();
        int times = nodeData.value("times", 1);
        node->setNumCycles(times); 
		node->setChild(buildTree(nodeData.at("child")));
        return node; 
    }
    else if (type == "Inverter") {
        auto node = std::make_shared<InverterNode>();
        node->setChild(buildTree(nodeData.at("child")));    
        return node;
    }
    else if (type == "RunOnce") {
        auto node = std::make_shared<RunOnceNode>();
        node->setChild(buildTree(nodeData.at("child")));
        return node;
    }
    else if (type == "WalkToTarget") {
        return std::make_shared<WalkToTargetNode>();
    }
    else if (type == "Attack") {
        return std::make_shared<AttackNode>();
    }
    else if (type == "Idle") {
        return std::make_shared<IdleNode>();
    }
    else if (type == "WalkTurn")
    {
        return std::make_shared<WalkTurnNode>();
    }
    else if (type == "NeedTurn")
    {
        return std::make_shared<NeedTurnNode>();
    }
    else if (type == "IsTargetInRange")
    {
        return std::make_shared<IsTargetInRangeNode>();
    }
    else if (type == "CanUseSpin")
    {
        return std::make_shared<CanUseSpinNode>();
    }
    else if(type=="SpinAttack")
    {
        return std::make_shared<SpinAttackNode>();
	}
    else if(type == "CanUseMeleeAttack1")
    {
        return std::make_shared<CanUseMeleeAttack1Node>();
	}
    else if (type == "SpinAttackWindup")
    {
        return std::make_shared<SpinAttackWindupNode>();
    }
    else if (type == "SpinAttackWinddown")
    {
        return std::make_shared<SpinAttackWinddownNode>();
    }
    else if (type == "IsTakingDamage")
    {
        return std::make_shared<IsTakingDamageNode>();
    }
    else if (type == "Intro")
    {
		return std::make_shared<IsInIntroNode>();
    }
    else if (type == "CanWallJump") {
        return std::make_shared<CanWallJumpNode>();
    }
    else if (type == "WallJump") {
        return std::make_shared<WallJumpNode>();
    }
    else if (type == "Jump") {
        return std::make_shared<JumpNode>();
    }
    else if (type == "IsInAir") {
        return std::make_shared<IsInAirNode>();
    }
    else if (type == "AerialAttack") {
        return std::make_shared<AerialAttackNode>();
    }
    else if (type == "IsInWallJump") {
        return std::make_shared<IsInWallJumpNode>();
    }
    else if (type == "IsPlayerHigher") {
        return std::make_shared<IsPlayerHigherNode>();
    }
    else if (type == "IsFalling") {
        return std::make_shared<IsFallingNode>();
    }
    throw std::runtime_error("Unknown node type: " + type);
}
std::string GetEnemyName(EnemyType type) {
    switch (type) {
    case EnemyType::GOOMBA: return "Goomba";
    case EnemyType::RED_KOOPA: return "Red_Koopa";
    case EnemyType::GREEN_KOOPA: return "Green_Koopa";
    case EnemyType::DRY_BOWSER: return "DryBowser";
    default: throw std::runtime_error("Unknown enemy type");
    }
}

std::shared_ptr<BehaviorTreeNode> BTFactory::createTree(EnemyType type) {
    std::string filename;

	filename = "assets/enemy/BT/" + GetEnemyName(type) + ".json";
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open BT JSON file: " + filename);
    }

    json treeJson;
    file >> treeJson;   

    return buildTree(treeJson);
}





nlohmann::json BTFactory::to_json(const std::shared_ptr<BehaviorTreeNode>& node) {
    if (!node) return {};

    nlohmann::json j;

    if (auto seq = std::dynamic_pointer_cast<SequenceNode>(node)) {
        j["type"] = "Sequence";
        j["children"] = nlohmann::json::array();
        for (const auto& child : seq->getChildren()) {
            j["children"].push_back(to_json(child));
        }
    }
    else if (auto sel = std::dynamic_pointer_cast<SelectorNode>(node)) {
        j["type"] = "Selector";
        j["children"] = nlohmann::json::array();
        for (const auto& child : sel->getChildren()) {
            j["children"].push_back(to_json(child));
        }
    }
    else if (auto sel = std::dynamic_pointer_cast<ReactiveFallBackNode>(node)) {
        j["type"] = "ReactiveFallBack";
        j["children"] = nlohmann::json::array();
        for (const auto& child : sel->getChildren()) {
            j["children"].push_back(to_json(child));
        }
    }
    else if (auto rep = std::dynamic_pointer_cast<RepeatNode>(node)) {
        j["type"] = "Repeat";
        j["times"] = rep->getNumCycles();
        j["child"] = to_json(rep->getChild());
    }
    else if (auto inv = std::dynamic_pointer_cast<InverterNode>(node)) {
        j["type"] = "Inverter";
        j["child"] = to_json(inv->getChild());
    }
    else if (auto inv = std::dynamic_pointer_cast<RunOnceNode>(node)) {
        j["type"] = "RunOnce";
        j["child"] = to_json(inv->getChild());
    }
    else if (std::dynamic_pointer_cast<WalkToTargetNode>(node)) {
        j["type"] = "WalkToTarget";
    }
    else if (std::dynamic_pointer_cast<AttackNode>(node)) {
        j["type"] = "Attack";
    }
    else if (std::dynamic_pointer_cast<IdleNode>(node)) {
        j["type"] = "Idle";
    }
    else if (std::dynamic_pointer_cast<WalkTurnNode>(node)) {
        j["type"] = "WalkTurn";
    }
    else if (std::dynamic_pointer_cast<IsTargetInRangeNode>(node)) {
        j["type"] = "IsTargetInRange";
    }
    else if (std::dynamic_pointer_cast<NeedTurnNode>(node)) {
        j["type"] = "NeedTurn";
    }
    else if (std::dynamic_pointer_cast<CanUseSpinNode>(node)) {
        j["type"] = "CanUseSpin";
    }
    else if (std::dynamic_pointer_cast<SpinAttackNode>(node)) {
        j["type"] = "SpinAttack";
    }
    else if( std::dynamic_pointer_cast<CanUseMeleeAttack1Node>(node)) {
        j["type"] = "CanUseMeleeAttack1";
	}
    else if (std::dynamic_pointer_cast<SpinAttackWindupNode>(node)) {
        j["type"] = "SpinAttackWindup";
    }
    else if (std::dynamic_pointer_cast<SpinAttackWinddownNode>(node)) {
        j["type"] = "SpinAttackWinddown";
    }
    else if (std::dynamic_pointer_cast<IsTakingDamageNode>(node)) {
        j["type"] = "IsTakingDamage";
    }
    else if (std::dynamic_pointer_cast<IsInIntroNode>(node)) {
        j["type"] = "Intro";
    }
    else if (std::dynamic_pointer_cast<CanWallJumpNode>(node)) {
        j["type"] = "CanWallJump";
    }
    else if (std::dynamic_pointer_cast<WallJumpNode>(node)) {
        j["type"] = "WallJump";
    }
    else if (std::dynamic_pointer_cast<JumpNode>(node)) {
        j["type"] = "Jump";
    }
    else if (std::dynamic_pointer_cast<IsInAirNode>(node)) {
        j["type"] = "IsInAir";
    }
    else if (std::dynamic_pointer_cast<AerialAttackNode>(node)) {
        j["type"] = "AerialAttack";
    }
    else if (std::dynamic_pointer_cast<IsInWallJumpNode>(node)) {
        j["type"] = "IsInWallJump";
    }
    else if (std::dynamic_pointer_cast<IsPlayerHigherNode>(node)) {
        j["type"] = "IsPlayerHigher";
    }
    else if (std::dynamic_pointer_cast<IsFallingNode>(node)) {
        j["type"] = "IsFalling";
    }
    else {
        throw std::runtime_error("Unknown node type during serialization");
    }

    return j;
}
