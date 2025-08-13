#include "../../include/Characters/Character.h"
#include "../../include/Characters/AttackState.h"
#include "../../include/Characters/IdleState.h"
#include "../../include/Game/GameContext.h"
#include "../../include/System/Box2DWorldManager.h"

void AttackState::enter(Character* character) {
	character->aniSpeed = 0.2f;
	character->aniTimer = 0.0f;
	character->attackTimer = 0.4f;
	character->invincibleTimer = 0.5f;
	character->setCurrentStateRow(6);
	character->projectilesLeft -= 1;
}

void AttackState::update(Character* character, float deltaTime) {
	if (character->attackTimer > 0) {
		character->attackTimer -= deltaTime;
	}
}

void AttackState::exit(Character* character) {
	std::cout << character->attackTimer << std::endl;
	Vector2 centerPosition = Box2DWorldManager::b2ToRaylib(character->physicsBody->GetPosition());
	int direction = (character->isFacingRight() ? 1 : -1);
	Vector2 spawnPosition = {
		centerPosition.x + direction * (character->size.x * Constants::TILE_SIZE * 0.5f + 15.0f),
		centerPosition.y
	};
	GameContext::getInstance().setPlayerCallsRequest(character->id == PlayerID::PLAYER_01 ? 1 : 2);
	GameContext::getInstance().addObject(ProjectileType::FIRE_BALL, spawnPosition, Constants::Projectile::STANDARD_SIZE);
}

void AttackState::handleInput(Character* character, const InputState& input) {

}

void AttackState::checkTransitions(Character* character, const InputState& input) {
	if (character->attackTimer <= 0) {
		character->changeState(IdleState::getInstance());
	}
}

AttackState& AttackState::getInstance() {
	static AttackState instance;
	return instance;
}