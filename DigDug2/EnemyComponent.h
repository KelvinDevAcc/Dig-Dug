#pragma once
#include <memory>
#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include "Component.h"
#include "EnemyState.h"

enum class Direction { Up, Down, Left, Right };

class EnemyComponent : public dae::Component {

    friend class EnemyNormalState;
    friend class EnemyGhostState;
    friend class EnemyWandering;
    friend class EnemyPumpedState;
    friend class EnemyDeadState;
    friend class EnemyCrushedState;
    friend class EnemyBreathingFireState;


public:
    // Constructor and destructor
    EnemyComponent(dae::GameObject* owner);
    virtual ~EnemyComponent() override;

    // State management
    virtual void SetState(std::unique_ptr<EnemyState> newState);
    virtual void Update() override;

    // Movement and detection
    virtual void MoveToNextWaypoint();
    virtual bool DetectsPlayer();
    virtual void EnableGhostMode();
    virtual void DisableGhostMode();
    virtual bool ReachedDestination();
    virtual bool CanRemainGhost();
    virtual void ChooseRandomDirection();
    virtual bool SeesPlayer();
    virtual void ChasePlayer();
    virtual bool TryMove(glm::vec3 direction);
    virtual void MoveToNextWaypointTowards(const glm::vec3& targetPosition);
    virtual glm::vec3 FindNearestValidSpot();
    virtual void OnCrushed();

    // Scoring
    static int CalculatePoints(int layer, std::string enemyType);
    static int DetermineLayer(float yPosition);

    // Attack and respawn
    dae::GameObject* GetLastAttacker() const { return m_LastAttacker; }
    virtual void HitByPump(dae::GameObject* lastatacker);
    int GetPumpHits() const { return m_PumpHits; }
    virtual void ReSpawn();

    // Ghost mode handling
    virtual bool ShouldEnterGhostMode();
    virtual void UpdateTimer();
    void StartDeflationTimer();

    // Component type information
    std::type_info const& GetComponentType() const override { return typeid(EnemyComponent); }

protected:
    // State and position management
    std::unique_ptr<EnemyState> m_CurrentState;
    glm::vec3 m_CurrentPosition;
    glm::vec3 m_Destination;
    dae::GameObject* m_Owner;
    std::vector<dae::GameObject*> m_DetectedPlayers;

    Direction m_currentdirection;
    // Ghost mode tracking
    float m_GhostModeTimer;
    float m_GhostModeInterval;
    float m_LastGhostModeChange;
    bool m_GhostModeEnabled;

    // Movement towards player in ghost mode
    glm::vec3 m_TargetPlayerPosition;
    float m_GhostModePursuitTimer;
    float m_GhostModePursuitDuration;

    // Attack tracking
    int m_PumpHits;
    dae::GameObject* m_LastAttacker = nullptr;

    // Deflation timing
    float m_DeflationTimeLimit;
    glm::vec3 m_startPosition;
};
