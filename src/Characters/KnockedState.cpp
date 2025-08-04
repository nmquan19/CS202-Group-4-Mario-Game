#include "../../include/Characters/Character.h"
#include "../../include/Characters/KnockedState.h"
#include "../../include/Characters/IdleState.h"
#include "../../include/System/Constant.h"
#include "../../include/System/Box2DWorldManager.h"
#include <iostream>

void KnockedState::enter(Character* character) {
	character->setCurrentStateRow(4);
	character->reviveTimer = Constants::Character::REVIVE_TIME;
	b2Fixture* fixtures = character->physicsBody->GetFixtureList();
	for (b2Fixture* fixture = fixtures; fixture != nullptr; fixture = fixture->GetNext()) {
		b2Filter filter = fixture->GetFilterData();
		filter.maskBits = 0x0000;
		fixture->SetFilterData(filter);
	}
}

void KnockedState::update(Character* character, float deltaTime) {
	if (character->reviveTimer <= 0) {
		character->changeState(IdleState::getInstance());
	}
}

void KnockedState::exit(Character* character) {
	character->hp = 1;
	b2Fixture* fixtures = character->physicsBody->GetFixtureList();
	for (b2Fixture* fixture = fixtures; fixture != nullptr; fixture = fixture->GetNext()) {
		b2Filter filter = fixture->GetFilterData();
		filter.maskBits = static_cast<uint16>(ObjectCategory::CHARACTER);
		fixture->SetFilterData(filter);
	}
	character->physicsBody->SetLinearVelocity(b2Vec2(0, 0));
	character->physicsBody->SetTransform(Box2DWorldManager::raylibToB2(Vector2{ 500, 500 }), 0);
}

KnockedState& KnockedState::getInstance() {
	static KnockedState instance;
	return instance;
}