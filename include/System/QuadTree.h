#pragma once

#include <vector>
#include <memory>
#include "raylib.h"
#include "Interface.h"
#include "../Objects/ObjectFactory.h"

struct Node {
	Rectangle bounds;
	std::vector<Object*> objects;
	std::unique_ptr<Node> children[4];
	bool isLeaf = true;
	int level;

	//200-500 objects;
	static const int MAX_OBJECTS = 6;
	static const int MAX_LEVELS = 5;

	Node(Rectangle bounds, int level) : bounds(bounds), level(level) {}
};

class QuadTree {
public:
	QuadTree(Rectangle worldBounds);
	~QuadTree() = default;

	void clear();
	void insert(Object* object);
	std::vector<Object*> retrieve(Object* object);
	std::vector<Object*> retrieve(Rectangle area);

	void getStats(int& totalNodes, int& maxDepth, int& totalObjects) const;
	void drawDebug() const;

private:
	std::unique_ptr<Node> root;
	Rectangle worldBounds;
	void subdivide(Node* node);
	void insertIntoNode(Node* node, Object* object);
	void retrieveFromNode(Node* node, Rectangle area, std::vector<Object*>& result) const;
	int getQuadrant(Node* node, Rectangle objectBounds) const;
	void getStatsRecursive(Node* node, int& totalNodes, int& maxDepth, int& totalObjects) const;
	void drawNodeRecursive(Node* node) const;
};