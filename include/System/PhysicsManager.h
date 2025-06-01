#pragma once

#include <vector>
#include <memory>
#include "Interface.h"
#include "QuadTree.h"

class PhysicsManager {
public:
	static PhysicsManager& getInstance();
	static void cleanup();

	void addCollidable(ICollidable* collidable);
	void removeCollidable(ICollidable* collidable);
	void update();
	bool checkCollision(const Rectangle& rect1, const Rectangle& rect2);
	std::vector<ICollidable*> getCollidablesInLayer(CollissionLayer layer);

	void setWorldBounds(Rectangle bounds);
	std::vector<ICollidable*> getObjectsInArea(Rectangle area);
	std::vector<ICollidable*> getObjectsNearPoint(Vector2 point, float radius);

	void drawDebug() const;
	void getDebugStats(int& totalNodes, int& maxDepth, int& totalObjects) const;
private:
	PhysicsManager() = default;
	std::vector<ICollidable*> collidables;
	std::unique_ptr<QuadTree> quadTree;
	static PhysicsManager* instance;
	const int REBUILD_FREQUENCY = 2;
	int frameCounter = 0;
};