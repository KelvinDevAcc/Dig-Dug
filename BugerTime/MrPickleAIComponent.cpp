#include "MrPickleAIComponent.h"
#include <random>
#include <unordered_set>
#include "GameObject.h"
#include "GameTime.h"
#include "SceneData.h"
#include "SceneHelpers.h"
#include <glm/gtx/rotate_vector.hpp>

MrPickleAIComponent::MrPickleAIComponent(dae::GameObject* owner, std::vector<std::vector<char>>& loadMap)
    : m_flipTimer(0.0f), m_flipDuration(4.0f), m_map(loadMap)
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

std::vector<SDL_Rect*> MrPickleAIComponent::CalculatePathToPeter(const glm::vec3& position)
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
    std::unordered_map<std::pair<int, int>, float, PairHash> gScore;
    std::unordered_map<std::pair<int, int>, float, PairHash> fScore;

    for (int y = 0; y < m_numCellsY; ++y) {
        for (int x = 0; x < m_numCellsX; ++x) {
            gScore[{x, y}] = std::numeric_limits<float>::infinity();
            fScore[{x, y}] = std::numeric_limits<float>::infinity();
        }
    }

    gScore[{startX, startY}] = 0;
    fScore[{startX, startY}] = GetHeuristicCost({ startX, startY }, { goalX, goalY });

    openSet.insert({ startX, startY });

    while (!openSet.empty()) {
        auto currentNode = *std::ranges::min_element(openSet,
            [&](const auto& a, const auto& b) { return fScore[a] < fScore[b]; });

        if (currentNode == std::make_pair(goalX, goalY)) {
            path = ReconstructPath(cameFrom, { goalX, goalY });
            break;
        }

        openSet.erase(currentNode);
        closedSet.insert(currentNode);

        for (auto neighbor : GetNeighbors(currentNode)) {
            if (closedSet.contains(neighbor) || !IsCellWalkable(neighbor.first, neighbor.second))
                continue;

            float tentativeGScore = gScore[currentNode] + GetCost(neighbor);

            if (!openSet.contains(neighbor))
                openSet.insert(neighbor);
            else if (tentativeGScore >= gScore[neighbor])
                continue;

            cameFrom[neighbor] = currentNode;
            gScore[neighbor] = tentativeGScore;
            fScore[neighbor] = gScore[neighbor] + GetHeuristicCost(neighbor, { goalX, goalY });
        }
    }

    return path;
}

void MrPickleAIComponent::Update()
{
    const auto position = m_gameObject->GetWorldPosition();

    const auto pathToPeter = CalculatePathToPeter(position);

    for (SDL_Rect* rectPtr : pathToPeter)
    {
        m_path.push_back(std::make_unique<SDL_Rect>(*rectPtr));
    }

    if (!m_path.empty())
    {
        SDL_Rect* nextPosition = m_path.front().get();
        glm::vec3 nextPositionWorld(nextPosition->x + nextPosition->w / 2.0f, nextPosition->y + nextPosition->h / 2.0f, 0.0f); // Center of the next position

        glm::vec3 direction = glm::normalize(nextPositionWorld - position);

        // Introduce randomness to movement direction
        if (m_flipTimer <= 0.0f)
        {
            // Generate random angle to flip direction along the x-y plane
            static std::default_random_engine engine(std::random_device{}());
            static std::uniform_real_distribution<float> distribution(-glm::pi<float>() / 2.0f, glm::pi<float>() / 2.0f); // Adjust the range as needed
            const float angle = distribution(engine);

            // Rotate direction vector in the x-y plane
            glm::vec2 directionXY(direction.x, direction.y);
            directionXY = glm::rotate(directionXY, angle);

            direction.x = directionXY.x;
            direction.y = directionXY.y;

            m_flipTimer = m_flipDuration;
        }

        m_moveDirection = direction;

        const float movementAmount = m_movementSpeed * dae::GameTime::GetDeltaTime();

        const glm::vec3 newPosition = position + direction * movementAmount;
        m_gameObject->SetLocalPosition(newPosition);

        if (glm::distance(newPosition, nextPositionWorld) < movementAmount)
        {
            m_path.erase(m_path.begin());
        }

        m_flipTimer -= dae::GameTime::GetDeltaTime();
    }
}

bool MrPickleAIComponent::IsCellWalkable(int x, int y) const {
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

std::vector<std::pair<int, int>> MrPickleAIComponent::GetNeighbors(const std::pair<int, int>& node) const {
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

std::vector<SDL_Rect*> MrPickleAIComponent::ReconstructPath(const std::unordered_map<std::pair<int, int>, std::pair<int, int>, PairHash>& cameFrom, const std::pair<int, int>& current) const
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


float MrPickleAIComponent::GetCost(const std::pair<int, int>& next) const {
	const int x = next.first;
    const int y = next.second;

    const char tile = m_map[y][x];

    // Assign costs based on tile type
    float baseCost = 1.0f; 
    if (tile == 'v' || tile == '^' || tile == '|') {
        baseCost = 0.5f; 
    }

    return baseCost;
}

float MrPickleAIComponent::GetHeuristicCost(const std::pair<int, int>& pNodeA, const std::pair<int, int>& pNodeB)
{
    int dx = abs(pNodeA.first - pNodeB.first);
    int dy = abs(pNodeA.second - pNodeB.second);
    return static_cast<float>(dx + dy);
}

