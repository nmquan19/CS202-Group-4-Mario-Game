#include "../../include/Item/Item.h"

Item::Item(){ }



Rectangle Item::getHitBox() const{
	return hitbox;
}




bool Item::isActive() {
	return IsActive;
}

