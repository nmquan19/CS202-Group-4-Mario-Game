#include "../../../include/Objects/Projectile/FireBall.h"
#include "../../../include/System/Box2DWorldManager.h"
#include "../../../include/Game/GameContext.h"

FireBall::FireBall(Vector2 position, int direction, Vector2 size) : Projectile(position, size) {
	timer = 0.0f;
	texture = LoadTexture("assets/Fireball_sprites.png");
	frameData = { {14, 8, 9, 10}, {49, 9, 9, 9}, {84, 9, 9, 10}, {119, 9, 9, 9} };
	spriteRec = frameData[0];
	physicsBody = Box2DWorldManager::getInstance().createProjectileBody(position, { size.x * Constants::TILE_SIZE, size.y * Constants::TILE_SIZE });
	if (physicsBody) {
		physicsBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
		for (b2Fixture* fixture = physicsBody->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
			b2Filter filter = fixture->GetFilterData();
			filter.maskBits = static_cast<uint16>(ObjectCategory::PROJECTILE);
			filter.categoryBits = static_cast<uint16>(ObjectCategory::BLOCK) | static_cast<uint16>(ObjectCategory::CHARACTER) |
				static_cast<uint16>(ObjectCategory::ENEMY) | static_cast<uint16>(ObjectCategory::INTERACTIVE) |
				static_cast<uint16>(ObjectCategory::SHELL) | static_cast<uint16>(ObjectCategory::PROJECTILE);
			fixture->SetFilterData(filter);
		}
		physicsBody->SetLinearVelocity(b2Vec2(5 * direction, 0));
	}
}

void FireBall::update(float deltaTime) {
	Projectile::update(deltaTime);
	timer += deltaTime;
	if (timer >= Constants::Projectile::FireBall::DURATION) {
		GameContext::getInstance().mark_for_deletion_Object(GameContext::getInstance().getSharedPtrFromRaw(this));
	}
}

ObjectType FireBall::getObjectType() const {
	return ProjectileType::FIRE_BALL;
}