#pragma once

class Spring;
class SpringState {
public:
	virtual ~SpringState() = default;
	virtual void enter(Spring* spring) = 0;
	virtual void update(Spring* spring, float deltaTime) = 0;
	virtual void exit(Spring* spring) = 0;
};

class SpringIdleState {

};