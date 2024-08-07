#include "PookaState.h"
#include "PookaComponent.h"
#include <iostream>

#include "AnimationComponent.h"
#include "GameObject.h"
#include "GameTime.h"

void PookaNormalState::Enter(PookaComponent* /*pooka*/) {
    std::cout << "Pooka entered Normal State" << std::endl;
}

void PookaNormalState::Update(PookaComponent* pooka) {
    pooka->MoveToNextWaypoint();
    std::cout << "Pooka is patrolling" << std::endl;

    if (pooka->DetectsPlayer()) {
        pooka->SetState(std::make_unique<PookaChaseState>());
    }

    if (pooka->ShouldEnterGhostMode()) {
        pooka->SetState(std::make_unique<PookaGhostState>());
    }
}

void PookaNormalState::Exit(PookaComponent* /*pooka*/) {
    std::cout << "Pooka exiting Normal State" << std::endl;
}

void PookaGhostState::Enter(PookaComponent* pooka) {
    std::cout << "Pooka entered Ghost State" << std::endl;
    pooka->EnableGhostMode();
    pooka->m_Owner->GetComponent<dae::AnimationComponent>()->Play("Ghost",true);

    pooka->DetectsPlayer();

    if (!pooka->m_DetectedPlayers.empty()) {
        pooka->m_TargetPlayerPosition = pooka->m_DetectedPlayers.front()->GetWorldPosition();
        pooka->m_GhostModePursuitTimer = pooka->m_GhostModePursuitDuration;
    }
}

void PookaGhostState::Update(PookaComponent* pooka) {
    std::cout << "Pooka is in ghost mode" << std::endl;

    if (pooka->m_TargetPlayerPosition != glm::vec3(0, 0, 0)) {
        glm::vec3 direction = pooka->m_TargetPlayerPosition - pooka->m_CurrentPosition;
        const float distance = glm::length(direction);

        if (distance > 1.0f) {
            // Move towards the player
            pooka->MoveToNextWaypointTowards(pooka->m_TargetPlayerPosition);
        }
        else {
            std::cout << "Pooka reached the player." << std::endl;
            pooka->m_TargetPlayerPosition = glm::vec3(0, 0, 0); // Reset target position
        }
    }

    // Decrement the pursuit timer
    pooka->m_GhostModePursuitTimer -= dae::GameTime::GetDeltaTime();

    if (pooka->m_GhostModePursuitTimer <= 0.0f) {
        // Time has elapsed, move to the closest valid space
        const glm::vec3 nearestValidSpot = pooka->FindNearestValidSpot();
        if (nearestValidSpot != pooka->m_CurrentPosition) {
            pooka->m_Destination = nearestValidSpot;
            pooka->MoveToNextWaypointTowards(pooka->m_Destination);
            std::cout << "Moving to nearest valid spot: (" << nearestValidSpot.x << ", " << nearestValidSpot.y << ", " << nearestValidSpot.z << ")\n";
        }
        else {
            std::cout << "No valid spot found to move to." << std::endl;
            pooka->m_Destination = pooka->m_CurrentPosition; // Stay in current position if no valid spot
        }

        // Update to continue moving towards the nearest valid spot
        pooka->MoveToNextWaypointTowards(pooka->m_Destination);

        // Check if the destination has been reached
        if (pooka->ReachedDestination()) {
            std::cout << "Pooka reached the nearest valid spot." << std::endl;
            pooka->SetState(std::make_unique<PookaWandering>());
        }
    }
}


void PookaGhostState::Exit(PookaComponent* pooka) {
    std::cout << "Pooka exiting Ghost State" << std::endl;
    pooka->DisableGhostMode();
}

void PookaWandering::Enter(PookaComponent* pooka) {
    pooka->ChooseRandomDirection();
    pooka->m_Owner->GetComponent<dae::AnimationComponent>()->Play("Walk_Up", true);
}

void PookaWandering::Update(PookaComponent* pooka) {
    if (pooka->ReachedDestination())
    {
        pooka->ChooseRandomDirection();
    }
    else
    {
        pooka->MoveToNextWaypoint();
          /*if (pooka->SeesPlayer()) {
              pooka->SetState(std::make_unique<PookaChaseState>());
          }*/

          if (pooka->ShouldEnterGhostMode()) {
              pooka->SetState(std::make_unique<PookaGhostState>());
          }
    }
}

void PookaWandering::Exit(PookaComponent* /*pooka*/) {
}

void PookaChaseState::Enter(PookaComponent* /*pooka*/) {
    std::cout << "Pooka entered Chase State" << std::endl;
    
}

void PookaChaseState::Update(PookaComponent* pooka) {
    std::cout << "Pooka is chasing the player" << std::endl;
    pooka->ChasePlayer();

    if (!pooka->DetectsPlayer()) {
        pooka->SetState(std::make_unique<PookaWandering>());
    }
}

void PookaChaseState::Exit(PookaComponent* /*pooka*/) {
    std::cout << "Pooka exiting Chase State" << std::endl;
}
