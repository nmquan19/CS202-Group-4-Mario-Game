#include "../../../include/Objects/Projectile/Projectile.h"
#include "../../../include/System/Box2DWorldManager.h"


Projectile::Projectile(Vector2 position, Vector2 size) {
	currentFrame = 0;
	aniSpeed = 0.1f;
	aniTimer = 0.0f;
	this->size = size;
}

Projectile::~Projectile() {
	if (physicsBody) {
		Box2DWorldManager::getInstance().destroyBody(physicsBody);
		physicsBody = nullptr;
	}
}

void Projectile::update(float deltaTime) {
	aniTimer += deltaTime;
	if (aniTimer >= aniSpeed) {
		currentFrame = (currentFrame + 1) % frameData.size();
		spriteRec = frameData[currentFrame];
		aniTimer = 0.0f;
	}
}

void Projectile::draw() {
	Rectangle src = spriteRec;
	Rectangle dest = getHitBox()[0];
	DrawTexturePro(texture, src, dest, { 0, 0 }, 0.0f, WHITE);
}

std::vector<Rectangle> Projectile::getHitBox() const {
	Vector2 centerPosition = Box2DWorldManager::b2ToRaylib(physicsBody->GetPosition());
	Vector2 ssize = { size.x * Constants::TILE_SIZE, size.y * Constants::TILE_SIZE };
	return { {centerPosition.x - ssize.x * 0.5f, centerPosition.y - ssize.y * 0.5f, ssize.x, ssize.y} };
}

ObjectCategory Projectile::getObjectCategory() const {
	return ObjectCategory::PROJECTILE;
}

std::vector<ObjectCategory> Projectile::getCollisionTargets() const {
	return {};
}

void Projectile::onCollision(std::shared_ptr<Object> other, Direction direction) {

}

void Projectile::setActive(bool flag) {

}

bool Projectile::isActive() const {
	return true;
}

void Projectile::setCollided(bool flag) {

}

bool Projectile::isCollided() const {
	return false;
}

Vector2 Projectile::getPosition() const {
	return {};
}

void Projectile::setPosition(Vector2 newPos) {

}

Vector2 Projectile::getSize() const {
	return size;
}