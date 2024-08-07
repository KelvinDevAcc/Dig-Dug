#include "PookaComponent.h"
#include <iostream>
#include <random>
#include <limits>
#include <algorithm>

#include "EnemyComponent.h"
#include "SceneData.h"
#include "GameObject.h"
#include "GameTime.h"
#include "SceneHelpers.h"

PookaComponent::PookaComponent(dae::GameObject* owner)
    : m_CurrentState(std::make_unique<PookaWandering>()), m_Destination(0, 0, 0), m_Owner(owner),
    m_GhostModeTimer(0.0f), m_GhostModeInterval(0.0f), m_GhostModeEnabled(false), m_GhostModePursuitTimer(0.0f)
{
    m_CurrentPosition = m_Owner->GetWorldPosition(); // Initialize the position
    m_CurrentState->Enter(this);
    SetGameObject(m_Owner);

    // Initialize the ghost mode timer with a random interval
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(5.0f, 15.0f); // Interval between 50 and 150 seconds
    m_GhostModeInterval = dis(gen);
    m_GhostModeTimer = m_GhostModeInterval;
}

PookaComponent::~PookaComponent() {
    if (m_CurrentState) {
        m_CurrentState->Exit(this);
    }
}

void PookaComponent::SetState(std::unique_ptr<PookaState> newState) {
    if (m_CurrentState) {
        m_CurrentState->Exit(this);
    }
    m_CurrentState = std::move(newState);
    m_CurrentState->Enter(this);
}

void PookaComponent::Update() {
    if (m_CurrentState) {
        m_CurrentState->Update(this);
    }

    UpdateTimer();
}

void PookaComponent::MoveToNextWaypoint() {
    const float speed = 50.0f * dae::GameTime::GetDeltaTime(); // Adjust speed as needed
    constexpr float precision = 1.0f; // Precision threshold for reaching destination

    glm::vec3 direction = m_Destination - m_CurrentPosition;
    const float distance = glm::length(direction);

    if (distance > precision) {
        direction = glm::normalize(direction);
        if (!TryMove(direction * speed)) {
            // If move fails, try smaller steps
            std::cout << "Failed to move directly, trying smaller steps." << std::endl;
            for (float step = speed / 2; step > 0.1f; step /= 2) {
                if (TryMove(direction * step)) {
                    break;
                }
            }
        }
    }
    else {
        m_CurrentPosition = m_Destination;
    }
    m_Owner->SetLocalPosition(m_CurrentPosition); // Update owner's position
}

bool PookaComponent::TryMove(glm::vec3 direction) {
    const auto& sceneData = dae::SceneData::GetInstance();
    const glm::vec3 newPosition = m_CurrentPosition + direction;

    if (sceneData.CanEnemyMove(direction.x, direction.y, *m_Owner)) {
        m_CurrentPosition = newPosition;
        return true;
    }
    return false;
}

void PookaComponent::MoveToNextWaypointTowards(const glm::vec3& targetPosition) {
    const float deltaTime = dae::GameTime::GetDeltaTime();
    const float speed = 50.0f; // Adjust this speed value as needed

    glm::vec3 direction = targetPosition - m_CurrentPosition;
    const float distance = glm::length(direction);

    if (distance > 0.1f) { // Threshold to stop moving
        direction = glm::normalize(direction);
        glm::vec3 movement = direction * speed * deltaTime;

        // Move `Pooka` towards the destination
        if (glm::length(targetPosition - (m_CurrentPosition + movement)) < distance) {
            m_CurrentPosition += movement;
        }
        else {
            // Move directly to the destination if the next step would overshoot
            m_CurrentPosition = targetPosition;
        }

        // Update position
        m_Owner->SetLocalPosition(m_CurrentPosition);
    }
    else {
        // Reached destination
        m_CurrentPosition = targetPosition;
        m_Owner->SetLocalPosition(m_CurrentPosition);
    }
}

glm::ivec2 PositionToGrid(const glm::vec3& position)
{
    float gridStartX = SceneHelpers::GetMinCoordinates().x;
    float gridStartY = SceneHelpers::GetMinCoordinates().y;
    float cellSizeX = SceneHelpers::GetCellSize().x;
    float cellSizeY = SceneHelpers::GetCellSize().y;

    return {
        static_cast<int>(std::floor((position.x - gridStartX) / cellSizeX)),
        static_cast<int>(std::floor((position.y - gridStartY) / cellSizeY))
    };
}

glm::vec3 GridToPosition(const glm::ivec2& gridCoords)
{
    float gridStartX = SceneHelpers::GetMinCoordinates().x;
    float gridStartY = SceneHelpers::GetMinCoordinates().y;
    float cellSizeX = SceneHelpers::GetCellSize().x;
    float cellSizeY = SceneHelpers::GetCellSize().y;

    return {
        (cellSizeX * gridCoords.x) + gridStartX,
        (cellSizeY * gridCoords.y) + gridStartY,
        0.0f
    };
}

glm::vec3 PookaComponent::FindNearestValidSpot() {
    const float maxSearchDistance = 100.0f; // Maximum distance to search
    const float stepSize = SceneHelpers::GetCellSize().x; // Step size in grid (assumes square cells)

    // Calculate the grid origin in 2D space
    glm::ivec2 currentGridPos = PositionToGrid(m_Owner->GetWorldPosition());

    float searchRange = stepSize;

    while (searchRange <= maxSearchDistance) {
        // Search in a grid pattern around the grid origin
        for (float x = -searchRange; x <= searchRange; x += stepSize) {
            for (float y = -searchRange; y <= searchRange; y += stepSize) {
                glm::ivec2 potentialGridPos = currentGridPos + glm::ivec2(x / stepSize, y / stepSize);
                glm::vec3 potentialSpot = GridToPosition(potentialGridPos);

                // Check if the center of the tile is walkable
                if (dae::SceneData::GetInstance().CanEnemyMove(potentialSpot.x, potentialSpot.y, *m_Owner)) {
                    return potentialSpot;
                }
            }
        }

        // Increase the search range
        searchRange += stepSize;
    }

    // If no valid spot was found within the search range
    std::cout << "No valid spot found within the search range." << std::endl;
    return m_CurrentPosition; // Return the current position if no valid spot is found
}


bool PookaComponent::DetectsPlayer() {
    const auto& sceneData = dae::SceneData::GetInstance();
    const auto& players = sceneData.GetPlayers();

    m_DetectedPlayers.clear(); // Clear previously detected players

    std::cout << "Detecting players..." << std::endl;

    // Assuming detection is always active (no distance check)
    for (const auto& player : players) {
        // In this simple case, we're just adding all players
        m_DetectedPlayers.push_back(player);

        // For debugging: Print the detected players' positions
        glm::vec3 playerPosition = player->GetWorldPosition();
        std::cout << "Player detected at: (" << playerPosition.x << ", " << playerPosition.y << ", " << playerPosition.z << ")" << std::endl;
    }

    std::cout << "Number of detected players: " << m_DetectedPlayers.size() << std::endl;

    return !m_DetectedPlayers.empty();
}

void PookaComponent::EnableGhostMode() {
    std::cout << "Pooka ghost mode enabled." << std::endl;
    m_GhostModeEnabled = true;
}

void PookaComponent::DisableGhostMode() {
    std::cout << "Pooka ghost mode disabled." << std::endl;
    m_GhostModeEnabled = false;
}

bool PookaComponent::ReachedDestination() {
    constexpr float threshold = 1.0f; // Adjust threshold distance
    const glm::vec3 direction = m_Destination - m_CurrentPosition;
    const float distance = glm::length(direction);

    return distance < threshold;
}

bool PookaComponent::CanRemainGhost() {
    return m_GhostModeEnabled; // Simple example logic, could be expanded
}

void PookaComponent::ChooseRandomDirection() {
    std::cout << "Pooka choosing a random direction." << std::endl;

    const std::vector<Direction> directions = { Direction::Up, Direction::Down, Direction::Left, Direction::Right };

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::vector<Direction> shuffledDirections = directions;
    std::ranges::shuffle(shuffledDirections, gen);

    glm::vec3 stepOffset;
    bool validDirectionFound = false;

    for (const auto& chosenDirection : shuffledDirections) {
        switch (chosenDirection) {
        case Direction::Up:
            stepOffset = glm::vec3(0.0f, SceneHelpers::GetCellSize().y, 0.0f);
            break;
        case Direction::Down:
            stepOffset = glm::vec3(0.0f, -SceneHelpers::GetCellSize().y, 0.0f);
            break;
        case Direction::Left:
            stepOffset = glm::vec3(-SceneHelpers::GetCellSize().x, 0.0f, 0.0f);
            break;
        case Direction::Right:
            stepOffset = glm::vec3(SceneHelpers::GetCellSize().x, 0.0f, 0.0f);
            break;
        }

        glm::vec3 testPosition = m_CurrentPosition + stepOffset;
        glm::vec3 lastValidPosition = m_CurrentPosition;

        while (dae::SceneData::GetInstance().CanEnemyMove(testPosition.x - m_CurrentPosition.x, testPosition.y - m_CurrentPosition.y, *m_Owner)) {
            lastValidPosition = testPosition;
            testPosition += stepOffset;
        }

        if (lastValidPosition != m_CurrentPosition) {
            m_Destination = lastValidPosition;
            validDirectionFound = true;
            std::cout << "New Destination: (" << m_Destination.x << ", " << m_Destination.y << ", " << m_Destination.z << ")\n";
            break;
        }
    }

    if (!validDirectionFound) {
        std::cout << "No valid direction found. Staying in current position." << std::endl;
        m_Destination = m_CurrentPosition;
    }
}



bool PookaComponent::SeesPlayer() {
    return DetectsPlayer(); // Placeholder for Line of Sight (LOS) checks
}

void PookaComponent::ChasePlayer() {
    if (m_DetectedPlayers.empty()) return;

    std::cout << "Pooka is moving towards the nearest player." << std::endl;

    const dae::GameObject* closestPlayer = nullptr;
    float closestDistance = std::numeric_limits<float>::max();

    for (const auto& player : m_DetectedPlayers) {
        glm::vec3 direction = player->GetWorldPosition() - m_CurrentPosition;
        const float distance = glm::length(direction);

        if (distance < closestDistance) {
            closestDistance = distance;
            closestPlayer = player;
        }
    }

    if (closestPlayer) {
        glm::vec3 direction = closestPlayer->GetWorldPosition() - m_CurrentPosition;
        constexpr float speed = 1.5f; // Adjust chase speed

        if (closestDistance > speed) {
            direction = glm::normalize(direction);
            const glm::vec3 newPosition = m_CurrentPosition + direction * speed;

            const auto& sceneData = dae::SceneData::GetInstance();
            if (sceneData.CanEntityMove(newPosition.x, newPosition.y, *m_Owner)) {
                m_CurrentPosition = newPosition;
            }
        }
        else {
            m_CurrentPosition = closestPlayer->GetWorldPosition();
        }
        m_Owner->SetLocalPosition(m_CurrentPosition); // Update owner's position
    }
}

void PookaComponent::UpdateTimer() {
    const float deltaTime = dae::GameTime::GetDeltaTime();

    // Decrement the timer if ghost mode is not enabled
    if (!m_GhostModeEnabled) {
        m_GhostModeTimer -= deltaTime;

        // If the timer has elapsed
        if (m_GhostModeTimer <= 0.0f) {
            // Randomly decide if the Pooka should enter ghost mode
            static std::random_device rd;
            static std::mt19937 gen(rd());
            std::uniform_real_distribution<float> dis(5.0f, 15.0f); // Interval between 50 and 150 seconds
            m_GhostModeInterval = dis(gen);

            // Reset the timer for the next check
            m_GhostModeTimer = m_GhostModeInterval;

            // Randomly decide if the Pooka should enter ghost mode
            // Example: 50% chance to enable ghost mode
            static std::uniform_real_distribution<float> chanceDis(0.0f, 1.0f);
            if (chanceDis(gen) > 0.5f) {
                m_GhostModeEnabled = !m_GhostModeEnabled;
                std::cout << "Pooka ghost mode " << (m_GhostModeEnabled ? "enabled." : "disabled.") << std::endl;
            }
        }
    }
}

bool PookaComponent::ShouldEnterGhostMode() {
    return m_GhostModeEnabled;
}

void PookaComponent::HitByPump() {
    ++m_PumpHits;
    if (m_PumpHits <= 4)
    {
        SetState(std::make_unique<PookaPumpedState>());
    }
}

void PookaComponent::StartDeflationTimer() {
    m_DeflationTimeLimit = 5.0f; // Set the deflation time limit (e.g., 5 seconds)
}

void PookaComponent::OnCrushed() {
        SetState(std::make_unique<PookaCrushedState>());
}