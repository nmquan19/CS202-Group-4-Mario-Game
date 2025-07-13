#include "../Enemy.h"
#include <vector>
#include "../../Objects/ObjectFactory.h"
#include "../LedgeDetector.h"
#include <raylib.h>
class Koopa: public Enemy
{
private:
    bool stompedAnimation = false;
    bool knockAnimation = false;
    float stompedTime = 0.0f;
    float stompedDuration = 0.3f;
    float knockVelocity = -300.0f;
	LedgeDetector ledgeDetector;
public:
    Koopa(Vector2 startPos, Vector2 size);
    Koopa(Vector2 startPos, Vector2 velocity, Vector2 accelleration);
    friend class KoopaWanderingState;
    friend class KoopaStompedState;
    friend class KoopaKnockState;

    void handleEnvironmentCollision(Object* other) override;
    void onCollision(Object* other) override;
    void update(float deltaTime);
    void draw() override;
    void checkCollision(const std::vector<Object*>& candidates) override;
    void handleCharacterCollision(Object* other);
    void die() override;
    void takeDamage(int damage) override;
}; 