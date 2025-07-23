#include "../../include/Item/Item.h"



Item::Item(){
	hitbox = { 0,0,0,0 };


	PhysicsManager::getInstance().addObject(this);
}

Item::~Item() {
	PhysicsManager::getInstance().markForDeletion(this);
}

std::vector<Rectangle> Item::getHitBox() const{
	return {hitbox};
}




bool Item::isActive() {
	return IsActive;
}

