#pragma once
#include "../Enemy.h"
#include "WorldState.h"
#include "BossPhaseState.h"
#include "SimulateState.h"
#include <memory>
#include <vector>
#include "../../Objects/ObjectFactory.h"
#include "../../System/Interface.h"
#include <string>
#include "../../System/InterpolationController.h"
#include <raylib.h>
class Boss : public Enemy {
	private : 
	
	protected: 
	
	public :
		Boss(Vector2 startPos, Vector2 size, Texture2D texture) : Enemy(startPos, texture, size) {}
		virtual void update(float dt) = 0;
		virtual void takeDamage(int amount) = 0;
		virtual void draw() = 0;
		virtual void handleCharacterCollision(Object* other) = 0;
		virtual void handleEnvironmentCollision(Object* other) = 0;
		virtual void checkCollision(const std::vector<Object*>& candidates) = 0;
		virtual void die() = 0;
		virtual bool isAlive() const = 0;
		virtual ObjectType getObjectType() const override =0;
		virtual EnemyType getType() const override = 0;
		virtual void onCollision(Object* other) = 0;
		virtual void changePhase(std::unique_ptr<BossPhaseState> newPhase) = 0;
		virtual const WorldState& getWorldState() const = 0;
		virtual std::unique_ptr<BaseSimState> getCurrentSimState() const = 0;
		virtual void setAnimation(const std::string& animationName) override = 0 ;
};