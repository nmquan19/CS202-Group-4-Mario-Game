#pragma once
#include <raylib.h>
#include <vector>
#include "Interface.h"

class ParticleEffect;

class Particle : public IUpdatable {
public:
	Particle(Vector2, Vector2, Vector2, float);
	~Particle();
	void update(float deltaTime) override;
	float lifeLeft() const;
	Vector2 getPosition() const;
private:
	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;
	float age;
	float lifeSpan;
};

class ParticleSystem : public IUpdatable, public IDrawable {
public:
	static ParticleSystem& getInstance();
	~ParticleSystem();

	ParticleSystem(const ParticleSystem&) = delete;
	void operator=(const ParticleSystem&) = delete;

	void update(float deltaTime) override;
	void draw();

	void addEffect(ParticleEffect*);
	void cleanup();
private:
	ParticleSystem() = default;
	static ParticleSystem* instance;
	std::vector<ParticleEffect*> particleEffects;
};

class ParticleEffect : public IUpdatable, public IDrawable {
public:
	ParticleEffect(Vector2, Vector2, Vector2, Vector2, Vector2, float, float, const char*);
	ParticleEffect(Vector2, Vector2, Vector2, Vector2, Vector2, float, float, Texture2D);
	ParticleEffect(const ParticleEffect&);
	virtual ~ParticleEffect();

	void update(float deltaTime) override = 0;
	void draw() override = 0;
public:
	Vector2 startPos;
	Vector2 particleBounds;
	Vector2 minVelocity;
	Vector2 maxVelocity;
	Vector2 acceleration;
	float lifeSpan;
	float spawnSpeed;
	bool isActive() const { return active; }
protected:
	Texture2D sprite;
	float timeElapsed;
	std::vector<Particle*> particles;
	bool active = true;
};

class BrokenBlockEffect : public ParticleEffect {
public:
	BrokenBlockEffect(Vector2 pos, Vector2 bounds, Vector2 minVelo, Vector2 maxVelo, Vector2 accelerate, float lifeSpan, float spawnSpeed, const char* texture);
	BrokenBlockEffect(Vector2 pos, Vector2 bounds, Vector2 minVelo, Vector2 maxVelo, Vector2 accelerate, float lifeSpan, float spawnSpeed, Texture2D texture);

	void update(float deltaTime) override;
	void draw() override;
};