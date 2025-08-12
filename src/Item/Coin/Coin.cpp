#pragma once
#include "../../../include/Item/Coin/Coin.h"
#include "../../../include/Game/GameContext.h"
#include "../../../include/System/Box2DWorldManager.h"

Coin::Coin(Vector2 position) : Item(position) {
	animation->SetType(ItemType::COIN);


	if (physicsBody) {
		Box2DWorldManager::getInstance().destroyBody(physicsBody);
	}
	physicsBody = Box2DWorldManager::getInstance().createItemBodyForCoin(position, { hitbox.width, hitbox.height });
}

Coin::~Coin() {
	// Destructor logic if needed
}
