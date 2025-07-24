    #include "../Enemy.h"
    #include <vector>
    #include "../../Objects/ObjectFactory.h"
    #include "../LedgeDetector.h"
    #include <raylib.h>
    #include <memory>
#include "../../System/Interface.h"
    class Koopa: public Enemy
    {
    private:
        bool stompedAnimation = false;
        bool knockAnimation = false;
	    std::shared_ptr<LedgeDetector> ledgeDetector;
    public:
        Koopa(Vector2 startPos, Vector2 size);
        Koopa(Vector2 startPos, Vector2 velocity, Vector2 accelleration);
        ~Koopa(); 
        friend class KoopaWanderingState;
        friend class KoopaStompedState;
        friend class KoopaKnockState;
        void handleEnvironmentCollision(std::shared_ptr<Object> other) override;
        void onCollision(std::shared_ptr<Object> other) override;
        void update(float deltaTime);
        void draw() override;
        void checkCollision(const std::vector<std::shared_ptr<Object>>& candidates) override;
        void die() override;
        void takeDamage(int damage) override;
		KoopaShellType getShellType() const;
    }; 

    class GreenKoopa : public Koopa {
        public:
            GreenKoopa(Vector2 startPos, Vector2 size);
            GreenKoopa(Vector2 startPos, Vector2 velocity, Vector2 accelleration);
            EnemyType getType() const override;
            ObjectType getObjectType() const override;
    }; 
    class RedKoopa : public Koopa {
        public:
            RedKoopa(Vector2 startPos, Vector2 size);
            RedKoopa(Vector2 startPos, Vector2 velocity, Vector2 accelleration);
            EnemyType getType() const override;
            ObjectType getObjectType() const override;
	};