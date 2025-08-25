#include "../../../include/Enemy/Koopa/KoopaShellState.h"
#include <vector>
#include "../../../include/Objects/ObjectFactory.h"
#include "../../../include/Enemy/Koopa/KoopaShell.h"
#include <raylib.h>
#include "../../../include/Game/GameContext.h"
#include "../../../include/System/Interface.h"
#include "../../../include/System/TextureManager.h"
#include "../../../include/System/Constant.h"
#include "../../../include/System/Box2DWorldManager.h"
#include "../../../include/System/CameraSystem.h"
#include "../../../include/Enemy/TriggerZone.h"
void KoopaShellIdleState::enter(KoopaShell* shell) {
    shell->timer = 0.0f; 
    shell->curFrame = 0;
    shell->spritebox = TextureManager::Enemy_sprite_boxes[shell->getSpriteData()[0].first];
    shell->activeTimer = 0.0f;
    shell->active = false;
    for (b2Fixture* fixture = shell->physicsBody->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
        b2Filter filter = fixture->GetFilterData();
        filter.maskBits = static_cast<uint16>(ObjectCategory::BLOCK);
        filter.categoryBits = static_cast<uint16>(ObjectCategory::BLOCK) | static_cast<uint16>(ObjectCategory::PROJECTILE) | 
            static_cast<uint16>(ObjectCategory::INTERACTIVE);
        fixture->SetFilterData(filter);
    }
}

void KoopaShellIdleState::exit(KoopaShell* shell) {
    shell->curFrame = shell->getSpriteData()[0].second - shell->getSpriteData()[0].first;
}

void KoopaShellIdleState::update(KoopaShell* shell, float deltaTime) {
	shell->timer += deltaTime;
    shell->activeTimer += deltaTime;
    if (!shell->active && shell->activeTimer > 0.05f) {
        for (b2Fixture* fixture = shell->physicsBody->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
            b2Filter filter = fixture->GetFilterData();
            filter.maskBits = static_cast<uint16>(ObjectCategory::SHELL);
            filter.categoryBits = static_cast<uint16> (ObjectCategory::CHARACTER) | static_cast<uint16>(ObjectCategory::BLOCK) |
                static_cast<uint16>(ObjectCategory::PROJECTILE) | static_cast<uint16>(ObjectCategory::INTERACTIVE);
            fixture->SetFilterData(filter);
        }
    }
    shell->curFrame %= shell->getSpriteData()[0].second - shell->getSpriteData()[0].first + 1 ;
    shell->spritebox = TextureManager::Enemy_sprite_boxes[shell->getSpriteData()[0].first + shell->curFrame];
    if (shell->timer > Constants::KoopaShell::IDLE_DURATION) { 
        shell->changeState(&KoopaShellRevivingState::getInstance());
    }
    b2Vec2 currentVel = shell->physicsBody->GetLinearVelocity();
    shell->physicsBody->SetLinearVelocity(b2Vec2(0, currentVel.y));
    if(shell->triggerZone)shell->triggerZone->update(deltaTime);
}

void KoopaShellIdleState::onCollision(KoopaShell* shell, std::shared_ptr<Object> other, Direction dir) {
    b2Vec2 currentVel = shell->physicsBody->GetLinearVelocity();
    switch (other->getObjectCategory()) {
    case ObjectCategory::CHARACTER:
        if (std::dynamic_pointer_cast<Character>(other)->getPowerState() == PowerState::STAR) {
            shell->changeState(&KoopaShellKnockedState::getInstance());
            break;
        }
        float shellX = shell->getPosition().x+ shell->getHitBox()[0].width/2;
		float referenceX = other->getPosition().x;
		
        if (shellX > referenceX) {
            shell->physicsBody->SetLinearVelocity(
                b2Vec2(Box2DWorldManager::raylibToB2(Constants::KoopaShell::MOVING_SPEED), currentVel.y)
            );
        }
        else {
            shell->physicsBody->SetLinearVelocity(
                b2Vec2(-Box2DWorldManager::raylibToB2(Constants::KoopaShell::MOVING_SPEED), currentVel.y)
            );
        }
        shell->changeState(&KoopaShellMovingState::getInstance());
    }
}

ObjectCategory KoopaShellIdleState::getObjectCategory() const {
    return ObjectCategory::SHELL;
}





void KoopaShellMovingState::enter(KoopaShell* shell) {
    shell->timer = 0.0f;
    shell->curFrame = 0; 
    shell->aniSpeed = 0.05;  
    shell->spritebox = TextureManager::Enemy_sprite_boxes[shell->getSpriteData()[1].first];
    for (b2Fixture* fixture = shell->physicsBody->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
        b2Filter filter = fixture->GetFilterData();
        filter.maskBits = static_cast<uint16>(ObjectCategory::PROJECTILE);
        filter.categoryBits = static_cast<uint16> (ObjectCategory::CHARACTER) | static_cast<uint16>(ObjectCategory::BLOCK) |
            static_cast<uint16>(ObjectCategory::ENEMY) | static_cast<uint16>(ObjectCategory::INTERACTIVE)|static_cast<uint16>(ObjectCategory::PROJECTILE);
        fixture->SetFilterData(filter);
    }
}

void KoopaShellMovingState::exit(KoopaShell* shell) {
	shell->aniSpeed = 0.2;
    shell->curFrame = shell->getSpriteData()[0].second - shell->getSpriteData()[0].first;
    shell->spritebox = TextureManager::Enemy_sprite_boxes[shell->getSpriteData()[1].first + shell->curFrame];
}

void KoopaShellMovingState::update(KoopaShell* shell, float deltaTime) {
    shell->curFrame %= shell->getSpriteData()[1].second - shell->getSpriteData()[1].first+1;
    shell->spritebox = TextureManager::Enemy_sprite_boxes[shell->getSpriteData()[1].first + shell->curFrame];
    shell->timer += deltaTime;
  /*  if (shell->timer > 10.0f) {
        shell->changeState(&KoopaShellKnockedState::getInstance());
    }*/
}

void KoopaShellMovingState::onCollision(KoopaShell* shell, std::shared_ptr<Object> other, Direction dir) {
    b2Vec2 currentVel = shell->physicsBody->GetLinearVelocity();
    if(other->getObjectCategory() == ObjectCategory::BLOCK) {
        switch (dir) {
        case Direction::LEFT:
            shell->physicsBody->SetLinearVelocity(b2Vec2(abs(currentVel.x), currentVel.y));
            break;
        case Direction::RIGHT:
            shell->physicsBody->SetLinearVelocity(b2Vec2(-abs(currentVel.x), currentVel.y));
            break;
        }
	}
    
    if (other->getObjectCategory() == ObjectCategory::CHARACTER) {
        if (std::dynamic_pointer_cast<Character>(other)->getPowerState() == PowerState::STAR) {
            shell->changeState(&KoopaShellKnockedState::getInstance());
        }
        else{
            shell->physicsBody->SetLinearVelocity(b2Vec2(0, currentVel.y));
            shell->changeState(&KoopaShellIdleState::getInstance());
		}
    }
}

ObjectCategory KoopaShellMovingState::getObjectCategory() const {
    return ObjectCategory::PROJECTILE;
}





void KoopaShellRevivingState::enter(KoopaShell* shell) {
    shell->curFrame = 0;
	shell->timer = 0.0f;
    shell->spritebox = TextureManager::Enemy_sprite_boxes[shell->getSpriteData()[2].second];
    for (b2Fixture* fixture = shell->physicsBody->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
        b2Filter filter = fixture->GetFilterData();
        filter.maskBits = static_cast<uint16>(ObjectCategory::BLOCK);
        filter.categoryBits = static_cast<uint16> (ObjectCategory::CHARACTER) | static_cast<uint16>(ObjectCategory::BLOCK) |
            static_cast<uint16>(ObjectCategory::PROJECTILE) | static_cast<uint16>(ObjectCategory::SHELL);
        fixture->SetFilterData(filter);
    }
}

void KoopaShellRevivingState::exit(KoopaShell* shell) {
	shell->timer = 0.0f; 

}

void KoopaShellRevivingState::update(KoopaShell* shell, float deltaTime) {
	shell->timer += deltaTime;
    if(shell->timer > Constants::KoopaShell::REVIVE_DURATION) { 
        EnemyType type;
        switch (shell->getType()) {
        case KoopaShellType::GREEN_KOOPA_SHELL:
            type = EnemyType::GREEN_KOOPA;
            break;
        case KoopaShellType::RED_KOOPA_SHELL:
            type = EnemyType::RED_KOOPA;
            break;
        default:
            type = EnemyType::GREEN_KOOPA;
            break;
        }
        Vector2 spawnPosition = { shell->getPosition().x + (shell->size.x * Constants::TILE_SIZE) / 2, shell->getPosition().y + shell->size.y * Constants::TILE_SIZE };
        GameContext::getInstance().addObject(type, spawnPosition, { Constants::GreenKoopa::standardSize });
        GameContext::getInstance().mark_for_deletion_Object(GameContext::getInstance().getSharedPtrFromRaw(shell));
    }
}

void KoopaShellRevivingState::onCollision(KoopaShell* shell, std::shared_ptr<Object> other, Direction dir) {
    if (other->getObjectCategory() == ObjectCategory::CHARACTER) {
        if (std::dynamic_pointer_cast<Character>(other)->getPowerState() == PowerState::STAR) {
            shell->changeState(&KoopaShellKnockedState::getInstance());
        }
        if(dir== Direction::UP)
        {
            b2Vec2 currentVel = shell->physicsBody->GetLinearVelocity();
            shell->physicsBody->SetLinearVelocity(b2Vec2(0, currentVel.y));
            shell->changeState(&KoopaShellIdleState::getInstance());
		}
    }
    if (other->getObjectCategory() == ObjectCategory::PROJECTILE)
    {
        shell->changeState(&KoopaShellKnockedState::getInstance());
    }
}

ObjectCategory KoopaShellRevivingState::getObjectCategory() const {
    return ObjectCategory::BLOCK;
}





void KoopaShellKnockedState::enter(KoopaShell* shell) {
    for (b2Fixture* fixture = shell->physicsBody->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
        b2Filter filter = fixture->GetFilterData();
        filter.maskBits = 0;
        fixture->SetFilterData(filter);
    }
    shell->physicsBody->SetLinearVelocity(b2Vec2(0, Box2DWorldManager::raylibToB2(Constants::GreenKoopa::KNOCK_VELOCITY)));
    shell->curFrame = 0;
    shell->spritebox = TextureManager::Enemy_sprite_boxes[shell->getSpriteData()[0].first]; 
}

void KoopaShellKnockedState::exit(KoopaShell* shell) {
    GameContext::getInstance().mark_for_deletion_Object(GameContext::getInstance().getSharedPtrFromRaw(shell));
}

void KoopaShellKnockedState::update(KoopaShell* shell, float deltaTime) {
    shell->curFrame %= shell->getSpriteData()[0].second - shell->getSpriteData()[0].first + 1;
    shell->spritebox = TextureManager::Enemy_sprite_boxes[shell->getSpriteData()[0].first + shell->curFrame];
    if (shell->getPosition().y >= GetScreenHeight()) {
        shell->changeState(nullptr);
    }
}

void KoopaShellKnockedState::onCollision(KoopaShell* shell, std::shared_ptr<Object> other, Direction dir) {

}

ObjectCategory KoopaShellKnockedState::getObjectCategory() const {
    return ObjectCategory::BACKGROUND;
}