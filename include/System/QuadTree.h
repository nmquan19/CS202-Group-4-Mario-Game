#pragma once

#include <vector>
#include <memory>
#include "raylib.h"
#include "Interface.h"

class QuadTree {
private:
	struct Node {
		Rectangle bounds;
		std::vector<ICollidable*> objects;
		std::unique_ptr<Node> children[4];
		bool isLeaf = true;
		int level;

		//200-500 objects;
		static const int MAX_OBJECTS = 6;
		static const int MAX_LEVELS = 5;

		Node(Rectangle bounds, int level) : bounds(bounds), level(level) {}
	};
public:
	QuadTree(Rectangle worldBounds);
	~QuadTree() = default;

	void clear();
	void insert(ICollidable* object);
	std::vector<ICollidable*> retrieve(ICollidable* object);
	std::vector<ICollidable*> retrieve(Rectangle area);

	void getStats(int& totalNodes, int& maxDepth, int& totalObjects) const;
	void drawDebug() const;

private:
	std::unique_ptr<Node> root;
	Rectangle worldBounds;
	void subdivide(Node* node);
	void insertIntoNode(Node* node, ICollidable* object);
	void retrieveFromNode(Node* node, Rectangle area, std::vector<ICollidable*>& result) const;
	int getQuadrant(Node* node, Rectangle ojectBounds) const;
	void getStatsRecursive(Node* node, int& totalNodes, int& maxDepth, int& totalObjects) const;
	void drawNodeRecursive(Node* node) const;
};
