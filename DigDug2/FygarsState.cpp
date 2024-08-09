
#include "FygarsState.h"

#include <iostream>

#include "AnimationComponent.h"
#include "FygarComponent.h"
#include "GameData.h"
#include "GameTime.h"
#include "PointComponent.h"
#include "SceneData.h"
#include "SceneManager.h"

void FygarNormalState::Enter(FygarComponent* /*Fygar*/) {
    std::cout << "Fygar entered Normal State" << std::endl;
}

void FygarNormalState::Update(FygarComponent* Fygar) {
    Fygar->MoveToNextWaypoint();
    std::cout << "Fygar is patrolling" << std::endl;

    if (Fygar->ShouldEnterGhostMode()) {
        Fygar->SetState(std::make_unique<FygarGhostState>());
    }
}

void FygarNormalState::Exit(FygarComponent* /*Fygar*/) {
    std::cout << "Fygar exiting Normal State" << std::endl;
}

void FygarGhostState::Enter(FygarComponent* Fygar) {
    std::cout << "Fygar entered Ghost State" << std::endl;
    Fygar->EnableGhostMode();
    Fygar->m_Owner->GetComponent<dae::AnimationComponent>()->Play("Ghost",true);

    Fygar->DetectsPlayer();

    if (!Fygar->m_DetectedPlayers.empty()) {
        Fygar->m_TargetPlayerPosition = Fygar->m_DetectedPlayers.front()->GetWorldPosition();
        Fygar->m_GhostModePursuitTimer = Fygar->m_GhostModePursuitDuration;
    }
}

void FygarGhostState::Update(FygarComponent* Fygar) {
    std::cout << "Fygar is in ghost mode" << std::endl;

    if (Fygar->m_TargetPlayerPosition != glm::vec3(0, 0, 0)) {
        glm::vec3 direction = Fygar->m_TargetPlayerPosition - Fygar->m_CurrentPosition;
        const float distance = glm::length(direction);

        if (distance > 1.0f) {
            // Move towards the player
            Fygar->MoveToNextWaypointTowards(Fygar->m_TargetPlayerPosition);
        }
        else {
            std::cout << "Fygar reached the player." << std::endl;
            Fygar->m_TargetPlayerPosition = glm::vec3(0, 0, 0); // Reset target position
        }
    }

    // Decrement the pursuit timer
    Fygar->m_GhostModePursuitTimer -= dae::GameTime::GetDeltaTime();

    if (Fygar->m_GhostModePursuitTimer <= 0.0f) {
        // Time has elapsed, move to the closest valid space
        const glm::vec3 nearestValidSpot = Fygar->FindNearestValidSpot();
        if (nearestValidSpot != Fygar->m_CurrentPosition) {
            Fygar->m_Destination = nearestValidSpot;
            Fygar->MoveToNextWaypointTowards(Fygar->m_Destination);
            std::cout << "Moving to nearest valid spot: (" << nearestValidSpot.x << ", " << nearestValidSpot.y << ", " << nearestValidSpot.z << ")\n";
        }
        else {
            std::cout << "No valid spot found to move to." << std::endl;
            Fygar->m_Destination = Fygar->m_CurrentPosition; // Stay in current position if no valid spot
        }

        // Update to continue moving towards the nearest valid spot
        Fygar->MoveToNextWaypointTowards(Fygar->m_Destination);

        // Check if the destination has been reached
        if (Fygar->ReachedDestination()) {
            std::cout << "Fygar reached the nearest valid spot." << std::endl;
            Fygar->SetState(std::make_unique<FygarWandering>());
        }
    }
}


void FygarGhostState::Exit(FygarComponent* Fygar) {
    std::cout << "Fygar exiting Ghost State" << std::endl;
    Fygar->DisableGhostMode();
}

void FygarWandering::Enter(FygarComponent* Fygar) {
    Fygar->ChooseRandomDirection();
    Fygar->m_Owner->GetComponent<dae::AnimationComponent>()->Play("Walk_Up", true);
}

void FygarWandering::Update(FygarComponent* Fygar) {
    if (Fygar->ReachedDestination())
    {
        Fygar->ChooseRandomDirection();
    }
    else
    {
        Fygar->MoveToNextWaypoint();
          /*if (pooka->SeesPlayer()) {
              pooka->SetState(std::make_unique<PookaChaseState>());
          }*/

          if (Fygar->ShouldEnterGhostMode()) {
              Fygar->SetState(std::make_unique<FygarGhostState>());
          }
    }
}

void FygarWandering::Exit(FygarComponent* /*Fygar*/) {
}

void FygarPumpedState::Enter(FygarComponent* Fygar) {
    std::cout << "Fygar hit by pump. Pump hits: " << Fygar->m_PumpHits << std::endl;

    m_ElapsedTime = 0.0f;

    // Play the corresponding animation based on the number of hits
    switch (Fygar->m_PumpHits) {
    case 1:
        Fygar->m_Owner->GetComponent<dae::AnimationComponent>()->Play("Pumped1", true);
        break;
    case 2:
        Fygar->m_Owner->GetComponent<dae::AnimationComponent>()->Play("Pumped2", true);
        break;
    case 3:
        Fygar->m_Owner->GetComponent<dae::AnimationComponent>()->Play("Pumped3", true);
        break;
    case 4:
        Fygar->m_Owner->GetComponent<dae::AnimationComponent>()->Play("Pumped4", true);
        Fygar->m_Owner->GetComponent<HitBox>()->Disable();
        m_Pumped4Duration = Fygar->m_Owner->GetComponent<dae::AnimationComponent>()->GetAnimationDuration();
        break;
    }
    Fygar->StartDeflationTimer();

    // Initialize the deflation timer
    m_DeflationTimer = Fygar->m_DeflationTimeLimit;
}


void FygarPumpedState::Update(FygarComponent* Fygar) {
    std::cout << "Fygar is being pumped" << std::endl;

    // If Pump hits is 4, track the elapsed time to switch to dead state
    if (Fygar->m_PumpHits == 4) {
        m_ElapsedTime += dae::GameTime::GetDeltaTime();
        if (m_ElapsedTime >= static_cast<float>(m_Pumped4Duration)) {
            Fygar->SetState(std::make_unique<FygarDeadState>());
            return;
        }
    }

    // Decrease the deflation timer
    m_DeflationTimer -= dae::GameTime::GetDeltaTime();

    if (m_DeflationTimer <= 0.0f) {
        // Deflation starts if the timer expires
        Fygar->m_PumpHits--;

        if (Fygar->m_PumpHits > 0) {
            // Play the corresponding deflation animation
            switch (Fygar->m_PumpHits) {
            case 1:
                Fygar->m_Owner->GetComponent<dae::AnimationComponent>()->Play("Pumped1", true);
                break;
            case 2:
                Fygar->m_Owner->GetComponent<dae::AnimationComponent>()->Play("Pumped2", true);
                break;
            case 3:
                Fygar->m_Owner->GetComponent<dae::AnimationComponent>()->Play("Pumped3", true);
                break;
            }

            // Reset the deflation timer
            m_DeflationTimer = Fygar->m_DeflationTimeLimit;
        }
        else {
            // If pump hits reach 0, transition back to wandering state
            Fygar->SetState(std::make_unique<FygarWandering>());
        }
    }
}


void FygarPumpedState::Exit(FygarComponent* Fygar) {
    std::cout << "Fygar exiting Pumped State" << std::endl;
    Fygar->DetectsPlayer();
}


void FygarDeadState::Enter(FygarComponent* Fygar) {
    std::cout << "Fygar entered Dead State" << std::endl;

    // Determine the layer based on the Y position
    int layer = Fygar->DetermineLayer(Fygar->m_Owner->GetWorldPosition().y);

    // Calculate the points based on the layer
    int points = Fygar->CalculatePoints(layer, "Pooka");

    // Get the last attacker and award points
    if (const auto lastAttacker = Fygar->GetLastAttacker()) {
	    if (const auto playerScoreComponent = lastAttacker->GetComponent<dae::PointComponent>()) {
            playerScoreComponent->SetScore(playerScoreComponent->GetScore() + points); // Award points to the correct player
        }
    }

    // Remove the Pooka object from the scene after the death animation plays
    dae::SceneData::GetInstance().RemoveGameObject(Fygar->m_Owner, dae::GameObjectType::enemy);
    GameData::GetInstance().CheckGameState();
    dae::SceneManager::GetInstance().GetActiveScene()->Remove(Fygar->m_Owner);
}

void FygarDeadState::Update(FygarComponent* /*Fygar*/) {
    // No update logic needed for dead state
}

void FygarDeadState::Exit(FygarComponent* /*Fygar*/) {
    std::cout << "Fygar exiting Dead State" << std::endl;
}

void FygarCrushedState::Enter(FygarComponent* Fygar) {
    std::cout << "Enemy entered Crushed State" << std::endl;
    // Play crushed animation
    Fygar->m_Owner->GetComponent<HitBox>()->Disable();
    Fygar->m_Owner->GetComponent<dae::AnimationComponent>()->Play("Crushed");
    m_CrushedDuration = Fygar->m_Owner->GetComponent<dae::AnimationComponent>()->GetAnimationDuration();
}

void FygarCrushedState::Update(FygarComponent* Fygar) {
    m_ElapsedTime += dae::GameTime::GetDeltaTime();
    if (m_ElapsedTime >= static_cast<float>(m_CrushedDuration)) {
        Fygar->SetState(std::make_unique<FygarDeadState>());
    }
}

void FygarCrushedState::Exit(FygarComponent* /*Fygar*/) {
    std::cout << "Enemy exiting Crushed State" << std::endl;
}