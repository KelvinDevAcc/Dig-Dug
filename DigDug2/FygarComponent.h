#pragma once
#include "FygarsState.h"
#include <memory>
#include <string>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "Component.h"



class FygarComponent final : public dae::Component {

    friend class  FygarNormalState;
    friend class  FygarGhostState;
    friend class  FygarWandering;
    friend class  FygarPumpedState;
    friend class  FygarDeadState;
    friend class  FygarCrushedState;

public:
    FygarComponent(dae::GameObject* owner);
    ~FygarComponent() override;

    void SetState(std::unique_ptr<FygarState> newState);
    void Update() override;

    void NavigateToNextWaypoint();
    bool CanDetectPlayer();
    void ActivateGhostMode();
    void DeactivateGhostMode();
    bool HasReachedDestination();
    bool IsGhostModeAllowed();
    void PickRandomDirection();
    bool IsPlayerVisible();
    void PursuePlayer();
    bool AttemptMove(glm::vec3 direction);
    void NavigateToNextWaypointTowards(const glm::vec3& targetPosition);
    glm::vec3 LocateNearestSafeSpot();
    void OnSquished();
    static int CalculateScore(int layer, std::string enemyType);
    static int DetermineLayer(float yPos);

    dae::GameObject* GetRecentAttacker() const { return m_RecentAttacker; }
    void AttackedByPump(dae::GameObject* attacker);
    int GetPumpStrikes() const { return m_PumpStrikes; }
    glm::ivec2 ConvertPositionToGrid(const glm::vec3& position);
    glm::vec3 ConvertGridToPosition(const glm::ivec2& gridCoords);

    bool ShouldTriggerGhostMode();
    void RefreshTimer(); // Method to refresh the timer

    void Respawn();
    std::type_info const& GetComponentType() const override { return typeid(FygarComponent); }

private:
    std::unique_ptr<FygarState> m_ActiveState;
    glm::vec3 m_CurrentLocation;
    glm::vec3 m_TargetLocation;
    dae::GameObject* m_EntityOwner;
    std::vector<dae::GameObject*> m_DetectedEntities;

    float m_GhostModeTimer; // Remaining time for the next ghost mode check
    float m_GhostModeInterval; // Interval in seconds to decide on ghost mode (can be randomized)
    float m_LastGhostModeCheck; // Last time ghost mode check occurred
    bool m_IsGhostModeActive; // To track if ghost mode is currently active

    glm::vec3 m_TargetPlayerLocation; // Target location to move to when in ghost mode
    float m_GhostModePursuitTimer; // Timer for pursuit duration
    float m_GhostModePursuitDuration; // Duration to try and reach the player (in seconds)
    int m_PumpStrikes;

    float m_DeflationTimeLimit; // Time limit for deflation
    void InitiateDeflationTimer();

    glm::vec3 m_InitialPosition;
    dae::GameObject* m_RecentAttacker = nullptr;
};