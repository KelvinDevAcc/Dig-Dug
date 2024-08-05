#pragma once
#include "PookaState.h"
#include <memory>
#include <vector>
#include <glm/vec3.hpp>
#include <typeinfo> // for typeid

#include "Component.h"

enum class Direction { Up, Down, Left, Right };

class PookaComponent : public dae::Component {
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

    std::type_info const& GetComponentType() const override { return typeid(PookaComponent); }

private:
    std::unique_ptr<PookaState> m_CurrentState;
    bool m_GhostModeEnabled = false;
    glm::vec3 m_CurrentPosition;
    glm::vec3 m_Destination;
    dae::GameObject* m_Owner;
    std::vector<dae::GameObject*> m_DetectedPlayers;
};