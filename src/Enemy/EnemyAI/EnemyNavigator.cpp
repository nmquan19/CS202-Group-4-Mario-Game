#include "../../../include/Enemy/EnemyAI/EnemyNavigator.h"
#include "../../../include/System/Constant.h"
#include "../../../include/System/LevelEditor.h"
#include "../../../include/Enemy/Enemy.h"
#include "../../../include/System/Grid.h"
#include "../../../include/System/CameraSystem.h"
#include <raymath.h>
#include <set>
#include <queue>
#include <math.h>
NavGraphNode::NavGraphNode(Vector2 position):position(position)
{

}
const std::vector<Edge>& NavGraphNode::getNeighbors() const { return edges; }
float Edge::getHCost(Enemy* enemy) const
{
    switch (direction)
    {
    case EdgeDirection::Horizontal:
    {
        float velocity = enemy->getWalkVelocity();
        return euclid_distance / velocity;
    }
    break;

    case EdgeDirection::Jumping:
    {
        float maxJumpSpeed = std::fabs(enemy->getJumpVelocity());
        float peak = maxJumpSpeed * maxJumpSpeed /( 2 * 980);
        if (euclid_distance > peak) return _FMAX;
        return euclid_distance/maxJumpSpeed;
    }
    case EdgeDirection::Falling: 
    {
        return sqrt(2 * euclid_distance / 980);
    }
     break;
    default: 
        return 1;
    }
}
void NavGraphNode::addNeighbor(std::shared_ptr<NavGraphNode> neighbor, float distance, EdgeDirection dir,int m_distance)
{
    for (const auto& edge : edges)
    {
        if (edge.toNode == neighbor)
            return; 
    }
   
    
	edges.emplace_back(neighbor, distance, dir, m_distance);
}
const Vector2& NavGraphNode::getPosition() const { return position; }
float NavGraphNode::getX() const { return position.x; }
float NavGraphNode::getY() const { return position.y; }
void NavGraphNode::draw(Color color)
{
    color.a = 128;
	DrawCircle(this->position.x, this->position.y, 20, color);
  
    for (const auto& edge : edges)
    {
        Color eColor; 
        switch (edge.direction)
        {
        case EdgeDirection::Horizontal:
            eColor = BLUE;
            continue;
            break;
        case EdgeDirection::Jumping: 
            eColor = RED;
            break;
        case EdgeDirection::Falling:
            eColor = GREEN; 
            continue;
            break;
        case EdgeDirection::Flying:
            eColor = YELLOW;
            continue;
            break;
        };

        DrawLine(this->position.x, this->position.y, edge.toNode->getX(), edge.toNode->getY(),eColor);
    }
}

NavGraph& NavGraph::getInstance()
{
	static NavGraph graph; 
	return graph;
}

void NavGraph::buildNodes(Rectangle bound) {
    float GRID_SIZE = Constants::TILE_SIZE;
    int startX = static_cast<int>(floor(bound.x / GRID_SIZE));
    int startY = static_cast<int>(floor(bound.y / GRID_SIZE));
    int endX = static_cast<int>(ceil((bound.x + bound.width) / GRID_SIZE));
    int endY = static_cast<int>(ceil((bound.y + bound.height) / GRID_SIZE));

    for (int y = startY; y < endY; ++y) {
        for (int x = startX; x < endX; ++x) {
            Vector2i tileCoord = { x, y };

            if (constructedRegions.contains(tileCoord) || LevelEditor::getInstance().isBlock({ tileCoord.x,tileCoord.y }))
                continue;

            constructedRegions[tileCoord] = true;

            Vector2 worldPos = {
                (x + 0.5f) * GRID_SIZE,
                (y + 0.5f) * GRID_SIZE
            };

            auto flyNode = std::make_shared<NavGraphNode>(worldPos);
            int _dx[8] = { 0,0,-1,-1,-1,1,1,1 };
            int _dy[8] = { 1,-1,0,1,-1,0,1,-1 };
            for (int i = 0; i < 8; i++)
            {
                Vector2i neighborCoord = { x + _dx[i], y + _dy[i]};
                Vector2i neighborCoord_x = { x + _dx[i], y  };
                Vector2i neighborCoord_y = { x , y + _dy[i] };
                if (FlyingNodes.contains(neighborCoord)&& (FlyingNodes.contains(neighborCoord_x)||FlyingNodes.contains(neighborCoord_y))) {
                    float distance = Vector2Distance(flyNode->getPosition(),FlyingNodes[neighborCoord]->getPosition());
                    flyNode->addNeighbor(FlyingNodes[neighborCoord], distance, EdgeDirection::Flying,1);
                    FlyingNodes[neighborCoord]->addNeighbor(flyNode, distance, EdgeDirection::Flying,1);
                }
            }
            FlyingNodes[tileCoord] = flyNode;

            // Ground check logic
            int dr[3] = { 1, 1, 1 };
            int dc[3] = { 0, -1, 1 };
            int dx[2] = { -1, 1 };
            int dy[2] = { 0, 0 };

            for (int i = 0; i < 3; ++i) {
                Vector2i diagBelow = { tileCoord.x + dc[i], tileCoord.y + dr[i] };

                if (LevelEditor::getInstance().isBlock({ diagBelow.x,diagBelow.y })) {
                    if (i == 0) {
                        // Directly under
                        auto groundNode = std::make_shared<NavGraphNode>(worldPos);
                        GroundNodes[tileCoord] = groundNode;
                        // Connect to horizontal ground neighbors
                        for (int j = 0; j < 2; ++j) {
                            Vector2i neighborCoord = { x + dx[j], y };
                            if (GroundNodes.contains(neighborCoord)) {
                                float distance = Vector2Distance(groundNode->getPosition(), GroundNodes[neighborCoord]->getPosition());
                                groundNode->addNeighbor(GroundNodes[neighborCoord],distance,EdgeDirection::Horizontal,1);
                                GroundNodes[neighborCoord]->addNeighbor(groundNode, distance, EdgeDirection::Horizontal,1);
                            }
                        }
                        int _dx[2] = { -1, 1 }; 

                        for (int dir = 0; dir < 2; dir++) {
                            Vector2i sideCheck = { tileCoord.x + _dx[dir], tileCoord.y };
                            if (!LevelEditor::getInstance().isBlock({ sideCheck.x,sideCheck.y })) {
                                int searchLimit = 10; 
                                for (int offset = 1; offset <= searchLimit; ++offset) {
                                    Vector2i searchCoord = { tileCoord.x + _dx[dir] * offset, tileCoord.y };
                                    if (constructedRegions.contains(searchCoord)) {
                                        Vector2 targetPos = {
                                            (searchCoord.x + 0.5f) * Constants::TILE_SIZE,
                                            (searchCoord.y + 0.5f) * Constants::TILE_SIZE
                                        };
                                        if (GroundNodes.contains(searchCoord)) {
                                            float distance = Vector2Distance(groundNode->getPosition(), GroundNodes[searchCoord]->getPosition());
                                            groundNode->addNeighbor(GroundNodes[searchCoord], distance, EdgeDirection::Jumping, offset);
                                            GroundNodes[searchCoord]->addNeighbor(groundNode, distance, EdgeDirection::Jumping, offset);
                                            break;
                                        }
                                    }
                                    else if(LevelEditor::getInstance().isBlock({ searchCoord.x,searchCoord.y }))
                                    {
                                        break; 
                                    }
                                }
                            }
                            int verticalSearchLimit = 30; 
                            for (int offset = 1; offset <= verticalSearchLimit; ++offset) {
                                Vector2i searchCoord = { tileCoord.x, tileCoord.y - offset };
                                if (constructedRegions.contains(searchCoord)) {
                                    Vector2 targetPos = {
                                        (searchCoord.x + 0.5f) * Constants::TILE_SIZE,
                                        (searchCoord.y + 0.5f) * Constants::TILE_SIZE
                                    };
                                    if (GroundNodes.contains(searchCoord)) {
                                        float distance = Vector2Distance(groundNode->getPosition(), GroundNodes[searchCoord]->getPosition());
                                        groundNode->addNeighbor(GroundNodes[searchCoord], distance, EdgeDirection::Jumping, offset);
                                        GroundNodes[searchCoord]->addNeighbor(groundNode, distance, EdgeDirection::Falling, offset);
                                    }
                                }
                                else if (LevelEditor::getInstance().isBlock({ searchCoord.x,searchCoord.y }))
                                {
                                    break;
                                }
                            }
                        }
                        break;
                    }
                    else {
                        // Check side block
                        Vector2i sideCheck = { tileCoord.x + dx[i - 1], tileCoord.y + dy[i - 1] };
                        if (!LevelEditor::getInstance().isBlock({ sideCheck.x,sideCheck.y })) {
                            {
                                // Ledge case
                                auto groundNode = std::make_shared<NavGraphNode>(worldPos);
                                GroundNodes[tileCoord] = groundNode;
                                
                                for (int j = 0; j < 2; ++j) {
                                    Vector2i neighborCoord = { x + dx[j], y };
                                    if (GroundNodes.contains(neighborCoord)) {
                                        float distance = Vector2Distance(groundNode->getPosition(), GroundNodes[neighborCoord]->getPosition());
                                        groundNode->addNeighbor(GroundNodes[neighborCoord], distance, EdgeDirection::Horizontal,1);

                                        GroundNodes[neighborCoord]->addNeighbor(groundNode, distance, EdgeDirection::Horizontal,1);
                                    }
                                }
                                int horizontalSearchLimit = 30;
                                for (int offset = 1; offset <= horizontalSearchLimit; ++offset) {
                                    Vector2i searchCoord = { tileCoord.x, tileCoord.y - offset };
                                    if (constructedRegions.contains(searchCoord)) {
                                        Vector2 targetPos = {
                                            (searchCoord.x + 0.5f) * Constants::TILE_SIZE,
                                            (searchCoord.y + 0.5f) * Constants::TILE_SIZE
                                        };
                                        if (GroundNodes.contains(searchCoord) && LevelEditor::getInstance().isBlock({ searchCoord.x,searchCoord.y +1 })){
                                            float distance = Vector2Distance(groundNode->getPosition(), GroundNodes[searchCoord]->getPosition());
                                            groundNode->addNeighbor(GroundNodes[searchCoord], distance, EdgeDirection::Falling,offset);
                                            GroundNodes[searchCoord]->addNeighbor(groundNode, distance, EdgeDirection::Jumping,offset);
                                        }
                                    }
                                    else if (LevelEditor::getInstance().isBlock({ searchCoord.x,searchCoord.y }))
                                    {
                                        break;
                                    }
                                }
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}

void NavGraph::draw()
{
    for (const auto& [pos, node] : FlyingNodes)
    {
        node->draw(BLUE);
    }
    for (const auto& [pos, node] : GroundNodes)
    {
        node->draw(YELLOW);
    }
}

void NavGraph::clear()
{
    constructedRegions.clear();
    GroundNodes.clear();
    FlyingNodes.clear();
}
std::shared_ptr<NavGraphNode> NavGraph::getNearestNode(Vector2 pos, NodeType type) {
    // Convert world position to grid coordinates
    Vector2i gridPos = { (int)GridSystem::getGridCoord(pos).x, (int)GridSystem::getGridCoord(pos).y };

    const auto& nodes = (type == NodeType::Ground) ? GroundNodes : FlyingNodes;

    auto it = nodes.find(gridPos);
    if (it != nodes.end()) {
        return it->second;
    }

    // BFS setup
    std::queue<Vector2i> queue;
    std::set<Vector2i> visited;
    queue.push(gridPos);
    visited.insert(gridPos);

    const Vector2i directions[] = { {0, 1},{1, 1},{-1, 1}};
    const int maxSearchRadius = 30; 

    while (!queue.empty()) {
        Vector2i current = queue.front();
        queue.pop();

        if (nodes.contains(current)) {
            if (type == NodeType::Ground) {
                if (LevelEditor::getInstance().isBlock({ current.x, current.y + 1 })) {
                    return nodes.at(current);
                }
            }
            else {
                return nodes.at(current);
            }
        }

        for (const auto& dir : directions) {
            Vector2i neighbor = { current.x + dir.x, current.y + dir.y };

            if (visited.contains(neighbor) || !constructedRegions.contains(neighbor)) {
                continue;
            }

            if (type == NodeType::Ground && LevelEditor::getInstance().isBlock({ neighbor.x,neighbor.y })) {
                continue;
            }

            int distance = std::abs(neighbor.x - gridPos.x) + std::abs(neighbor.y - gridPos.y);
            if (distance > maxSearchRadius) {
                continue;
            }

            queue.push(neighbor);
            visited.insert(neighbor);
        }
    }

    return nullptr;
}


std::vector<std::shared_ptr<NavGraphNode>> NavGraph::getShortestPath(Enemy* enemy, Vector2 targetPos, NodeType type) {
    auto& nodes = (type == NodeType::Ground) ? GroundNodes : FlyingNodes;
    Vector2 enemyPos = { enemy->getPosition().x + enemy->getHitBox()[0].width/2, enemy->getPosition().y + enemy->getHitBox()[0].height / 2 };

    std::shared_ptr<NavGraphNode> start = getNearestNode(enemyPos, type);
    std::shared_ptr<NavGraphNode> goal = getNearestNode(targetPos, type);
    if (!start || !goal) return {};

    std::unordered_map<std::shared_ptr<NavGraphNode>, std::shared_ptr<NavGraphNode>> cameFrom;
    std::unordered_map<std::shared_ptr<NavGraphNode>, float> costSoFar;

    auto cmp = [](const std::pair<std::shared_ptr<NavGraphNode>, float>& a,
        const std::pair<std::shared_ptr<NavGraphNode>, float>& b) {
            return a.second > b.second;
        };
    std::priority_queue<
        std::pair<std::shared_ptr<NavGraphNode>, float>,
        std::vector<std::pair<std::shared_ptr<NavGraphNode>, float>>,
        decltype(cmp)
    > frontier(cmp);

    frontier.emplace(start, 0.0f);
    cameFrom[start] = nullptr;
    costSoFar[start] = 0.0f;

    while (!frontier.empty()) {
        auto current = frontier.top().first;
        frontier.pop();

        if (current == goal)
            break;

        for (const Edge& edge : current->getNeighbors()) {
            auto next = edge.toNode;
            float movementCost =  edge.getHCost(enemy);
            if (movementCost >= _FMAX) continue;
            float newCost = costSoFar[current] + movementCost;
            if (!costSoFar.count(next) || newCost < costSoFar[next]) {
                costSoFar[next] = newCost;

                float heuristic = Vector2Distance(next->getPosition(), goal->getPosition())/Constants::TILE_SIZE + edge.manhattan_distance;
                float priority = newCost + heuristic;

                frontier.emplace(next, priority);
                cameFrom[next] = current;
            }
        }
    }

    std::vector<std::shared_ptr<NavGraphNode>> path;
    std::shared_ptr<NavGraphNode> current = goal;
    while (current && current != start) {
            path.push_back(current);
        current = cameFrom[current];
    }

    if (current == start) {
        path.push_back(start);
        std::reverse(path.begin(), path.end());
        return path;
    }
    return {}; 
}

