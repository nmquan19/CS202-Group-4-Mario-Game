#pragma once
#include <vector>
#include "../../Objects/ObjectFactory.h"
#include "../../System/Interface.h"
class KoopaShell;
class  KoopaShellState
{
public : 
	virtual ~KoopaShellState() = default;
	virtual void enter(KoopaShell* shell) = 0;
	virtual void exit(KoopaShell* shell) = 0;
	virtual void update(KoopaShell* shell, float deltaTime) = 0;
    virtual void checkCollision(KoopaShell* shell, const std::vector<Object*>& candidates) = 0; 
    virtual ObjectCategory getObjectCategory() const =0 ; 
    virtual std::vector<ObjectCategory> getCollisionTargets() const = 0;
};
class KoopaShellIdleState : public KoopaShellState {
public:
    static KoopaShellIdleState& getInstance() {
        static KoopaShellIdleState instance;
        return instance;
    }
    void enter(KoopaShell* shell) override;
    void exit(KoopaShell* shell) override;
    void update(KoopaShell* shell, float deltaTime) override;
    void checkCollision(KoopaShell* shell, const std::vector<Object*>& candidates) override;
    ObjectCategory getObjectCategory() const override; 
    std::vector<ObjectCategory> getCollisionTargets() const override; 

private:
    KoopaShellIdleState() = default;
};
class KoopaShellMovingState : public KoopaShellState {
public:
    static KoopaShellMovingState& getInstance() {
        static KoopaShellMovingState instance;
        return instance;
    }
    void enter(KoopaShell* shell) override;
    void exit(KoopaShell* shell) override;
    void update(KoopaShell* shell, float deltaTime) override;
    void checkCollision(KoopaShell* shell, const std::vector<Object*>& candidates) override;
    ObjectCategory getObjectCategory() const override;

    std::vector<ObjectCategory> getCollisionTargets() const override;

private:
    KoopaShellMovingState() = default;
};
    
class KoopaShellRevivingState : public KoopaShellState {
public:
    static KoopaShellRevivingState& getInstance() {
        static KoopaShellRevivingState instance;
        return instance;
    }
    void enter(KoopaShell* shell) override;
    void exit(KoopaShell* shell) override;
    void update(KoopaShell* shell, float deltaTime) override;
    void checkCollision(KoopaShell* shell, const std::vector<Object*>& candidates) override;
    ObjectCategory getObjectCategory() const override;

    std::vector<ObjectCategory> getCollisionTargets() const override;

private:
    KoopaShellRevivingState() = default;
};

class KoopaShellCollectedState: public KoopaShellState{
public:
    static KoopaShellCollectedState& getInstance() {
        static KoopaShellCollectedState instance;
        return instance;
    }
    void enter(KoopaShell* shell) override;
    void exit(KoopaShell* shell) override;
    void update(KoopaShell* shell, float deltaTime) override;
    void checkCollision(KoopaShell* shell, const std::vector<Object*>& candidates) override;
    ObjectCategory getObjectCategory() const override;

    std::vector<ObjectCategory> getCollisionTargets() const override;

private:
    KoopaShellCollectedState() = default;
};

class KoopaShellKnockedState : public KoopaShellState {
   public:
    static KoopaShellKnockedState& getInstance() {
        static KoopaShellKnockedState instance;
        return instance;
    }
    void enter(KoopaShell* shell) override;
    void exit(KoopaShell* shell) override;
    void update(KoopaShell* shell, float deltaTime) override;
    void checkCollision(KoopaShell* shell, const std::vector<Object*>& candidates) override;
    ObjectCategory getObjectCategory() const override;
    std::vector<ObjectCategory> getCollisionTargets() const override;

private:
    KoopaShellKnockedState() = default;
};

