#pragma once
#include "PookaState.h"
#include <memory>
#include <vector>
#include <glm/vec3.hpp>
#include <typeinfo> // for typeid

#include "Component.h"

enum class Direction { Up, Down, Left, Right };


class PookaComponent : public dae::Component {

    friend class  PookaNormalState;
    friend class  PookaGhostState;
    friend class  PookaWandering;

public:
    PookaComponent(dae::GameObject* owner);
    ~PookaComponent() override;

    void SetState(std::unique_ptr<PookaState> newState);
    void Update() override;

    // State-specific methods
    void MoveToNextWaypoint();
    bool DetectsPlayer();
    void EnableGhostMode();
    void DisableGhostMode();
    bool ReachedDestination();
    bool CanRemainGhost();
    void ChooseRandomDirection();
    bool SeesPlayer();
    void ChasePlayer();
    bool TryMove(glm::vec3 direction);
    void MoveToNextWaypointTowards(const glm::vec3& targetPosition);
    glm::vec3 FindNearestValidSpot();

    bool ShouldEnterGhostMode();
    void UpdateTimer(); // Method to update the timer

    std::type_info const& GetComponentType() const override { return typeid(PookaComponent); }

private:
    std::unique_ptr<PookaState> m_CurrentState;
    glm::vec3 m_CurrentPosition;
    glm::vec3 m_Destination;
    dae::GameObject* m_Owner;
    std::vector<dae::GameObject*> m_DetectedPlayers;

    float m_GhostModeTimer = 0.0f; // Remaining time for the next ghost mode check
    float m_GhostModeInterval = 10.0f; // Interval in seconds to decide on ghost mode (can be randomized)
    float m_LastGhostModeChange = 0.0f; // Last time ghost mode check occurred
    bool m_GhostModeEnabled = false; // To track if ghost mode is currently enabled

    glm::vec3 m_TargetPlayerPosition; // Target position to move to when in ghost mode
    float m_GhostModePursuitTimer = 0.0f; // Timer for pursuit duration
    float m_GhostModePursuitDuration = 5.0f; // Duration to try and reach the player (in seconds)

};