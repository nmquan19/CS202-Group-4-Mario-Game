#pragma once
#include <raylib.h>
#include <vector>
#include "Interface.h"

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
	ParticleSystem(Vector2, Vector2, Vector2, float, float, float, const char*);
	~ParticleSystem();
	
	void update(float deltaTime) override;
	void draw() override;

public:
	Vector2 startPos;
	Vector2 velocity;
	Vector2 acceleration;
	float lifeSpan;
	float spawnSpeed;
private:
	Texture2D sprite;
	Vector2 screen;
	float timeElapsed;
	std::vector<Particle*> particles;
};