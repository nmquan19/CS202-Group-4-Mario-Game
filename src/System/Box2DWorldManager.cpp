#include <iostream>
#include "../../include/System/Box2DWorldManager.h"
#include "../../include/Characters/Character.h"

Box2DWorldManager* Box2DWorldManager::instance = nullptr;

Box2DWorldManager& Box2DWorldManager::getInstance() {
	if (!instance) {
		instance = new Box2DWorldManager();
	}
	return *instance;
}

void Box2DWorldManager::cleanup() {
	delete instance;
	instance = nullptr;
}

void Box2DWorldManager::initialize(Vector2 gravity) {
	b2Vec2 b2Gravity = raylibToB2(gravity);
	world = std::make_unique<b2World>(b2Gravity);
	world->SetContactListener(this);
}

void Box2DWorldManager::step(float deltaTime) {
	if (world) {
		int32 veloIter = 8;
		int32 posIter = 3;
		world->Step(deltaTime, veloIter, posIter);
	}
}

b2Body* Box2DWorldManager::createRectangleBody(Vector2 pos, Vector2 hitboxSize) {
	b2Vec2 b2_size = raylibToB2(hitboxSize);
	b2Vec2 b2_pos = raylibToB2(pos);
	b2_pos.x += b2_size.x / 2;
	b2_pos.y += b2_size.y / 2;

	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position = b2_pos;
	bodyDef.fixedRotation = true;
	bodyDef.allowSleep = true;
	bodyDef.awake = true;

	b2Body* body = world->CreateBody(&bodyDef);

	b2PolygonShape mainShape;
	mainShape.SetAsBox(b2_size.x / 2, b2_size.y / 2);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &mainShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;
	fixtureDef.restitution = 0.0f;

	body->CreateFixture(&fixtureDef);

	attachSensors(body, hitboxSize);

	return body;
}

b2Body* Box2DWorldManager::createDynamicRectangleBody(Vector2 pos, Vector2 hitboxSize) {
	b2Vec2 b2_size = raylibToB2(hitboxSize);
	b2Vec2 b2_pos = raylibToB2(pos);
	b2_pos.x += b2_size.x / 2;
	b2_pos.y += b2_size.y / 2;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = b2_pos;
	bodyDef.fixedRotation = true;
	bodyDef.allowSleep = true;
	bodyDef.awake = true;

	b2Body* body = world->CreateBody(&bodyDef);

	b2PolygonShape mainShape;
	mainShape.SetAsBox(b2_size.x / 2, b2_size.y / 2);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &mainShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;
	fixtureDef.restitution = 0.0f;

	body->CreateFixture(&fixtureDef);

	attachSensors(body, hitboxSize);

	return body;
}

void Box2DWorldManager::attachRectangleFixtures(b2Body* body, Vector2 pos, Vector2 hitboxSize) {
	b2Vec2 b2_size = raylibToB2(hitboxSize);

	b2PolygonShape mainShape;
	mainShape.SetAsBox(b2_size.x / 2, b2_size.y / 2);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &mainShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;
	fixtureDef.restitution = 0.0f;

	body->CreateFixture(&fixtureDef);
}


void Box2DWorldManager::attachCapsuleFixtures(b2Body* body, Vector2 pos, Vector2 hitboxSize) {
	b2Vec2 b2_size = raylibToB2(hitboxSize);
	float radius = b2_size.x / 2;
	float rectHeight = b2_size.y - b2_size.x;

	b2CircleShape topShape;
	topShape.m_radius = radius;
	topShape.m_p = b2Vec2(0, -rectHeight * 0.5f);
	b2FixtureDef topFixture;
	topFixture.shape = &topShape;
	topFixture.density = 0.5f;
	body->CreateFixture(&topFixture);

	b2CircleShape bottomShape;
	bottomShape.m_radius = radius;
	bottomShape.m_p = b2Vec2(0, rectHeight * 0.5f);
	b2FixtureDef bottomFixture;
	bottomFixture.shape = &bottomShape;
	bottomFixture.density = 0.5f;
	body->CreateFixture(&bottomFixture);

	if (rectHeight > 0) {
		b2PolygonShape rectShape;
		rectShape.SetAsBox(radius, rectHeight * 0.5f);
		b2FixtureDef rectFixture;
		rectFixture.shape = &rectShape;
		rectFixture.density = 0.5f;
		body->CreateFixture(&rectFixture);
	}
}

void Box2DWorldManager::attachSphericalFixture(b2Body* body, Vector2 pos, Vector2 hitboxSize) {
	b2Vec2 b2_size = raylibToB2(hitboxSize);
	float radius = b2_size.x / 2;

	b2CircleShape mainShape;
	mainShape.m_radius = radius;
	mainShape.m_p = b2Vec2(0, 0);
	b2FixtureDef mainFixture;
	mainFixture.shape = &mainShape;
	mainFixture.density = 0.5f;
	mainFixture.friction = 0.3f;
	mainFixture.restitution = 0.9f;
	body->CreateFixture(&mainFixture);
}

void Box2DWorldManager::attachSensors(b2Body* body, Vector2 hitboxSize) {
	// Sensor
	float sensorThickness = 2.0f;
	float sensorWidth = hitboxSize.x * 0.6f;
	float sensorHeight = hitboxSize.y * 0.6f;

	// Bottom
	b2PolygonShape botSensor;
	botSensor.SetAsBox(raylibToB2(sensorWidth * 0.5f), raylibToB2(sensorThickness * 0.5f),
		b2Vec2(0, raylibToB2(hitboxSize.y * 0.5f + sensorThickness * 0.5f)), 0);
	b2FixtureDef botFixture;
	botFixture.shape = &botSensor;
	botFixture.isSensor = true;
	botFixture.userData.pointer = 1; //bot sensor id

	body->CreateFixture(&botFixture);

	// Top
	b2PolygonShape topSensor;
	topSensor.SetAsBox(raylibToB2(sensorWidth * 0.5f), raylibToB2(sensorThickness * 0.5f),
		b2Vec2(0, raylibToB2(-hitboxSize.y * 0.5f - sensorThickness * 0.5f)), 0);
	b2FixtureDef topFixture;
	topFixture.shape = &topSensor;
	topFixture.isSensor = true;
	topFixture.userData.pointer = 2; //top sensor id;

	body->CreateFixture(&topFixture);

	// Left
	b2PolygonShape leftSensor;
	leftSensor.SetAsBox(raylibToB2(sensorThickness * 0.5f), raylibToB2(sensorHeight * 0.5f),
		b2Vec2(raylibToB2(-hitboxSize.x * 0.5f - sensorThickness * 0.5f), 0), 0);
	b2FixtureDef leftFixture;
	leftFixture.shape = &leftSensor;
	leftFixture.isSensor = true;
	leftFixture.userData.pointer = 3; //left sensor id

	body->CreateFixture(&leftFixture);

	// Right
	b2PolygonShape rightSensor;
	rightSensor.SetAsBox(raylibToB2(sensorThickness * 0.5f), raylibToB2(sensorHeight * 0.5f),
		b2Vec2(raylibToB2(hitboxSize.x * 0.5f + sensorThickness * 0.5f), 0), 0);
	b2FixtureDef rightFixture;
	rightFixture.shape = &rightSensor;
	rightFixture.isSensor = true;
	rightFixture.userData.pointer = 4; //right sensor id
	body->CreateFixture(&rightFixture);
}

void Box2DWorldManager::destroyBody(b2Body* body) {
	if (world && body) {
		world->DestroyBody(body);
	}
}

void Box2DWorldManager::destroyJoint(b2Joint* joint) {
	if (world && joint) {
		world->DestroyJoint(joint);
	}
}

b2Body* Box2DWorldManager::createKinematicBody(b2Vec2 b2_pos) {
	if (!world) return nullptr;

	b2BodyDef bodyDef;
	bodyDef.type = b2_kinematicBody;
	bodyDef.position = b2_pos;
	bodyDef.allowSleep = false;
	bodyDef.awake = true;

	b2Body* body = world->CreateBody(&bodyDef);
	return body;
}

b2Body* Box2DWorldManager::createDynamicBody(b2Vec2 b2_pos) {
	if (!world) return nullptr;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = b2_pos;
	/*bodyDef.allowSleep = true;
	bodyDef.awake = true;*/

	b2Body* body = world->CreateBody(&bodyDef);
	return body;
}

b2Body* Box2DWorldManager::createCharacterBody(Vector2 pos, Vector2 hitboxSize) {
	if (!world) return nullptr;

	b2Vec2 b2_size = raylibToB2(hitboxSize);
	b2Vec2 b2_pos = raylibToB2(pos);
	b2_pos.x += b2_size.x / 2;
	b2_pos.y += b2_size.y / 2;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = b2_pos;
	bodyDef.fixedRotation = true;
	bodyDef.allowSleep = false;
	bodyDef.awake = true;

	b2Body* body = world->CreateBody(&bodyDef);
	
	attachCapsuleFixtures(body, pos, hitboxSize);
	attachSensors(body, hitboxSize);

	return body;
}

b2Body* Box2DWorldManager::createProjectileBody(Vector2 pos, Vector2 hitboxSize) {
	if (!world) return nullptr;

	b2Vec2 b2_size = raylibToB2(hitboxSize);
	b2Vec2 b2_pos = raylibToB2(pos);
	b2_pos.x += b2_size.x * 0.5f;
	b2_pos.y += b2_size.y * 0.5f;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = b2_pos;
	bodyDef.fixedRotation = true;
	bodyDef.allowSleep = true;
	bodyDef.awake = true;

	b2Body* body = world->CreateBody(&bodyDef);

	attachSphericalFixture(body, pos, hitboxSize);
	attachSensors(body, hitboxSize);

	return body;
}

b2Body* Box2DWorldManager::createBlockBody(Vector2 pos, Vector2 hitboxSize) {

	b2Vec2 b2_size = raylibToB2(hitboxSize);
	b2Vec2 b2_pos = raylibToB2(pos);
	b2_pos.x += b2_size.x / 2;
	b2_pos.y += b2_size.y / 2;

	b2Body* body = createKinematicBody(b2_pos);
	if (!body) {
		return nullptr;
	}

	attachRectangleFixtures(body, pos, hitboxSize);
	attachSensors(body, hitboxSize);

	return body;
}

b2Body* Box2DWorldManager::createEnemyBody(Vector2 pos, Vector2 hitboxSize) {
	if (!world) return nullptr;

	b2Vec2 b2_size = raylibToB2(hitboxSize);
	b2Vec2 b2_pos = raylibToB2(pos);
	b2_pos.x += b2_size.x / 2;
	b2_pos.y += b2_size.y / 2;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = b2_pos;
	bodyDef.fixedRotation = true;
	bodyDef.allowSleep = true;
	bodyDef.awake = true;

	b2Body* body = world->CreateBody(&bodyDef);

	attachCapsuleFixtures(body, pos, hitboxSize);
	attachSensors(body, hitboxSize);
	
	return body;
}

b2Body* Box2DWorldManager::createItemStaticBody(Vector2 pos, Vector2 hitboxSize) {
	if (!world) return nullptr;

	b2Vec2 b2_size = raylibToB2(hitboxSize);
	b2Vec2 b2_pos = raylibToB2(pos);
	b2_pos.x += b2_size.x / 2;
	b2_pos.y += b2_size.y / 2;

	b2BodyDef bodyDef;
	bodyDef.type = b2_kinematicBody;
	bodyDef.position = b2_pos;
	bodyDef.fixedRotation = true;
	bodyDef.allowSleep = true;
	bodyDef.awake = true;

	b2Body* body = world->CreateBody(&bodyDef);

	b2Vec2 b2_size2 = raylibToB2(hitboxSize);
	
	b2PolygonShape mainShape;
	mainShape.SetAsBox(b2_size2.x / 2, b2_size2.y / 2);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &mainShape;
	fixtureDef.isSensor = true;

	body->CreateFixture(&fixtureDef);

	return body;
}
b2Body* Box2DWorldManager::createItemDynamicBody(Vector2 pos, Vector2 hitboxSize) {
	if (!world) return nullptr;

	b2Vec2 b2_size = raylibToB2(hitboxSize);
	b2Vec2 b2_pos = raylibToB2(pos);
	b2_pos.x += b2_size.x / 2;
	b2_pos.y += b2_size.y / 2;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = b2_pos;
	bodyDef.fixedRotation = true;
	bodyDef.allowSleep = true;
	bodyDef.awake = true;

	b2Body* body = world->CreateBody(&bodyDef);

	attachCapsuleFixtures(body, pos, hitboxSize);
	attachSensors(body, hitboxSize);

	return body;
}

void Box2DWorldManager::BeginContact(b2Contact* contact) {
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	b2Body* bodyA = fixtureA->GetBody();
	b2Body* bodyB = fixtureB->GetBody();

	Object* objA = reinterpret_cast<Object*>(bodyA->GetUserData().pointer);
	Object* objB = reinterpret_cast<Object*>(bodyB->GetUserData().pointer);

	if (!objA || !objB) return;

	int sensorIdA = static_cast<int>(fixtureA->GetUserData().pointer);
	int sensorIdB = static_cast<int>(fixtureB->GetUserData().pointer);

	Direction directAtoB = Direction::NONE;
	Direction directBtoA = Direction::NONE;

	if (fixtureA->IsSensor() && sensorIdA > 0) {
		switch (sensorIdA) {
		case 1: 
			directAtoB = Direction::DOWN;
			directBtoA = Direction::UP;
			break;
		case 2:
			directAtoB = Direction::UP;
			directBtoA = Direction::DOWN;
			break;
		case 3:
			directAtoB = Direction::LEFT;
			directBtoA = Direction::RIGHT;
			break;
		case 4:
			directAtoB = Direction::RIGHT;
			directBtoA = Direction::LEFT;
			break;
		case 5:
			directAtoB = Direction::IN;
			directBtoA = Direction::OUT;
			break;
		}
	}
	else if (fixtureB->IsSensor() && sensorIdB > 0) {
		switch (sensorIdB) {
		case 1:
			directBtoA = Direction::DOWN;
			directAtoB = Direction::UP;
			break;
		case 2:
			directBtoA = Direction::UP;
			directAtoB = Direction::DOWN;
			break;
		case 3:
			directBtoA = Direction::LEFT;
			directAtoB = Direction::RIGHT;
			break;
		case 4:
			directBtoA = Direction::RIGHT;
			directAtoB = Direction::LEFT;
			break;
		case 5:
			directBtoA = Direction::IN;
			directAtoB = Direction::OUT;
			break;
		}
	}
	else {
		b2WorldManifold worldManifold;
		contact->GetWorldManifold(&worldManifold);
		b2Vec2 normal = worldManifold.normal;

		if (abs(normal.y) > abs(normal.x)) {
			if (normal.y > 0) {
				directAtoB = Direction::DOWN;
				directBtoA = Direction::UP;
			}
			else {
				directAtoB = Direction::UP;
				directBtoA = Direction::DOWN;
			}
		} 
		else {
			if (normal.x > 0) {
				directAtoB = Direction::RIGHT;
				directBtoA = Direction::LEFT;
			}
			else {
				directAtoB = Direction::LEFT;
				directBtoA = Direction::RIGHT;
			}
		}
	}

	auto sharedA = std::shared_ptr<Object>(objA, [](Object*){});
	auto sharedB = std::shared_ptr<Object>(objB, [](Object*){});

	sharedA->onCollision(sharedB, directAtoB);
	sharedB->onCollision(sharedA, directBtoA);
}

void Box2DWorldManager::EndContact(b2Contact* contact) {
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	b2Body* bodyA = fixtureA->GetBody();
	b2Body* bodyB = fixtureB->GetBody();

	Object* objA = reinterpret_cast<Object*>(bodyA->GetUserData().pointer);
	Object* objB = reinterpret_cast<Object*>(bodyB->GetUserData().pointer);

	if (!objA || !objB) return;

	int sensorIdA = static_cast<int>(fixtureA->GetUserData().pointer);
	int sensorIdB = static_cast<int>(fixtureB->GetUserData().pointer);

	// Check if this is a bottom sensor leaving contact with a block
	if (fixtureA->IsSensor() && sensorIdA == 1) { // Bottom sensor of objA
		if (objB->getObjectCategory() == ObjectCategory::BLOCK || objB->getObjectCategory() == ObjectCategory::INTERACTIVE) {
			if (auto character = dynamic_cast<Character*>(objA)) {
				character->removeGroundContact();
			}
		}
	}
	else if (fixtureB->IsSensor() && sensorIdB == 1) { // Bottom sensor of objB
		if (objA->getObjectCategory() == ObjectCategory::BLOCK || objA->getObjectCategory() == ObjectCategory::INTERACTIVE) {
			if (auto character = dynamic_cast<Character*>(objB)) {
				character->removeGroundContact();
			}
		}
	}

	if (fixtureA->IsSensor() && sensorIdA == 1) { // Bottom sensor of objA
		if (objB->getObjectCategory() == ObjectCategory::INTERACTIVE) {
			ObjectType objectType = objB->getObjectType();
			if (auto* interactiveType = std::get_if<InteractiveType>(&objectType)) {
				switch (*interactiveType) {
				case InteractiveType::MOVING_PLATFORM:
					if (auto character = dynamic_cast<Character*>(objA)) {
						// character->removePlatformContact();
						character->setPlatform(nullptr);
					}
					break;
				}
			}
		}
	}
	else if (fixtureB->IsSensor() && sensorIdB == 1) { // Bottom sensor of objB
		if (objA->getObjectCategory() == ObjectCategory::INTERACTIVE) {
			ObjectType objectType = objA->getObjectType();
			if (auto* interactiveType = std::get_if<InteractiveType>(&objectType)) {
				switch (*interactiveType) {
				case InteractiveType::MOVING_PLATFORM:
					if (auto character = dynamic_cast<Character*>(objB)) {
						// character->removePlatformContact();
						character->setPlatform(nullptr);
					}
					break;
				}
			}
		}
	}
}

void Box2DWorldManager::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {
	
}

Vector2 Box2DWorldManager::b2ToRaylib(const b2Vec2& vec) {
	return { vec.x  * Constants::TILE_SIZE, vec.y  * Constants::TILE_SIZE };
}

b2Vec2 Box2DWorldManager::raylibToB2(const Vector2& vec) {
	return { vec.x / Constants::TILE_SIZE, vec.y / Constants::TILE_SIZE };
}

float Box2DWorldManager::b2ToRaylib(float value) {
	return value * Constants::TILE_SIZE;
}

float Box2DWorldManager::raylibToB2(float value) {
	return value / Constants::TILE_SIZE;
}

void Box2DWorldManager::drawDebugBodies() {
	if (!world || !debugDrawEnabled) return;

	for (b2Body* body = world->GetBodyList(); body; body = body->GetNext()) {
		drawBody(body);
	}
}

void Box2DWorldManager::drawBody(b2Body* body) {
	if (!body) return;

	b2Vec2 bodyPos = body->GetPosition();
	Vector2 worldPos = b2ToRaylib(bodyPos);

	for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
		b2Shape* shape = fixture->GetShape();
		
		// Get the sensor ID to determine color
		int sensorId = static_cast<int>(fixture->GetUserData().pointer);
		Color drawColor = getFixtureColor(fixture, body, sensorId);
		
		// Handle different shape types
		switch (shape->GetType()) {
		case b2Shape::e_polygon:
			drawPolygonShape(body, fixture, (b2PolygonShape*)shape, drawColor);
			break;
		case b2Shape::e_circle:
			drawCircleShape(body, fixture, (b2CircleShape*)shape, drawColor);
			break;
		case b2Shape::e_edge:
			drawEdgeShape(body, fixture, (b2EdgeShape*)shape, drawColor);
			break;
		case b2Shape::e_chain:
			drawChainShape(body, fixture, (b2ChainShape*)shape, drawColor);
			break;
		default:
			// Draw a simple cross for unknown shapes
			DrawLineV({ worldPos.x - 5, worldPos.y }, { worldPos.x + 5, worldPos.y }, drawColor);
			DrawLineV({ worldPos.x, worldPos.y - 5 }, { worldPos.x, worldPos.y + 5 }, drawColor);
			break;
		}
	}
	
	// Draw body center point
	DrawCircleV(worldPos, 3, RED);
}

Color Box2DWorldManager::getFixtureColor(b2Fixture* fixture, b2Body* body, int sensorId) {
	if (fixture->IsSensor()) {
		// Different colors for different sensors
		switch (sensorId) {
		case 1: return BLUE;     // Bottom sensor
		case 2: return RED;      // Top sensor  
		case 3: return GREEN;    // Left sensor
		case 4: return ORANGE;   // Right sensor
		default: return PURPLE;  // Unknown sensor
		}
	} else {
		// Main body color based on body type
		switch (body->GetType()) {
		case b2_staticBody: return GRAY;
		case b2_dynamicBody: return YELLOW;
		case b2_kinematicBody: return PINK;
		default: return WHITE;
		}
	}
}

void Box2DWorldManager::drawPolygonShape(b2Body* body, b2Fixture* fixture, b2PolygonShape* polygon, Color drawColor) {
	// Draw the polygon outline
	Vector2 vertices[b2_maxPolygonVertices];
	int vertexCount = polygon->m_count;
	
	for (int i = 0; i < vertexCount; i++) {
		b2Vec2 vertex = body->GetWorldPoint(polygon->m_vertices[i]);
		vertices[i] = b2ToRaylib(vertex);
	}
	
	// Draw lines between vertices
	for (int i = 0; i < vertexCount; i++) {
		int next = (i + 1) % vertexCount;
		DrawLineV(vertices[i], vertices[next], drawColor);
	}
	
	// Draw filled rectangle for sensors with transparency
	if (fixture->IsSensor()) {
		Color fillColor = drawColor;
		fillColor.a = 64; // Semi-transparent
		
		// For box shapes, draw a filled rectangle
		if (vertexCount == 4) {
			float minX = vertices[0].x, maxX = vertices[0].x;
			float minY = vertices[0].y, maxY = vertices[0].y;
			
			for (int i = 1; i < 4; i++) {
				if (vertices[i].x < minX) minX = vertices[i].x;
				if (vertices[i].x > maxX) maxX = vertices[i].x;
				if (vertices[i].y < minY) minY = vertices[i].y;
				if (vertices[i].y > maxY) maxY = vertices[i].y;
			}
			
			DrawRectangle((int)minX, (int)minY, (int)(maxX - minX), (int)(maxY - minY), fillColor);
		}
	}
}

void Box2DWorldManager::drawCircleShape(b2Body* body, b2Fixture* fixture, b2CircleShape* circle, Color drawColor) {
	b2Vec2 center = body->GetWorldPoint(circle->m_p);
	Vector2 worldCenter = b2ToRaylib(center);
	float radius = b2ToRaylib(circle->m_radius);
	
	// Draw circle outline
	DrawCircleLinesV(worldCenter, radius, drawColor);
	
	// Draw filled circle for sensors with transparency
	if (fixture->IsSensor()) {
		Color fillColor = drawColor;
		fillColor.a = 64; // Semi-transparent
		DrawCircleV(worldCenter, radius, fillColor);
	}
	
	// Draw center point
	DrawCircleV(worldCenter, 2, drawColor);
}

void Box2DWorldManager::drawEdgeShape(b2Body* body, b2Fixture* fixture, b2EdgeShape* edge, Color drawColor) {
	b2Vec2 v1 = body->GetWorldPoint(edge->m_vertex1);
	b2Vec2 v2 = body->GetWorldPoint(edge->m_vertex2);
	
	Vector2 worldV1 = b2ToRaylib(v1);
	Vector2 worldV2 = b2ToRaylib(v2);
	
	DrawLineV(worldV1, worldV2, drawColor);
	
	// Draw end points
	DrawCircleV(worldV1, 3, drawColor);
	DrawCircleV(worldV2, 3, drawColor);
}

void Box2DWorldManager::drawChainShape(b2Body* body, b2Fixture* fixture, b2ChainShape* chain, Color drawColor) {
	for (int i = 0; i < chain->m_count - 1; i++) {
		b2Vec2 v1 = body->GetWorldPoint(chain->m_vertices[i]);
		b2Vec2 v2 = body->GetWorldPoint(chain->m_vertices[i + 1]);
		
		Vector2 worldV1 = b2ToRaylib(v1);
		Vector2 worldV2 = b2ToRaylib(v2);
		
		DrawLineV(worldV1, worldV2, drawColor);
	}
	
	// Draw vertices
	for (int i = 0; i < chain->m_count; i++) {
		b2Vec2 vertex = body->GetWorldPoint(chain->m_vertices[i]);
		Vector2 worldVertex = b2ToRaylib(vertex);
		DrawCircleV(worldVertex, 2, drawColor);
	}
}


struct SimpleRayCastCallback : public b2RayCastCallback {
	b2Vec2 hitPoint;
	b2Body* hitBody = nullptr;
	bool hit = false;

	float ReportFixture(b2Fixture* fixture,
		const b2Vec2& point,
		const b2Vec2& normal,
		float fraction) override {
		hitPoint = point;
		hitBody = fixture->GetBody();
		hit = true;
		return fraction; 
	}
};

bool Box2DWorldManager::raycast(const Vector2& start,
	const Vector2& end,
	Vector2& outHitPoint,
	b2Body** outBody) {
	if (!world) return false;

	SimpleRayCastCallback callback;
	world->RayCast(&callback, raylibToB2(start), raylibToB2(end));

	if (callback.hit) {
		outHitPoint = b2ToRaylib(callback.hitPoint);
		if (outBody) {
			*outBody = callback.hitBody;
		}
		return true;
	}
	return false;
}

b2Body* Box2DWorldManager::createLaserBeamBody(Vector2 pos, float length, float thickness, float angle) {
	if (!world) return nullptr;

	b2BodyDef bodyDef;
	bodyDef.type = b2_kinematicBody;
	bodyDef.position = raylibToB2(pos);
	bodyDef.angle = angle;

	b2Body* body = world->CreateBody(&bodyDef);

	b2PolygonShape beamShape;
	beamShape.SetAsBox(
		raylibToB2(length * 0.5f),  // half length
		raylibToB2(thickness * 0.5f),
		b2Vec2(raylibToB2(length * 0.5f), 0.0f), // offset to match start point
		0.0f
	);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &beamShape;
	fixtureDef.isSensor = true;

	b2Filter filter;
	filter.categoryBits = static_cast<uint16>(ObjectCategory::PROJECTILE);
	filter.maskBits =
		static_cast<uint16>(ObjectCategory::CHARACTER) |
		static_cast<uint16>(ObjectCategory::BLOCK) |
		static_cast<uint16>(ObjectCategory::INTERACTIVE);
	fixtureDef.filter = filter;

	body->CreateFixture(&fixtureDef);
	return body;
}
