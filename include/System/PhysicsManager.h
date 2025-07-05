#pragma once

#include <vector>
#include <memory>
#include "Interface.h"
#include "QuadTree.h"
#include "../Objects/ObjectFactory.h"

class PhysicsManager {
public:
	static PhysicsManager& getInstance();
	static void cleanup();

	void addObject(Object* object);
	void markForDeletion(Object* object);
	void deleteObjects();
	void update();
	bool checkCollision(const Rectangle& rect1, const Rectangle& rect2);
	std::vector<Object*> getObjectsInLayer(ObjectCategory objectCategory);

	void setWorldBounds(Rectangle bounds);
	std::vector<Object*> getObjectsInArea(Rectangle area);
	std::vector<Object*> getObjectsNearPoint(Vector2 point, float radius);

	void drawDebug() const;
	void getDebugStats(int& totalNodes, int& maxDepth, int& totalObjects) const;
private:
	PhysicsManager() = default;
	std::vector<Object*> objects;	
	std::vector<Object*> toDelete;
	std::unique_ptr<QuadTree> quadTree; 
	static PhysicsManager* instance;
	const int REBUILD_FREQUENCY = 2;
	int frameCounter = 0;
};