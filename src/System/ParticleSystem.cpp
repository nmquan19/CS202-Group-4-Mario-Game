#include "../../include/System/ParticleSystem.h"
#include "../../include/Game/GameContext.h"
#include <iostream>

Particle::Particle(Vector2 pos, Vector2 velo, Vector2 acce, float span) : position(pos), velocity(velo), acceleration(acce), lifeSpan(span) {
	this->age = 0;
}

Particle::~Particle() {}

void Particle::update(float deltaTime) {
	age += deltaTime;

	velocity.x += acceleration.x * deltaTime;
	velocity.y += acceleration.y * deltaTime;
	position.x += velocity.x * deltaTime;
	position.y += velocity.y * deltaTime;
}

float Particle::lifeLeft() const {
	return lifeSpan - age;
}

Vector2 Particle::getPosition() const {
	return position;
}

ParticleSystem::ParticleSystem(Vector2 pos, Vector2 velo, Vector2 acce, float life, float speed, float elapsed, const char* texture) : 
	startPos(pos), velocity(velo), acceleration(acce), lifeSpan(life), spawnSpeed(speed), timeElapsed(elapsed) {
	screen = { (float)GetScreenWidth(), (float)GetScreenHeight() };
	sprite = LoadTexture(texture);
}

ParticleSystem::~ParticleSystem() {
	UnloadTexture(sprite);
}

void ParticleSystem::update(float deltaTime) {
	timeElapsed += deltaTime;

	while (timeElapsed >= spawnSpeed) {
		timeElapsed -= spawnSpeed;
		Particle* p = new Particle(startPos, velocity, acceleration, lifeSpan);
		particles.push_back(p);
	}

	std::vector<Particle*>::iterator it;
	it = particles.begin();
	while (!particles.empty() && it != particles.end()) {
		(*it)->update(deltaTime);

		if ((*it)->lifeLeft() <= 0) {
			delete (*it);
			it = particles.erase(it); 
		}
		else {
			it++;
		}
	}
}

void ParticleSystem::draw() {
	std::vector<Particle*>::iterator it;
	for (it = particles.begin(); it != particles.end(); it++) {
		DrawTextureEx(sprite, GetWorldToScreen2D((*it)->getPosition(), GameContext::getInstance().camera), 0.0f, 0.1f, WHITE);
		Vector2 pos = GetWorldToScreen2D((*it)->getPosition(), GameContext::getInstance().camera);
		//std::cout << pos.x << " " << pos.y << "\n";
	}
}