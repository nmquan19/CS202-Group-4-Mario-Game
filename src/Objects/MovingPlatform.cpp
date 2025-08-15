#include "../../include/Objects/MovingPlatform.h"
#include "../../include/System/Box2DWorldManager.h"
#include "../../include/System/TextureManager.h"
#include <cmath>

MovingPlatform::MovingPlatform(Vector2 position, Vector2 size) : InteractiveObject(position, size) {
	spriteRec = { 405, 467, 50, 10 };
	time = 0.0f;
	physicsBody = Box2DWorldManager::getInstance().createBlockBody(position, Vector2{size.x * Constants::TILE_SIZE, size.y * Constants::TILE_SIZE});
	if (physicsBody) {
		physicsBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
		for (b2Fixture* fixture = physicsBody->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
			b2Filter filter = fixture->GetFilterData();
			filter.maskBits = static_cast<uint16>(ObjectCategory::INTERACTIVE);
			filter.categoryBits = static_cast<uint16>(ObjectCategory::CHARACTER) | static_cast<uint16>(ObjectCategory::ENEMY) | 
				static_cast<uint16>(ObjectCategory::ITEM) | static_cast<uint16>(ObjectCategory::PROJECTILE);
			fixture->SetFilterData(filter);
		}
	}
	interactiveType = InteractiveType::MOVING_PLATFORM;
}

MovingPlatform::~MovingPlatform() {
	if (physicsBody) {
		if (physicsBody) {
			Box2DWorldManager::getInstance().destroyBody(physicsBody);
			physicsBody = nullptr;
		}
	}
}

void MovingPlatform::update(float deltaTime) {
	time += deltaTime;
	float amplitude = Box2DWorldManager::raylibToB2(Constants::MovingPlatform::AMPLITUDE);
	float period = Constants::MovingPlatform::PERIOD;
	float omega = 2 * PI / period;
	physicsBody->SetLinearVelocity(b2Vec2(-omega * amplitude * sin(omega * time + PI), 0));
	Vector2 centerPosition = Box2DWorldManager::b2ToRaylib(physicsBody->GetPosition());
	hitBox.x = centerPosition.x - size.x * Constants::TILE_SIZE * 0.5f;
	hitBox.y = centerPosition.y - size.y * Constants::TILE_SIZE * 0.5f;
}

void MovingPlatform::draw() {
	Rectangle src = spriteRec;
	Rectangle dest = hitBox;
	DrawTexturePro(TextureManager::interactiveTextures, src, dest, { 0, 0 }, 0.0f, WHITE);
}

void MovingPlatform::onCollision(std::shared_ptr<Object> other, Direction dir) {

}

std::vector<Rectangle> MovingPlatform::getHitBox() const {
	return {};
}

ObjectType MovingPlatform::getObjectType() const {
	return InteractiveType::MOVING_PLATFORM;
}

Vector2 MovingPlatform::getSize() const {
	return size;
}