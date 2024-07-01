#include "MrHotDogAIComponent.h"
#include <queue>
#include <unordered_set>
#include "GameObject.h"
#include "SceneData.h"
#include "SceneHelpers.h"

MrHotDogAIComponent::MrHotDogAIComponent(dae::GameObject* owner, std::vector<std::vector<char>>& loadMap)
    : m_map(loadMap)
{
    m_gameObject = owner;
    m_startNode = nullptr;
    m_goalNode = nullptr;
    m_cellSize = SceneHelpers::GetCellSize();
    m_minCoords = SceneHelpers::GetMinCoordinates();
    m_minCoords.x -= m_cellSize.x / 2;
    m_minCoords.y -= m_cellSize.y / 2;
    m_maxCoords = SceneHelpers::GetMaxCoordinates();
    m_maxCoords.x += m_cellSize.x;
    m_maxCoords.y += m_cellSize.y;
    m_numCellsX = static_cast<int>((m_maxCoords.x - m_minCoords.x) / m_cellSize.x);
    m_numCellsY = static_cast<int>((m_maxCoords.y - m_minCoords.y) / m_cellSize.y);

    for (int i = 0; i < m_numCellsY; ++i)
    {
        for (int j = 0; j < m_numCellsX; ++j)
        {
            // Calculate the top-left corner of the cell
            const float x = m_minCoords.x + static_cast<int>(j) * (m_cellSize.x);
            const float y = m_minCoords.y + static_cast<int>(i) * (m_cellSize.y);

            // Create the SDL_Rect for the cell
            auto cellRect = std::make_unique<SDL_Rect>();
            cellRect->x = static_cast<int>(x);
            cellRect->y = static_cast<int>(y);
            cellRect->w = static_cast<int>(m_cellSize.x);
            cellRect->h = static_cast<int>(m_cellSize.y);

            m_gridCells.push_back(std::move(cellRect));
        }
    }

    const auto position = m_gameObject->GetWorldPosition();

    // Calculate the shortest path to Peter using a pathfinding algorithm
    const auto pathToPeter = CalculatePathToPeter(position);

}


std::vector<SDL_Rect*> MrHotDogAIComponent::CalculatePathToPeter(const glm::vec3& position)
{
    m_path.clear();
    const int startX = static_cast<int>((position.x - m_minCoords.x) / m_cellSize.x);
    const int startY = static_cast<int>((position.y - m_minCoords.y) / m_cellSize.y);

    const auto players = dae::SceneData::GetInstance().GetPlayers();
    glm::vec3 closestPlayerPosition = glm::vec3(0, 0, 0);
    float closestDistance = std::numeric_limits<float>::infinity();

    for (const auto& player : players)
    {
        glm::vec3 playerPosition = player->GetWorldPosition();
        float distance = glm::distance(position, playerPosition);
        if (distance < closestDistance)
        {
            closestDistance = distance;
            closestPlayerPosition = playerPosition;
        }
    }

    const int goalX = static_cast<int>((closestPlayerPosition.x - m_minCoords.x) / m_cellSize.x);
    const int goalY = static_cast<int>((closestPlayerPosition.y - m_minCoords.y) / m_cellSize.y);

    std::vector<SDL_Rect*> path;
    std::unordered_set<std::pair<int, int>, PairHash> openSet;
    std::unordered_set<std::pair<int, int>, PairHash> closedSet;
    std::unordered_map<std::pair<int, int>, std::pair<int, int>, PairHash> cameFrom;
    std::unordered_map<std::pair<int, int>, float, PairHash> actualCost;
    std::unordered_map<std::pair<int, int>, float, PairHash> estimatedCost;

    for (int y = 0; y < m_numCellsY; ++y) {
        for (int x = 0; x < m_numCellsX; ++x) {
            actualCost[{x, y}] = std::numeric_limits<float>::infinity();
            estimatedCost[{x, y}] = std::numeric_limits<float>::infinity();
        }
    }

    actualCost[{startX, startY}] = 0;
    estimatedCost[{startX, startY}] = GetHeuristicCost({ startX, startY }, { goalX, goalY });

    openSet.insert({ startX, startY });

    while (!openSet.empty()) {
        // Find the node in the open set with the lowest f-score
        auto currentNode = *std::ranges::min_element(openSet,
            [&](const auto& a, const auto& b) { return estimatedCost[a] < estimatedCost[b]; });

        // Check if the current node is the goal node
        if (currentNode == std::make_pair(goalX, goalY)) {
            // Reconstruct the path from the start node to the goal node
            path = ReconstructPath(cameFrom, { goalX, goalY });
            break; // Exit the loop since we found the goal node
        }

        // Remove the current node from the open set and add it to the closed set
        openSet.erase(currentNode);
        closedSet.insert(currentNode);

        // Iterate through the neighbors of the current node
        for (auto neighbor : GetNeighbors(currentNode)) {
            // Skip if the neighbor is in the closed set or is not walkable
            if (closedSet.contains(neighbor) || !IsCellWalkable(neighbor.first, neighbor.second))
                continue;

            // Calculate the tentative g-score for reaching the neighbor through the current node
            float tentativeGScore = actualCost[currentNode] + GetCost(neighbor);

            // If the neighbor is not in the open set, add it
            if (!openSet.contains(neighbor))
                openSet.insert(neighbor);
            // If the neighbor is already in the open set and the tentative g-score is not better, skip
            else if (tentativeGScore >= actualCost[neighbor])
                continue;

            // Update the cameFrom map, g-score, and f-score for the neighbor
            cameFrom[neighbor] = currentNode;
            actualCost[neighbor] = tentativeGScore;
            estimatedCost[neighbor] = actualCost[neighbor] + GetHeuristicCost(neighbor, { goalX, goalY });
        }
    }

    return path;
}

void MrHotDogAIComponent::Update()
{
    const auto position = m_gameObject->GetWorldPosition();

    const auto pathToPeter = CalculatePathToPeter(position);

    for (SDL_Rect* rectPtr : pathToPeter)
    {
        m_path.push_back(std::make_unique<SDL_Rect>(*rectPtr));
    }

   if (!m_path.empty())
   {
       // Get the next position in the path
       const SDL_Rect* nextPosition = m_path.front().get();
       const glm::vec3 nextPositionWorld(nextPosition->x + nextPosition->w / 2.0f, nextPosition->y + nextPosition->h / 2.0f, 0.0f); // Center of the next position

       // Calculate movement direction
       const glm::vec3 direction = glm::normalize(nextPositionWorld - position);

       const float movementAmount = m_movementSpeed * dae::GameTime::GetDeltaTime();

       m_moveDirection = direction;

       const glm::vec3 newPosition = position + direction * movementAmount;
       m_gameObject->SetLocalPosition(newPosition);

       // Check if the character has reached the next position
       if (glm::distance(newPosition, nextPositionWorld) < movementAmount)
       {
           // Remove the reached position from the path
           m_path.erase(m_path.begin());
       }
   }
       
}

glm::vec3 MrHotDogAIComponent::GetMoveDirection() const
{
    return m_moveDirection;
}

bool MrHotDogAIComponent::IsCellWalkable(int x, int y) const {
    if (x < 0 || x >= m_numCellsX || y < 0 || y >= m_numCellsY) {
        return false;
    }

    switch (const char tile = m_map[y][x]) {
    case 'v':
    case '#':
    case '^':
    case '|':
    case '_':
        return true;
    default:
        return false;
    }
}

std::vector<std::pair<int, int>> MrHotDogAIComponent::GetNeighbors(const std::pair<int, int>& node) const {
    std::vector<std::pair<int, int>> neighbors;

    // Define possible movements: up, down, left, right
    const std::vector<std::pair<int, int>> directions = {
        {0, -1}, {0, 1}, {-1, 0}, {1, 0}
    };

    // Check each direction for valid neighbors
    for (const auto& [dx, dy] : directions) {
        const int newX = node.first + dx;
        const int newY = node.second + dy;

        if (IsCellWalkable(newX, newY)) {
            neighbors.emplace_back(newX, newY);
        }
    }

    return neighbors;
}

std::vector<SDL_Rect*> MrHotDogAIComponent::ReconstructPath(const std::unordered_map<std::pair<int, int>, std::pair<int, int>, PairHash>& cameFrom, const std::pair<int, int>& current) const
{
    std::vector<SDL_Rect*> path;
    std::pair<int, int> currentPair = current;

    while (cameFrom.count(currentPair) > 0) {
        path.push_back(m_gridCells[currentPair.second * m_numCellsX + currentPair.first].get());
        currentPair = cameFrom.at(currentPair);
    }

    std::reverse(path.begin(), path.end());

    return path;
}


float MrHotDogAIComponent::GetCost(const std::pair<int, int>& next) const {
    int x = next.first;
    int y = next.second;

    // Get the tile type of the next cell
    char tile = m_map[y][x];

    // Assign costs based on tile type
    float baseCost = 1.0f; // Default cost for non-ladder tiles
    if (tile == 'v' || tile == '^' || tile == '|') {
        baseCost = 0.5f;
    }

    return baseCost;
}

float MrHotDogAIComponent::GetHeuristicCost(const std::pair<int, int>& pNodeA, const std::pair<int, int>& pNodeB)
{
	const int dx = abs(pNodeA.first - pNodeB.first);
	const int dy = abs(pNodeA.second - pNodeB.second);
    return static_cast<float>(dx + dy);
}

