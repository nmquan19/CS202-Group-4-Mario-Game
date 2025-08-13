#include "../../../../include/Enemy/Boss/BehaviorTree.h"
#include "../../../../include/Enemy/Boss/DryBowser/DryBowserPhase.h"
#include "../../../../include/Enemy/Boss/BT_Factory.h"
#include "../../../../include/Enemy/Boss/DryBowser/DryBowserMoveState.h"
#include <fstream>
#include <iostream>
#include <string>
DryBowserPhase1BT::DryBowserPhase1BT() {
       
	root = BTFactory::getInstance().createTree(EnemyType::DRY_BOWSER);
	currentState = std::make_shared<DryBowserStandingState>();  
		//nlohmann::json jsonData = BTFactory::getInstance().to_json(root);
      /*  std::ofstream file;   
		file.open("assets/enemy/BT/DryBowserPhase1BT.json");
        if (file.is_open()) {
            
            std::cout << "File opened for writing: " << "\n";
            file << jsonData.dump(4);
            file.close();
        }
        else
        {
            std::cerr << "Failed to open file for writing: " <<"\n";
        }*/
        
}
void DryBowserPhase1BT::enter(Boss* boss)
{
	this->changeMoveState(boss, std::make_shared<DryBowserIntroState>());
}
void DryBowserPhase1BT::exit(Boss* boss)
{

}
void DryBowserPhase1BT::changeMoveState(Boss* boss, std::shared_ptr<BossMoveState> moveState)
{

        if (currentState) currentState->exit(boss);
        currentState = moveState;
        if (currentState) currentState->enter(boss);
}
void DryBowserPhase1BT::update(Boss* boss, float dt){
           
    if (boss->getCurAnimation() == "Die")
    {
        root = nullptr; 
        changeMoveState(boss, nullptr);
    }
        if (root)root->tick(boss, dt);
        if(currentState)currentState->update(boss,dt);
}
std::string DryBowserPhase1BT::getCurMove() const {
        return currentState ? currentState->getName() : "None";
}
bool DryBowserPhase1BT::isMoveFinished() const {
        return currentState ? currentState->isFinished() : true;
}
void DryBowserPhase2GOAP ::enter(Boss* boss) {
    
}
void DryBowserPhase2GOAP::exit(Boss* boss) {

}
void DryBowserPhase2GOAP::update(Boss* boss, float dt)  {
   
}
void DryBowserPhase2GOAP::changeMoveState(Boss* boss, std::shared_ptr<BossMoveState> moveState)
{
        /*currentState->exit(boss);
        currentState = moveState;
        currentState->enter(boss);*/
}
std::string DryBowserPhase2GOAP::getCurMove() const {
        return currentState ? currentState->getName() : "None";
}
bool DryBowserPhase2GOAP::isMoveFinished() const {
        return currentState ? currentState->isFinished() : true;
}