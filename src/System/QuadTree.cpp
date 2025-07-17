#include <algorithm>
#include "../../include/System/QuadTree.h"
#include <memory>
#include <vector>
#include "../../include/Objects/ObjectFactory.h"
#include "../../include/System/Interface.h"
#include <raylib.h>

QuadTree::QuadTree(Rectangle worldBounds) : worldBounds(worldBounds) {
	root = std::make_unique<Node>(worldBounds, 0);
}

void QuadTree::clear() {
	root = std::make_unique<Node>(worldBounds, 0);
}

void QuadTree::insert(std::shared_ptr<Object> object) {
	if (!object || !object->isActive()) return;
	Rectangle objBounds = object->getHitBox();
	if (!CheckCollisionRecs(objBounds, worldBounds)) return;
	insertIntoNode(root.get(), object);
}

void QuadTree::insertIntoNode(Node* node, std::shared_ptr<Object> object) {
	Rectangle objBounds = object->getHitBox();
	if (!node->isLeaf) {
		int quadrant = getQuadrant(node, objBounds);
		if (quadrant != -1) {
			insertIntoNode(node->children[quadrant].get(), object);
			return;
		}
	}

	node->objects.push_back(object);

	if (node->isLeaf &&
		node->objects.size() > Node::MAX_OBJECTS &&
		node->level < Node::MAX_LEVELS) {

		subdivide(node);

		auto objects = node->objects;
		node->objects.clear();
		for (auto obj : objects) {
			insertIntoNode(node, obj);
		}
	}
}

void QuadTree::subdivide(Node* node) {
	float halfWidth = node->bounds.width / 2.0f;
	float halfHeight = node->bounds.height / 2.0f;
	float x = node->bounds.x;
	float y = node->bounds.y;

	node->children[0] = std::make_unique<Node>(
		Rectangle{ x, y, halfWidth, halfHeight }, node->level + 1);
	node->children[1] = std::make_unique<Node>(
		Rectangle{ x + halfWidth, y, halfWidth, halfHeight }, node->level + 1);
	node->children[2] = std::make_unique<Node>(
		Rectangle{ x, y + halfHeight, halfWidth, halfHeight }, node->level + 1);
	node->children[3] = std::make_unique<Node>(
		Rectangle{ x + halfWidth, y + halfHeight, halfWidth, halfHeight }, node->level + 1);

	node->isLeaf = false;
}

int QuadTree::getQuadrant(Node* node, Rectangle objectBounds) const {
	float midX = node->bounds.x + node->bounds.width / 2.0f;
	float midY = node->bounds.y + node->bounds.height / 2.0f;

	bool topHalf = (objectBounds.y >= node->bounds.y && objectBounds.y + objectBounds.height <= midY);
	bool botHalf = (objectBounds.y >= midY);
	bool leftHalf = (objectBounds.x >= node->bounds.x && objectBounds.x + objectBounds.width <= midX);
	bool rightHalf = (objectBounds.x >= midX);

	if (topHalf && leftHalf) return 0;
	if (topHalf && rightHalf) return 1;
	if (botHalf && leftHalf) return 2;
	if (botHalf && rightHalf) return 3;
	return -1;
}

std::vector<std::shared_ptr<Object>> QuadTree::retrieve(std::shared_ptr<Object> object) {
	return retrieve(object->getHitBox());
}

std::vector<std::shared_ptr<Object>> QuadTree::retrieve(Rectangle area) { 
	std::vector<std::shared_ptr<Object>> result;
	retrieveFromNode(root.get(), area, result);

	std::sort(result.begin(), result.end(), 
	[](const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b) {
		return a.get() < b.get();
	});
	result.erase(std::unique(result.begin(), result.end(),
	[](const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b) {
		return a.get() == b.get();
	}), result.end());

	return result;
}

void QuadTree::retrieveFromNode(Node* node, Rectangle area, std::vector<std::shared_ptr<Object>>& result) const {
	for (auto obj : node->objects) {
		if (CheckCollisionRecs(obj->getHitBox(), area)) {
			result.push_back(obj);
		}
	}
	if (!node->isLeaf) {
		for (int i = 0; i < 4; i++) {
			if (CheckCollisionRecs(area, node->children[i]->bounds)) {
				retrieveFromNode(node->children[i].get(), area, result);
			}
		}
	}
}

void QuadTree::getStats(int& totalNodes, int& maxDepth, int& totalObjects) const {
	totalNodes = 0;
	maxDepth = 0;
	totalObjects = 0;
	getStatsRecursive(root.get(), totalNodes, maxDepth, totalObjects);
}

void QuadTree::getStatsRecursive(Node* node, int& totalNodes, int& maxDepth, int& totalObjects) const {
	totalNodes++;
	maxDepth = std::max(maxDepth, node->level);
	totalObjects += (int)node->objects.size();

	if (!node->isLeaf) {
		for (int i = 0; i < 4; i++) {
			getStatsRecursive(node->children[i].get(), totalNodes, maxDepth, totalObjects);
		}
	}
}

void QuadTree::drawDebug() const {
	drawNodeRecursive(root.get());
}

void QuadTree::drawNodeRecursive(Node* node) const {
	Color color = BLACK;
	switch (node->level) {
	case 0: color = RED; break;
	case 1: color = BLUE; break;
	case 2: color = GREEN; break;
	case 3: color = YELLOW; break;
	case 4: color = ORANGE; break;
	default: color = PURPLE; break;
	}
	DrawRectangleLinesEx(node->bounds, 2, color);
	for (auto obj : node->objects) {
		if (obj && obj->isActive()) {
			Rectangle hitBox = obj->getHitBox();
			Color hitboxColor = BLACK;
			switch (obj->getObjectCategory()) {
			case ObjectCategory::CHARACTER:
				hitboxColor = LIME;
				break;
			case ObjectCategory::ENEMY:
				hitboxColor = MAGENTA;
				break;
			case ObjectCategory::BLOCK:
				hitboxColor = BLUE;
				break;
			case ObjectCategory::ITEM:
				hitboxColor = GOLD;
				break;
			case ObjectCategory::PROJECTILE:
				hitboxColor = ORANGE;
				break;
			case ObjectCategory::TRIGGER:
				hitboxColor = VIOLET;
				break;
			default:
				hitboxColor = LIGHTGRAY;
				break;
			}
			DrawRectangleLinesEx(hitBox, 3, hitboxColor);
		}
	}
	if (!node->isLeaf) {
		for (int i = 0; i < 4; i++) {
			drawNodeRecursive(node->children[i].get());
		}
	}
}