#pragma once

#include <box2d/box2d.h>
#include <raylib.h>
#include <memory>
#include "../Objects/ObjectFactory.h"
#include "../System/Constant.h"

class Box2DWorldManager : public b2ContactListener {
public:
	static Box2DWorldManager& getInstance();
	static void cleanup();

	void initialize(Vector2 gravity = { 0.0f, Constants::GRAVITY });
	void step(float deltaTime);

	b2Body* createCharacterBody(Vector2 pos, Vector2 hitboxSize);
	b2Body* createBlockBody(Vector2 pos, Vector2 hitboxSize);
	b2Body* createEnemyBody(Vector2 pos, Vector2 hitboxSize);
	b2Body* createItemBody(Vector2 pos, Vector2 hitboxSize);

	void BeginContact(b2Contact* contact) override;
	void EndContact(b2Contact* contact) override;
	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;

	void destroyBody(b2Body* body);
	
	void drawDebugBodies();
	void drawBody(b2Body* body);
	void setDebugDraw(bool enabled) { debugDrawEnabled = enabled; }
	bool isDebugDrawEnabled() const { return debugDrawEnabled; }

	static Vector2 b2ToRaylib(const b2Vec2& vec);
	static b2Vec2 raylibToB2(const Vector2& vec);
	static float b2ToRaylib(const float value);
	static float raylibToB2(const float value);

private:
	b2Body* createRectangleBody(Vector2 pos, Vector2 hitboxSize);
	b2Body* createCapsuleBody(Vector2 pos, Vector2 hitboxSize);
	void attachSensors(b2Body* body, Vector2 hitboxSize);

private:
	Box2DWorldManager() = default;
	~Box2DWorldManager() = default;
	static Box2DWorldManager* instance;

	std::unique_ptr<b2World> world;
	bool debugDrawEnabled = false;  // Debug drawing toggle
	
	// Debug drawing helper methods
	Color getFixtureColor(b2Fixture* fixture, b2Body* body, int sensorId);
	void drawPolygonShape(b2Body* body, b2Fixture* fixture, b2PolygonShape* polygon, Color drawColor);
	void drawCircleShape(b2Body* body, b2Fixture* fixture, b2CircleShape* circle, Color drawColor);
	void drawEdgeShape(b2Body* body, b2Fixture* fixture, b2EdgeShape* edge, Color drawColor);
	void drawChainShape(b2Body* body, b2Fixture* fixture, b2ChainShape* chain, Color drawColor);
};