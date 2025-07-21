#pragma once

#include <vector>
#include <memory>
#include "Interface.h"
#include "QuadTree.h"
#include "../Objects/ObjectFactory.h"
#include <raylib.h>

class PhysicsManager {
public:
	static PhysicsManager& getInstance();
	static void cleanup();

	void addObject(std::shared_ptr<Object> object);
	void markForDeletion(std::shared_ptr<Object> object);
	void update();
	bool checkCollision(const Rectangle& rect1, const Rectangle& rect2);
	std::vector<std::shared_ptr<Object>> getObjectsInLayer(ObjectCategory objectCategory);

	void setWorldBounds(Rectangle bounds);
	std::vector<std::shared_ptr<Object>> getObjectsInArea(Rectangle area);
	std::vector<std::shared_ptr<Object>> getObjectsNearPoint(Vector2 point, float radius);

	void drawDebug() const;
	void getDebugStats(int& totalNodes, int& maxDepth, int& totalObjects) const;
private:
	PhysicsManager() = default;
	std::vector<std::shared_ptr<Object>> objects;	
	std::vector<std::shared_ptr<Object>> toDeleteObjects;
	std::unique_ptr<QuadTree> quadTree; 
	static PhysicsManager* instance;
	const int REBUILD_FREQUENCY = 2;
	int frameCounter = 0;
};