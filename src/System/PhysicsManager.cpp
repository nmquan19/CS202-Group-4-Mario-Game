#include <algorithm>
#include "..\..\include\System\PhysicsManager.h"

PhysicsManager* PhysicsManager::instance = nullptr;

PhysicsManager& PhysicsManager::getInstance() {
	if (!instance) {
		instance = new PhysicsManager();
	}
	return *instance;
}

void PhysicsManager::addCollidable(ICollidable* collidable) {
	if (collidable && std::find(collidables.begin(), collidables.end(), collidable) == collidables.end()) {
		collidables.push_back(collidable);
	}
}

void PhysicsManager::markForDeletion(ICollidable* collidable) {
	if (collidable) {
		if (collidable->isActive()) collidable->setActive(false);
		toDelete.push_back(collidable);
	}
}

void PhysicsManager::deleteObjects() {
	for (auto* obj : toDelete) {;
		auto it = std::find(collidables.begin(), collidables.end(), obj);
		if (it != collidables.end()) {
			collidables.erase(it);
		}
		delete obj;
	}
	toDelete.clear();
}

void PhysicsManager::update() {
	deleteObjects();

	frameCounter++;
	if (frameCounter >= REBUILD_FREQUENCY) {
		frameCounter = 0;

		quadTree->clear();
		for (auto* collidable : collidables) {
			if (collidable->isActive()) {
				quadTree->insert(collidable);
			}
		}
	}
	for (auto* collidable : collidables) {
		if (!collidable) continue;
		if (collidable && !collidable->isActive()) {
			markForDeletion(collidable);
			continue;
		}

		auto targetLayer = collidable->getCollisionTargets();
		if (targetLayer.empty()) continue;

		std::vector<ICollidable*> candidates = quadTree->retrieve(collidable);
		std::vector<ICollidable*> filterdCandidates;
		filterdCandidates.reserve(candidates.size());

		for (auto* candidate : candidates) {
			if (candidate && candidate != collidable && candidate->isActive()) {
				if (std::find(targetLayer.begin(), targetLayer.end(), candidate->getCollisionLayer()) != targetLayer.end()
					&& PhysicsManager::getInstance().checkCollision(collidable->getHitBox(), candidate->getHitBox())) {
					filterdCandidates.push_back(candidate);
				}
			}
		}

		if (!filterdCandidates.empty()) {
			collidable->checkCollision(filterdCandidates);
			// set bool onCollidable true
		}
		//else {
		//	// set false;
		//}
	}
}

bool PhysicsManager::checkCollision(const Rectangle& rect1, const Rectangle& rect2) {
	return CheckCollisionRecs(rect1, rect2);
}

std::vector<ICollidable*> PhysicsManager::getCollidablesInLayer(CollissionLayer layer) {
	std::vector<ICollidable*> result;
	for (auto* collidable : collidables) {
		if (collidable->getCollisionLayer() == layer) {
			result.push_back(collidable);
		}
	}
	return result;
}

void PhysicsManager::cleanup() {
	delete instance;
	instance = nullptr;
}

void PhysicsManager::setWorldBounds(Rectangle bounds) {
	quadTree = std::make_unique<QuadTree>(bounds);
}

std::vector<ICollidable*> PhysicsManager::getObjectsInArea(Rectangle area) {
	return quadTree->retrieve(area);
}

std::vector<ICollidable*> PhysicsManager::getObjectsNearPoint(Vector2 point, float radius) {
	Rectangle area = { point.x - radius, point.y - radius, radius * 2, radius * 2 };
	return quadTree->retrieve(area);
}

void PhysicsManager::drawDebug() const {
	if (quadTree) {
		quadTree->drawDebug();
	}
}

void PhysicsManager::getDebugStats(int& totalNodes, int& maxDepth, int& totalObjects) const {
	if (quadTree) {
		quadTree->getStats(totalNodes, maxDepth, totalObjects);
	}
	else {
		totalNodes = maxDepth = totalObjects = 0;
	}
}