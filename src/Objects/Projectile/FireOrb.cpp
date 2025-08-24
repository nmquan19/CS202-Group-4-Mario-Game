#include "../../../include/Objects/Projectile/FireOrb.h"
#include "../../../include/System/Box2DWorldManager.h"
#include "../../../include/Game/GameContext.h"

FireOrb::FireOrb(Vector2 position, int direction, Vector2 size) : Projectile(position, size) {
	this->direction = direction;
	timer = 0.0f;
	texture = TextureManager::fireOrbTexture;
	frameData = { {14, 15, 27, 17}, {69, 14, 30, 19} };
	spriteRec = frameData[0];
	physicsBody = Box2DWorldManager::getInstance().createFireOrbBody(position, { size.x * Constants::TILE_SIZE, size.y * Constants::TILE_SIZE });

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
		physicsBody->SetLinearVelocity(b2Vec2(Constants::Projectile::FireOrb::INITIAL_VELOCITY.x * direction, Constants::Projectile::FireOrb::INITIAL_VELOCITY.y));
	}
}

void FireOrb::draw() {
	Rectangle src = spriteRec;
	if (direction < 0) {
		src.width = -src.width;
	}
	Rectangle dest = getHitBox()[0];
	DrawTexturePro(texture, src, dest, { 0, 0 }, 0.0f, WHITE);
}

void FireOrb::onCollision(std::shared_ptr<Object> other, Direction direction) {
	for (b2Fixture* fixture = physicsBody->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
		b2Filter filter = fixture->GetFilterData();
		filter.maskBits = 0;
		fixture->SetFilterData(filter);
	}
	GameContext::getInstance().mark_for_deletion_Object(GameContext::getInstance().getSharedPtrFromRaw(this));
}

ObjectType FireOrb::getObjectType() const {
	return ProjectileType::FIRE_ORB;
}