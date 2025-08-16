#include "../../../include/Objects/InteractiveObjects/FireBar.h"
#include "../../../include/System/Box2DWorldManager.h"
#include "../../../include/System/TextureManager.h"
#include "../../../include/Game/GameContext.h"

FireBar::FireBar(Vector2 position, Vector2 size) : InteractiveObject(position, size) {
	aniTimer = 0.0f;
	aniSpeed = 0.3f;
	currentFrame = 0;
	frameData = { {182, 550, 16, 43}, {199, 550, 16, 43}, {216, 550, 16, 43}, {233, 550, 16, 43} };
	timer = 0.0f;
	triggering = true;

	b2Vec2 barSize = Box2DWorldManager::raylibToB2(Vector2{ 2.5f * Constants::TILE_SIZE, 0.25f * Constants::TILE_SIZE });
	b2Vec2 barPosition = Box2DWorldManager::raylibToB2(Vector2{ position.x + size.x * Constants::TILE_SIZE * 0.5f, position.y + size.y * Constants::TILE_SIZE * 0.5f });
	
	physicsBody = Box2DWorldManager::getInstance().createKinematicBody(barPosition);

	if (physicsBody) {
		this->physicsBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);

		b2PolygonShape mainShape;
		mainShape.SetAsBox(barSize.x * 0.5f, barSize.y * 0.5f, b2Vec2(barSize.x * 0.5f, 0), 0.0f);
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &mainShape;
		fixtureDef.isSensor = true;

		physicsBody->CreateFixture(&fixtureDef);

		for (b2Fixture* fixture = physicsBody->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
			b2Filter filter = fixture->GetFilterData();
			filter.maskBits = static_cast<uint16>(ObjectCategory::INTERACTIVE);
			filter.categoryBits = static_cast<uint16>(ObjectCategory::CHARACTER);
			fixture->SetFilterData(filter);
		}
	}
}

FireBar::~FireBar() {
	if (physicsBody) {
		Box2DWorldManager::getInstance().destroyBody(physicsBody);
		physicsBody = nullptr;
	}
}

void FireBar::update(float deltaTime) {
	aniTimer += deltaTime;
	if (aniTimer >= aniSpeed) {
		currentFrame = (currentFrame + 1) % frameData.size();
		aniTimer = 0.0f;
	}

	timer += deltaTime;

	float period = 3.0f;

	physicsBody->SetTransform(physicsBody->GetPosition(), 2.0f * PI / 3.0f * timer);

	if (timer >= period) {
		triggering = !triggering;
		timer = 0;
		if (triggering) {
			for (b2Fixture* fixture = physicsBody->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
				b2Filter filter = fixture->GetFilterData();
				filter.maskBits = static_cast<uint16>(ObjectCategory::INTERACTIVE);
				filter.categoryBits = static_cast<uint16>(ObjectCategory::CHARACTER);
				fixture->SetFilterData(filter);
			}
		}
		else {
			currentFrame = 0;
			aniTimer = 0.0f;
			for (b2Fixture* fixture = physicsBody->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
				b2Filter filter = fixture->GetFilterData();
				filter.maskBits = 0;
				fixture->SetFilterData(filter);
			}
		}
	}
}

void FireBar::draw() {

}

void FireBar::draw(int) {
	if (triggering) {
		Vector2 centerPosition = { hitBox.x + size.x * Constants::TILE_SIZE * 0.5f, hitBox.y + size.y * Constants::TILE_SIZE * 0.5f };
		Rectangle source = frameData[currentFrame];
		Rectangle dest = {
			centerPosition.x,
			centerPosition.y,
			0.25f * Constants::TILE_SIZE,
			2.5f * Constants::TILE_SIZE
		};
		Vector2 origin = {
			dest.width * 0.5f,
			dest.height
		};
		DrawTexturePro(TextureManager::interactiveTextures, source, dest, origin, (PI / 2.0f + 2.0f * PI / 3.0f * timer) * 180.0f / PI, WHITE);
	}
}

void FireBar::onCollision(std::shared_ptr<Object> other, Direction dir) {
	
}

std::vector<Rectangle> FireBar::getHitBox() const {
	return {hitBox};
}

ObjectType FireBar::getObjectType() const {
	return InteractiveType::FIRE_BAR;
}

Vector2 FireBar::getSize() const {
	return size;
}