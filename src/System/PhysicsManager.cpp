#include <algorithm>
#include "../../include/System/PhysicsManager.h"

PhysicsManager* PhysicsManager::instance = nullptr;

PhysicsManager& PhysicsManager::getInstance() {
	if (!instance) {
		instance = new PhysicsManager();
	}
	return *instance;
}

void PhysicsManager::addObject(Object* object) {
	if (object && std::find(objects.begin(), objects.end(), object) == objects.end()) {
		objects.push_back(object);
	}
}

void PhysicsManager::markForDeletion(Object* object) {
	if (object) {
		if (object->isActive()) object->setActive(false);
		toDelete.push_back(object);
	}
}

void PhysicsManager::deleteObjects() {
	for (auto* obj : toDelete) {
		auto it = std::find(objects.begin(), objects.end(), obj);
		if (it != objects.end()) {
			objects.erase(it);
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
		for (auto* object : objects) {
			if (object && object->isActive()) {
				quadTree->insert(object);
			}
		}
	}
	for (auto* object : objects) {
		if (!object) continue;
		if (object && !object->isActive()) {
			markForDeletion(object);
			continue;
		}

		auto targetLayer = object->getCollisionTargets();
		if (targetLayer.empty()) continue;

		std::vector<Object*> candidates = quadTree->retrieve(object);
		std::vector<Object*> filterdCandidates;
		filterdCandidates.reserve(candidates.size());

		for (auto* candidate : candidates) {
			if (candidate && candidate != object && candidate->isActive()) {
				if (std::find(targetLayer.begin(), targetLayer.end(), candidate->getObjectCategory()) != targetLayer.end()
					&& PhysicsManager::getInstance().checkCollision(object->getHitBox(), candidate->getHitBox())) {
					filterdCandidates.push_back(candidate);
				}
			}
		}

		if (!filterdCandidates.empty()) {
			object->checkCollision(filterdCandidates);
			// set true
		}
		//else {
		//	// set false;
		//}
	}
}

bool PhysicsManager::checkCollision(const Rectangle& rect1, const Rectangle& rect2) {
	return CheckCollisionRecs(rect1, rect2);
}

std::vector<Object*> PhysicsManager::getObjectsInLayer(ObjectCategory objectCategory) {
	std::vector<Object*> result;
	for (auto* object : objects) {
		if (object->getObjectCategory() == objectCategory) {
			result.push_back(object);
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

std::vector<Object*> PhysicsManager::getObjectsInArea(Rectangle area) {
	return quadTree->retrieve(area);
}

std::vector<Object*> PhysicsManager::getObjectsNearPoint(Vector2 point, float radius) {
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