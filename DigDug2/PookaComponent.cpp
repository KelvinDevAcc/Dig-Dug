#include "PookaComponent.h"
#include <iostream>
#include <cmath>
#include <random>
#include <limits>
#include <algorithm>

#include "Command.h"
#include "SceneData.h"
#include "GameObject.h"
#include "SceneHelpers.h"

PookaComponent::PookaComponent(dae::GameObject* owner)
    : m_CurrentState(std::make_unique<PookaWandering>()), m_Owner(owner)
{
    m_CurrentPosition = m_Owner->GetWorldPosition(); // Initialize the position
    m_CurrentState->Enter(this);
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

    // Ensure the owner’s position is updated in each frame
    //m_Owner->SetLocalPosition(m_CurrentPosition);
}

void PookaComponent::MoveToNextWaypoint() {
    std::cout << "Pooka is moving to the next waypoint." << std::endl;
    constexpr float speed = 1.0f; // Adjust speed as needed
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
        std::cout << "Reached Destination: ("
            << m_Destination.x << ", "
            << m_Destination.y << ", "
            << m_Destination.z << ")" << std::endl;
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

bool PookaComponent::DetectsPlayer() {
    constexpr float detectionRange = 100.0f; // Adjust detection range as needed
    const auto& sceneData = dae::SceneData::GetInstance();
    const auto& players = sceneData.GetPlayers();

    m_DetectedPlayers.clear(); // Clear previously detected players

    for (const auto& player : players) {
        glm::vec3 playerPosition = player->GetWorldPosition();
        glm::vec3 direction = playerPosition - m_CurrentPosition;
        const float distance = glm::length(direction);

        if (distance <= detectionRange) {
            m_DetectedPlayers.push_back(player);
        }
    }

    return !m_DetectedPlayers.empty();
}

void PookaComponent::EnableGhostMode() {
    std::cout << "Pooka ghost mode enabled." << std::endl;
    m_GhostModeEnabled = true;
    // Additional logic to disable collisions, change appearance, etc.
}

void PookaComponent::DisableGhostMode() {
    std::cout << "Pooka ghost mode disabled." << std::endl;
    m_GhostModeEnabled = false;
    // Additional logic to re-enable collisions, reset appearance, etc.
}

bool PookaComponent::ReachedDestination() {
    constexpr float threshold = 1.0f; // Adjust threshold distance
    const glm::vec3 direction = m_Destination - m_CurrentPosition;
    const float distance = glm::length(direction);

    std::cout << "Current Position: (" << m_CurrentPosition.x << ", " << m_CurrentPosition.y << ", " << m_CurrentPosition.z << ")\n";
    std::cout << "Destination: (" << m_Destination.x << ", " << m_Destination.y << ", " << m_Destination.z << ")\n";
    std::cout << "Distance to Destination: " << distance << " Threshold: " << threshold << std::endl;

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

    glm::vec3 offset;
    bool validDirectionFound = false;

    for (const auto& chosenDirection : shuffledDirections) {
        switch (chosenDirection) {
        case Direction::Up:    offset = glm::vec3(0.0f, SceneHelpers::GetCellSize().y, 0.0f); break;
        case Direction::Down:  offset = glm::vec3(0.0f, -SceneHelpers::GetCellSize().y, 0.0f); break;
        case Direction::Left:  offset = glm::vec3(-SceneHelpers::GetCellSize().x, 0.0f, 0.0f); break;
        case Direction::Right: offset = glm::vec3(SceneHelpers::GetCellSize().x, 0.0f, 0.0f); break;
        }

        const glm::vec3 testPosition = m_CurrentPosition + offset;

        if (dae::SceneData::GetInstance().CanEntityMove(testPosition.x, testPosition.y, *m_Owner)) {
            m_Destination = testPosition;
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
