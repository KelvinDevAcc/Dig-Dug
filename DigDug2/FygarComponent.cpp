#include "FygarComponent.h"

#include <iostream>
#include <random>
#include <limits>
#include <algorithm>

#include "SceneData.h"
#include "GameObject.h"
#include "GameTime.h"
#include "SceneHelpers.h"
#include "PookaComponent.h"

FygarComponent::FygarComponent(dae::GameObject* owner)
    : m_ActiveState(std::make_unique<FygarWandering>()), m_TargetLocation(0, 0, 0), m_EntityOwner(owner),
    m_GhostModeTimer(0.0f), m_GhostModeInterval(10.0f), m_LastGhostModeCheck(0), m_IsGhostModeActive(false),
    m_TargetPlayerLocation(0, 0, 0), m_GhostModePursuitTimer(0.0f), m_GhostModePursuitDuration(5.0f), m_PumpStrikes(0), m_DeflationTimeLimit(0)
{
    m_InitialPosition = m_EntityOwner->GetWorldPosition();
    m_CurrentLocation = m_EntityOwner->GetWorldPosition(); // Initialize the position
    m_ActiveState->Enter(this);
    SetGameObject(m_EntityOwner);
    // Initialize the ghost mode timer with a random interval
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(5.0f, 15.0f); // Interval between 5 and 15 seconds
    m_GhostModeInterval = dis(gen);
    m_GhostModeTimer = m_GhostModeInterval;
}

FygarComponent::~FygarComponent() {
    if (m_ActiveState) {
        m_ActiveState->Exit(this);
    }
}

void FygarComponent::SetState(std::unique_ptr<FygarState> newState) {
    if (m_ActiveState) {
        m_ActiveState->Exit(this);
    }
    m_ActiveState = std::move(newState);
    m_ActiveState->Enter(this);
}

void FygarComponent::Update() {
    if (m_ActiveState) {
        m_ActiveState->Update(this);
    }

    RefreshTimer();
}

void FygarComponent::NavigateToNextWaypoint() {
    const float speed = 50.0f * dae::GameTime::GetDeltaTime(); // Adjust speed as needed
    constexpr float precision = 1.0f; // Precision threshold for reaching destination

    glm::vec3 direction = m_TargetLocation - m_CurrentLocation;
    const float distance = glm::length(direction);

    if (distance > precision) {
        direction = glm::normalize(direction);
        if (!AttemptMove(direction * speed)) {
            // If move fails, try smaller steps
            std::cout << "Failed to move directly, trying smaller steps." << std::endl;
            for (float step = speed / 2; step > 0.1f; step /= 2) {
                if (AttemptMove(direction * step)) {
                    break;
                }
            }
        }
    }
    else {
        m_CurrentLocation = m_TargetLocation;
    }
    m_EntityOwner->SetLocalPosition(m_CurrentLocation); // Update owner's position
}

bool FygarComponent::AttemptMove(glm::vec3 direction) {
    const auto& sceneData = dae::SceneData::GetInstance();
    const glm::vec3 newPosition = m_CurrentLocation + direction;

    if (sceneData.CanEnemyMove(direction.x, direction.y, *m_EntityOwner)) {
        m_CurrentLocation = newPosition;
        return true;
    }
    return false;
}

void FygarComponent::NavigateToNextWaypointTowards(const glm::vec3& targetPosition) {
    const float deltaTime = dae::GameTime::GetDeltaTime();
    const float speed = 50.0f; // Adjust this speed value as needed

    glm::vec3 direction = targetPosition - m_CurrentLocation;
    const float distance = glm::length(direction);

    if (distance > 0.1f) { // Threshold to stop moving
        direction = glm::normalize(direction);
        glm::vec3 movement = direction * speed * deltaTime;

        // Move `Pooka` towards the destination
        if (glm::length(targetPosition - (m_CurrentLocation + movement)) < distance) {
            m_CurrentLocation += movement;
        }
        else {
            // Move directly to the destination if the next step would overshoot
            m_CurrentLocation = targetPosition;
        }

        // Update position
        m_EntityOwner->SetLocalPosition(m_CurrentLocation);
    }
    else {
        // Reached destination
        m_CurrentLocation = targetPosition;
        m_EntityOwner->SetLocalPosition(m_CurrentLocation);
    }
}

glm::ivec2 FygarComponent::ConvertPositionToGrid(const glm::vec3& position)
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

glm::vec3 FygarComponent::ConvertGridToPosition(const glm::ivec2& gridCoords)
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

glm::vec3 FygarComponent::LocateNearestSafeSpot() {
    const float maxSearchDistance = 100.0f; // Maximum distance to search
    const float stepSize = SceneHelpers::GetCellSize().x; // Step size in grid (assumes square cells)

    // Calculate the grid origin in 2D space
    glm::ivec2 currentGridPos = ConvertPositionToGrid(m_EntityOwner->GetWorldPosition());

    float searchRange = stepSize;

    while (searchRange <= maxSearchDistance) {
        // Search in a grid pattern around the grid origin
        for (float x = -searchRange; x <= searchRange; x += stepSize) {
            for (float y = -searchRange; y <= searchRange; y += stepSize) {
                glm::ivec2 potentialGridPos = currentGridPos + glm::ivec2(x / stepSize, y / stepSize);
                glm::vec3 potentialSpot = ConvertGridToPosition(potentialGridPos);

                // Check if the center of the tile is walkable
                if (dae::SceneData::GetInstance().CanEnemyMove(potentialSpot.x, potentialSpot.y, *m_EntityOwner)) {
                    return potentialSpot;
                }
            }
        }

        // Increase the search range
        searchRange += stepSize;
    }

    // If no valid spot was found within the search range
    std::cout << "No valid spot found within the search range." << std::endl;
    return m_CurrentLocation; // Return the current position if no valid spot is found
}


bool FygarComponent::DetectsPlayer() {
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

void FygarComponent::EnableGhostMode() {
    std::cout << "Pooka ghost mode enabled." << std::endl;
    m_GhostModeEnabled = true;
}

void FygarComponent::DisableGhostMode() {
    std::cout << "Pooka ghost mode disabled." << std::endl;
    m_GhostModeEnabled = false;
}

bool FygarComponent::ReachedDestination() {
    constexpr float threshold = 1.0f; // Adjust threshold distance
    const glm::vec3 direction = m_Destination - m_CurrentPosition;
    const float distance = glm::length(direction);

    return distance < threshold;
}

bool FygarComponent::CanRemainGhost() {
    return m_GhostModeEnabled; // Simple example logic, could be expanded
}

void FygarComponent::ChooseRandomDirection() {
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



bool FygarComponent::SeesPlayer() {
    return DetectsPlayer(); // Placeholder for Line of Sight (LOS) checks
}

void FygarComponent::ChasePlayer() {
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

void FygarComponent::UpdateTimer() {
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

bool FygarComponent::ShouldEnterGhostMode() {
    return m_GhostModeEnabled;
}

void FygarComponent::HitByPump(dae::GameObject* lastatacker) {
    ++m_PumpHits;
    if (m_PumpHits <= 4)
    {
        SetState(std::make_unique<FygarPumpedState>());
    }
    m_LastAttacker = lastatacker;
}

void FygarComponent::StartDeflationTimer() {
    m_DeflationTimeLimit = 5.0f; // Set the deflation time limit (e.g., 5 seconds)
}

void FygarComponent::OnCrushed() {
        SetState(std::make_unique<FygarCrushedState>());
}

int FygarComponent::CalculatePoints(int layer, std::string enemyType) {
    if (enemyType == "Pooka") {
        switch (layer) {
        case 1: return 200;
        case 2: return 300;
        case 3: return 400;
        case 4: return 500;
        }
    }
    else if (enemyType == "Fygar") {
        switch (layer) {
        case 1: return 400;
        case 2: return 600;
        case 3: return 800;
        case 4: return 1000;
        }
    }
    return 0;
}

int FygarComponent::DetermineLayer(float yPosition) {
    if (yPosition >= 0 && yPosition < 240) {
        return 1;
    }
    else if (yPosition >= 240 && yPosition < 400) {
        return 2;
    }
    else if (yPosition >= 400 && yPosition < 560) {
        return 3;
    }
    else if (yPosition >= 560) {
        return 4;
    }
    return 0;
}

void FygarComponent::ReSpawn() {
    std::cout << "Pooka respawning at starting position." << std::endl;

    m_CurrentPosition = m_startPosition;
    m_Owner->SetLocalPosition(m_CurrentPosition);
    m_Destination = m_startPosition;
    m_PumpHits = 0;
    // Reset to Wandering state
    SetState(std::make_unique<FygarWandering>());
}