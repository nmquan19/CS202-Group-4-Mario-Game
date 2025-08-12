#include "../../include/System/ParticleSystem.h"
#include "../../include/Game/GameContext.h"
#include <iostream>

float GetRandom(int decimal);

// Particle
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

// Particle System
ParticleSystem* ParticleSystem::instance = nullptr;
ParticleSystem& ParticleSystem::getInstance() {
	if (!instance) {
		instance = new ParticleSystem();
	}
	return *instance;
}

ParticleSystem::~ParticleSystem() {
	std::vector<ParticleEffect*>::iterator it;
	it = particleEffects.begin();
	while (!particleEffects.empty()) {
		delete (*it);
		it = particleEffects.erase(it);
	}
	particleEffects.clear();
}

void ParticleSystem::update(float deltaTime) {
	std::vector<ParticleEffect*>::iterator it;
	it = particleEffects.begin();
	while (!particleEffects.empty() && it != particleEffects.end()) {
		if ((*it)->isActive()) {
			(*it)->update(deltaTime);
			it++;
		}
		else it = particleEffects.erase(it);
	}
}

void ParticleSystem::draw() {
	for (auto& effect : particleEffects) {
		if (effect && effect->isActive()) effect->draw();
	}
}

void ParticleSystem::addEffect(ParticleEffect* effect) {
	particleEffects.push_back(effect);
}

void ParticleSystem::cleanup() {
	if (instance) {
		delete instance;
		instance = nullptr;
	}
}

// Base Particle Effect
ParticleEffect::ParticleEffect(Vector2 pos, Vector2 bounds, Vector2 minVelo, Vector2 maxVelo, Vector2 accelerate, float lifeSpan, float spawnSpeed, const char* texture) :
	startPos(pos), acceleration(accelerate), particleBounds(bounds), minVelocity(minVelo), maxVelocity(maxVelo), lifeSpan(lifeSpan), spawnSpeed(spawnSpeed), timeElapsed(0) {
	sprite = LoadTexture(texture);
}

ParticleEffect::ParticleEffect(Vector2 pos, Vector2 bounds, Vector2 minVelo, Vector2 maxVelo, Vector2 accelerate, float lifeSpan, float spawnSpeed, Texture2D texture) :
	startPos(pos), acceleration(accelerate), particleBounds(bounds), minVelocity(minVelo), maxVelocity(maxVelo), lifeSpan(lifeSpan), spawnSpeed(spawnSpeed), timeElapsed(0) {
	sprite = texture;
}

ParticleEffect::ParticleEffect(const ParticleEffect& p) {
	this->startPos = p.startPos;
	this->particleBounds = p.particleBounds;
	this->minVelocity = p.minVelocity;
	this->maxVelocity = p.maxVelocity;
	this->acceleration = p.acceleration;
	this->lifeSpan = p.lifeSpan;
	this->spawnSpeed = p.spawnSpeed;
	this->active = true;
	this->sprite = p.sprite;
	this->timeElapsed = 0;
	std::vector<Particle*>::iterator it;
	it = particles.begin();
	while (!particles.empty()) {
		delete (*it);
		it = particles.erase(it);
	}
	particles.clear();
	for (int i = 0; i < p.particles.size(); i++) {
		this->particles.push_back(p.particles[i]);
	}
}

ParticleEffect::~ParticleEffect() {
	std::vector<Particle*>::iterator it;
	it = particles.begin();
	while (!particles.empty()) {
		delete (*it);
		it = particles.erase(it);
	}
	particles.clear();
	UnloadTexture(sprite);
}

float GetRandom(int decimal) {
	int n = std::pow(10, decimal);
	return (std::rand() % n) / (float)n;
}

// Broken Block Effect
BrokenBlockEffect::BrokenBlockEffect(Vector2 pos, Vector2 bounds, Vector2 minVelo, Vector2 maxVelo, Vector2 accelerate, float lifeSpan, float spawnSpeed, const char* texture) 
	: ParticleEffect(pos, bounds, minVelo, maxVelo, accelerate, lifeSpan, spawnSpeed, texture) {}

BrokenBlockEffect::BrokenBlockEffect(Vector2 pos, Vector2 bounds, Vector2 minVelo, Vector2 maxVelo, Vector2 accelerate, float lifeSpan, float spawnSpeed, Texture2D texture) 
	: ParticleEffect(pos, bounds, minVelo, maxVelo, accelerate, lifeSpan, spawnSpeed, texture) {}

void BrokenBlockEffect::update(float deltaTime) {
	timeElapsed += deltaTime;
	while (timeElapsed >= spawnSpeed && particles.size() < 7) {
		timeElapsed -= spawnSpeed;
		Vector2 randomPos;
		randomPos.x = startPos.x + (GetRandom(4) * particleBounds.x);
		randomPos.y = startPos.y + (GetRandom(4) * particleBounds.y);

		Vector2 randomVel;
		randomVel.x = minVelocity.x + (GetRandom(4) * (maxVelocity.x - minVelocity.x));
		randomVel.y = minVelocity.y + (GetRandom(4) * (maxVelocity.y - minVelocity.y));

		Particle* p = new Particle(randomPos, randomVel, acceleration, lifeSpan);
		particles.push_back(p);
	}

	std::vector<Particle*>::iterator it;
	it = particles.begin();
	while (!particles.empty() && it != particles.end()) {
		(*it)->update(deltaTime);

		if ((*it)->lifeLeft() <= 0) {
			delete (*it);
			it = particles.erase(it);
			active = false;
		}
		else {
			it++;
		}
	}
}

void BrokenBlockEffect::draw() {
	std::vector<float> degree = { 60, -60, 120, -120 };
	int i = 0;
	std::vector<Particle*>::iterator it;
	for (it = particles.begin(); it != particles.end(); it++) {
		//DrawTextureEx(sprite, (*it)->getPosition(), 0.0f, 0.05f, WHITE);
		Rectangle dest = { (*it)->getPosition().x, (*it)->getPosition().y, 64 * 0.3, 64 * 0.3 };
		DrawTexturePro(sprite, Constants::PaletteResources::BRICK, dest, { 0,0 }, degree[i++], WHITE);
		if (i >= 4) i = 0;
	}
}