#include "../../../include/Enemy/LaserMechaKoopa/LaserBeam.h"
#include "../../../include/Game/GameContext.h"
#include "../../../include/System/Box2DWorldManager.h"
#include "../../../include/System/TextureManager.h"
#include "../../../include/Objects/Projectile/Projectile.h"
#include <raymath.h>
#include <memory>
LaserBeam::LaserBeam(Vector2 startPos, Vector2 dir, float chargeTime, float beamDuration, float beamMaxLength)
    : startPoint(startPos), direction(Vector2Normalize(dir)),
    chargeTime(chargeTime), beamDuration(beamDuration), beamMaxLength(beamMaxLength),
    timer(0.0f), beamLength(beamMaxLength), state(CHARGING),
    chargeStartWidth(320.0f), chargeEndWidth(50.0f),
    chargeColor({ 255, 255, 0, 180 }), beamColor(RED),
    hasImpact(false), Projectile(startPos,{1,1})
{
    physicsBody = Box2DWorldManager::getInstance().createProjectileBody(position, { size.x * Constants::TILE_SIZE, size.y * Constants::TILE_SIZE });
    if (physicsBody) {
        physicsBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
        for (b2Fixture* fixture = physicsBody->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
            b2Filter filter = fixture->GetFilterData();
            filter.maskBits = static_cast<uint16>(ObjectCategory::PROJECTILE);
            filter.categoryBits = static_cast<uint16>(ObjectCategory::BLOCK) | static_cast<uint16>(ObjectCategory::CHARACTER) |
                static_cast<uint16>(ObjectCategory::CHARACTER) | static_cast<uint16>(ObjectCategory::INTERACTIVE) |
                static_cast<uint16>(ObjectCategory::SHELL) | static_cast<uint16>(ObjectCategory::PROJECTILE);
            fixture->SetFilterData(filter);
        }
   }
    setActive(true);
}
LaserBeam::LaserBeam(Vector2 startPos, Vector2 size):startPoint(startPos),Projectile(startPos,size), chargeStartWidth(320.0f), chargeEndWidth(50.f),
chargeColor({ 255, 255, 0, 180 }), beamColor(RED),
hasImpact(false)
{
    physicsBody = Box2DWorldManager::getInstance().createProjectileBody(position, { size.x * Constants::TILE_SIZE, size.y * Constants::TILE_SIZE });
    if (physicsBody) {
        physicsBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
        for (b2Fixture* fixture = physicsBody->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
            b2Filter filter = fixture->GetFilterData();
            filter.maskBits = static_cast<uint16>(ObjectCategory::PROJECTILE);
            filter.categoryBits = static_cast<uint16>(ObjectCategory::BLOCK) | static_cast<uint16>(ObjectCategory::CHARACTER) |
                static_cast<uint16>(ObjectCategory::CHARACTER) | static_cast<uint16>(ObjectCategory::INTERACTIVE) |
                static_cast<uint16>(ObjectCategory::SHELL) | static_cast<uint16>(ObjectCategory::PROJECTILE);
            fixture->SetFilterData(filter);
        }
    }
}
void LaserBeam::update(float dt) {
    timer += dt;

    switch (state) {
    case CHARGING:
        updateCharge(dt);
        break;
    case FIRING:
        updateFire(dt);
        break;
    case DONE:
        setActive(false);
        break;
    }
}

void LaserBeam::updateCharge(float dt) {
    if (timer >= chargeTime) {
        timer = 0.0f;
        state = FIRING;
        performRaycast();
    }
}

void LaserBeam::updateFire(float dt) {
    if (timer >= beamDuration) {
        state = DONE;
        GameContext::getInstance().mark_for_deletion_Object(GameContext::getInstance().getSharedPtrFromRaw(this));
        return;
    }
    performRaycast();
}

void LaserBeam::performRaycast() {
    Vector2 endPos = Vector2Add(startPoint, Vector2Scale(direction, beamMaxLength));

    Vector2 hitPoint;
    b2Body* hitBody = nullptr;

    bool hit = Box2DWorldManager::getInstance().raycast(startPoint, endPos, hitPoint, &hitBody);

    // --- Visual beam length ---
    if (hit) {
        beamLength = Vector2Distance(startPoint, hitPoint);
        impactPoint = hitPoint;
        hasImpact = true;
    }
    else {
        beamLength = beamMaxLength;
        impactPoint = endPos;
        hasImpact = false;
    }

    // --- Physics beam length (slightly longer) ---
    constexpr float LASER_HITBOX_BUFFER = 10.0f;
    float physicsLength = std::min(beamLength + LASER_HITBOX_BUFFER, beamMaxLength);

    if (!physicsBody) {
        // Create new laser body
        physicsBody = Box2DWorldManager::getInstance().createLaserBeamBody(
            startPoint, physicsLength, 4.0f, atan2f(direction.y, direction.x)
        );
        if (physicsBody) {
            physicsBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
        }
    }
    else {
        // Update existing fixture
        while (b2Fixture* f = physicsBody->GetFixtureList()) {
            physicsBody->DestroyFixture(f);
        }

        b2PolygonShape beamShape;
        beamShape.SetAsBox(
            Box2DWorldManager::raylibToB2(physicsLength * 0.5f),
            Box2DWorldManager::raylibToB2(4.0f),
            b2Vec2(Box2DWorldManager::raylibToB2(physicsLength * 0.5f), 0.0f),
            0.0f
        );

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &beamShape;
        fixtureDef.isSensor = true;

        b2Filter filter;
        filter.maskBits = static_cast<uint16>(ObjectCategory::PROJECTILE);
        filter.categoryBits =
            static_cast<uint16>(ObjectCategory::CHARACTER) |
            static_cast<uint16>(ObjectCategory::BLOCK) |
            static_cast<uint16>(ObjectCategory::INTERACTIVE);
        fixtureDef.filter = filter;

        physicsBody->CreateFixture(&fixtureDef);
        physicsBody->SetTransform(Box2DWorldManager::raylibToB2(startPoint), atan2f(direction.y, direction.x));
        
    }
}




void LaserBeam::draw() {
    switch (state) {
    case CHARGING: {
        float t = timer / chargeTime;
        float currentWidth = Lerp(chargeStartWidth, chargeEndWidth, t);
        Vector2 endPos = Vector2Add(startPoint, Vector2Scale(direction, beamMaxLength));
        float brightness1 = Lerp(0.5f, 1.f, t);
        Color chargeColor1 = {
            (unsigned char)Lerp(SKYBLUE.r, WHITE.r, t) * brightness1,
            (unsigned char)Lerp(SKYBLUE.g,WHITE.g, t) * brightness1,
            (unsigned char)Lerp(SKYBLUE.b,WHITE.b, t) * brightness1,
            (unsigned char)Lerp(155,255, t) // fade out
        };

        DrawLineEx(startPoint, endPos, currentWidth, chargeColor1);
        break;
    }
    case FIRING: {
        Vector2 endPos = Vector2Add(startPoint, Vector2Scale(direction, beamLength));

        Texture2D beamTex = TextureManager::enemyTextures;
        Rectangle srcStart = TextureManager::Enemy_sprite_boxes[377]; // start piece
        Rectangle srcMid = TextureManager::Enemy_sprite_boxes[398]; // repeat piece
        Rectangle srcEnd = TextureManager::Enemy_sprite_boxes[399]; // end piece

        float rotation = atan2f(direction.y, direction.x) * RAD2DEG;

        float startWidth = srcStart.width;
        float midWidth = srcMid.width;
        float endWidth = srcEnd.width;
        float height = chargeEndWidth; // beam thickness

        // Draw start
        {
            Rectangle dest = { startPoint.x, startPoint.y, startWidth, height };
            Vector2 origin = { 0, height / 2.0f };
            DrawTexturePro(beamTex, srcStart, dest, origin, rotation, SKYBLUE);
        }

        float remainingLength = beamLength-startWidth;
        Vector2 segPos = startPoint;

        segPos.x += direction.x * startWidth;
        segPos.y += direction.y * startWidth;

        while (remainingLength > 0) {
            float drawWidth = (remainingLength >= midWidth) ? midWidth : remainingLength;

            Rectangle dest = { segPos.x, segPos.y, drawWidth, height };
            Vector2 origin = { 0, height / 2.0f };
            DrawTexturePro(beamTex, srcMid, dest, origin, rotation, SKYBLUE);
            segPos.x += direction.x * drawWidth;
            segPos.y += direction.y * drawWidth;
            remainingLength -= drawWidth;
        }
        
         Rectangle dest = { segPos.x, segPos.y, endWidth, height };
         Vector2 origin = { 0, height / 2.0f };

         if (remainingLength < endWidth) {
                dest.width = fmax(remainingLength, 0.0f);
         }

            DrawTexturePro(beamTex, srcEnd, dest, origin, rotation, SKYBLUE);
       

            if (hasImpact) {
                Texture2D pointTex = TextureManager::enemyTextures;
                Rectangle srcPoint = TextureManager::Enemy_sprite_boxes[378];

                float scale = 3.f; 
                Rectangle dest = {
                    impactPoint.x,
                    impactPoint.y,
                    srcPoint.width * scale,
                    srcPoint.height * scale
                };
                Vector2 origin = { dest.width / 2.0f, dest.height / 2.0f };
                DrawTexturePro(pointTex, srcPoint, dest, origin, 0.0f, WHITE);
            }
        break;
    }



    default:
        break;
    }
}
void LaserBeam::onCollision(std::shared_ptr<Object> other, Direction direction) {

    std::cout << "HITTED" << "\n";
    switch (other->getObjectCategory()) {
    case ObjectCategory::CHARACTER: {
        auto cha = std::dynamic_pointer_cast<Character>(other);
        if (cha) {
            cha->takeDamage(1); 
        }
        break;
    }
    }
}
