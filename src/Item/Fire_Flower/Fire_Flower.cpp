#pragma once
#include "../../../include/Item/Fire_Flower/Fire_Flower.h"
#include "../../../include/Game/GameContext.h"
#include "../../../include/System/Box2DWorldManager.h"

Fire_Flower::Fire_Flower(Vector2 position) : Item(position) {
	animation->SetType(ItemType::FIRE_FLOWER);
}

Fire_Flower::~Fire_Flower() {
	// Destructor logic if needed
}

void Fire_Flower::update(float deltaTime) {
	animation->Update();






	// Update position based on physics body
	Vector2 renderPos = Box2DWorldManager::b2ToRaylib(physicsBody->GetPosition());
	position.x = renderPos.x - Constants::TILE_SIZE / 2;
	position.y = renderPos.y - Constants::TILE_SIZE / 2;
	hitbox.x = position.x;
	hitbox.y = position.y;
	// Additional logic for Fire Flower can be added here
}



