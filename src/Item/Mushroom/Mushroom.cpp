#pragma once
#include "../../../include/Item/Mushroom/Mushroom.h"

Mushroom::Mushroom(Vector2 position) : Item(position) {
	animation->SetType(ItemType::MUSHROOM);
}

Mushroom::~Mushroom() {
	// Destructor logic if needed
}

void Mushroom::Move(double dt) {
	// Implement mushroom-specific movement logic here
	// For example, it could move upwards or bounce around
	velocity.y -= gravity * dt; // Apply gravity
	position.y += velocity.y * dt; // Update position based on velocity
	// Check if the mushroom is below the ground level
	if (position.y > groundY) {
		position.y = groundY; // Reset to ground level
		velocity.y = 0; // Stop falling
	}
	
	// Update hitbox position
	hitbox.x = position.x;
	hitbox.y = position.y;
}

