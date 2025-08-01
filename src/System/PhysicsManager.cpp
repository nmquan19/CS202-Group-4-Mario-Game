#include <algorithm>
#include "../../include/System/PhysicsManager.h"
#include <memory>
#include <iostream>
#include <vector>
#include "../../include/Objects/ObjectFactory.h"
#include "../../include/System/Interface.h"
#include "../../include/System/QuadTree.h"
#include <raylib.h>

PhysicsManager* PhysicsManager::instance = nullptr;

PhysicsManager& PhysicsManager::getInstance() {
	if (!instance) {
		instance = new PhysicsManager();
	}
	return *instance;
}

void PhysicsManager::addObject(std::shared_ptr<Object> object) {
	if (object && std::find(objects.begin(), objects.end(), object) == objects.end()) {
		objects.push_back(object);
	}
}

void PhysicsManager::markForDeletion(std::shared_ptr<Object> object) {
	if (object) {
		object->setActive(false);
		toDeleteObjects.push_back(object);
	}
}
void PhysicsManager::update() {

	for (const auto& obj : toDeleteObjects)
	{
		if (std::find(objects.begin(), objects.end(), obj) != objects.end()) {
			objects.erase(std::remove(objects.begin(), objects.end(), obj), objects.end());
			std::cout << "Deleted an object in Physics\n";
		}
	}
	toDeleteObjects.clear();

	frameCounter++;
	if (frameCounter >= REBUILD_FREQUENCY) {
		frameCounter = 0;

		quadTree->clear();
		for (const auto object : objects) {
			if (object && object->isActive()) {
				quadTree->insert(object);
			}
		}
	}
 
	for (const auto object : objects) {

		if (!object|| !object->isActive()) continue;
					
		auto targetLayer = object->getCollisionTargets();
		if (targetLayer.empty()) continue;

		std::vector<std::shared_ptr<Object>> candidates = quadTree->retrieve(object);
		std::vector<std::shared_ptr<Object>> filterdCandidates;
		filterdCandidates.reserve(candidates.size());

		for (auto candidate : candidates) {
			if (candidate && candidate != object && candidate->isActive()) {
				if (std::find(targetLayer.begin(), targetLayer.end(), candidate->getObjectCategory()) != targetLayer.end()
					&& PhysicsManager::getInstance().checkCollision(object->getHitBox(), candidate->getHitBox())) {
					filterdCandidates.push_back(candidate);
				}
			}
		}
	}
}

bool PhysicsManager::checkCollision(const Rectangle& rect1, const Rectangle& rect2) {
	return CheckCollisionRecs(rect1, rect2);
}

bool PhysicsManager::checkCollision(const std::vector<Rectangle>& hitboxes1, const std::vector<Rectangle>& hitboxes2) {
	for (const auto& rect1 : hitboxes1) {
		for (const auto& rect2 : hitboxes2) {
			if (CheckCollisionRecs(rect1, rect2)) {
				return true;
			}
		}
	}
	return false;
}

std::vector<std::shared_ptr<Object>> PhysicsManager::getObjectsInLayer(ObjectCategory objectCategory) {
	std::vector<std::shared_ptr<Object>> result;
	for (const auto object : objects) {
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

std::vector<std::shared_ptr<Object>> PhysicsManager::getObjectsInArea(Rectangle area) {
	auto rawObjects = quadTree->retrieve(area);
	std::vector<std::shared_ptr<Object>> result;

	for (auto rawObj : rawObjects) {
		auto it = std::find_if(objects.begin(), objects.end(), [rawObj](const std::shared_ptr<Object>& obj) {
			return obj == rawObj;
			});
		if (it != objects.end()) {
			result.push_back(*it);
		}
	}
	return result;
}

std::vector<std::shared_ptr<Object>> PhysicsManager::getObjectsNearPoint(Vector2 point, float radius) {
	Rectangle area = { point.x - radius, point.y - radius, radius * 2, radius * 2 };
	return getObjectsInArea(area);
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