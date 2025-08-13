#pragma once
#include <raylib.h>
#include <vector>
#include <memory>
#include <unordered_set>
#include <unordered_map>
class NavGraphNode;
class Enemy; 
struct Vector2i {
	int x, y;

	bool operator==(const Vector2i& other) const {
		return x == other.x && y == other.y;
	}
	Vector2i operator=(const Vector2i& other)
	{
		this->x = other.x;
		this->y = other.y; 
	}
	bool operator<(const Vector2i& other) const {
		return x < other.x || (x == other.x && y < other.y);
	}
};

namespace std {
	template<>
	struct hash<Vector2i> {
		std::size_t operator()(const Vector2i& v) const {
			return std::hash<int>()(v.x) ^ (std::hash<int>()(v.y) << 1);
		}
	};
};
enum class EdgeDirection
{
	Horizontal, 
	Jumping, 
	Falling, 
	Flying
};
enum class NodeType
{
	Ground, 
	Flying
};
struct Edge
{
	std::shared_ptr<NavGraphNode> toNode;  
	EdgeDirection direction; 
	float euclid_distance;
	int manhattan_distance;
	float getHCost(Enemy* enemy) const;
	Edge(std::shared_ptr<NavGraphNode> target, float distance, EdgeDirection dir, int manhattan_distance)
		: toNode(target), euclid_distance(distance), direction(dir), manhattan_distance(manhattan_distance) {
	}
};
class NavGraphNode
{ 
public: 
	NavGraphNode(Vector2 position);
	const std::vector<Edge>& getNeighbors() const;
	void addNeighbor(std::shared_ptr<NavGraphNode> neighbor, float distance, EdgeDirection dir, int m_distance);
	const Vector2& getPosition() const;
	float getX() const;
	float getY() const;
	void draw(Color color); 
private:
	Vector2 position;
	std::vector<Edge> edges;
};
struct NodeRecord {
	std::shared_ptr<NavGraphNode> node;
	float costSoFar;
	float estimatedTotalCost;
	bool operator>(const NodeRecord& other) const {
		return estimatedTotalCost > other.estimatedTotalCost;
	}
};
class NavGraph
{
private: 
	std::unordered_map<Vector2i, std::shared_ptr<NavGraphNode>> GroundNodes;
	std::unordered_map<Vector2i, std::shared_ptr<NavGraphNode>> FlyingNodes;
	std::unordered_map<Vector2i, bool> constructedRegions;
public : 
	static NavGraph& getInstance(); 
	std::vector<std::shared_ptr<NavGraphNode>> getShortestPath(Enemy* enemy, Vector2 target, NodeType type);
	void buildNodes(Rectangle bound);
	void draw();
	void clear();
	std::shared_ptr<NavGraphNode> getNearestNode(Vector2 pos, NodeType type);
};

