#pragma once
#include "PookaState.h"
#include <memory>
#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include "Component.h"

enum class Direction { Up, Down, Left, Right };


class PookaComponent : public dae::Component {

    friend class  PookaNormalState;
    friend class  PookaGhostState;
    friend class  PookaWandering;
    friend class  PookaPumpedState;
    friend class  PookaDeadState;
    friend class  PookaCrushedState;

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
    void OnCrushed();
    static int CalculatePoints(int layer, std::string enemyType);
    static int DetermineLayer(float yPosition);

    dae::GameObject* GetLastAttacker() const { return m_LastAttacker; }
    void HitByPump(dae::GameObject*  lastatacker);
    int GetPumpHits() const { return m_PumpHits; }

    bool ShouldEnterGhostMode();
    void UpdateTimer(); // Method to update the timer

    void ReSpawn();
    std::type_info const& GetComponentType() const override { return typeid(PookaComponent); }

private:
    std::unique_ptr<PookaState> m_CurrentState;
    glm::vec3 m_CurrentPosition;
    glm::vec3 m_Destination;
    dae::GameObject* m_Owner;
    std::vector<dae::GameObject*> m_DetectedPlayers;

    float m_GhostModeTimer; // Remaining time for the next ghost mode check
    float m_GhostModeInterval; // Interval in seconds to decide on ghost mode (can be randomized)
    float m_LastGhostModeChange; // Last time ghost mode check occurred
    bool m_GhostModeEnabled; // To track if ghost mode is currently enabled

    glm::vec3 m_TargetPlayerPosition; // Target position to move to when in ghost mode
    float m_GhostModePursuitTimer; // Timer for pursuit duration
    float m_GhostModePursuitDuration; // Duration to try and reach the player (in seconds)
    int m_PumpHits;

    float m_DeflationTimeLimit; // Time limit for deflation
    void StartDeflationTimer();

    glm::vec3 m_startPosition;
    dae::GameObject* m_LastAttacker = nullptr;
};