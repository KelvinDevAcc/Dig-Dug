#include "PookaState.h"
#include "PookaComponent.h"
#include <iostream>

#include "AnimationComponent.h"
#include "GameData.h"
#include "GameObject.h"
#include "GameTime.h"
#include "PointComponent.h"
#include "SceneData.h"
#include "SceneManager.h"

void PookaNormalState::Enter(PookaComponent* /*pooka*/) {
    std::cout << "Pooka entered Normal State" << std::endl;
}

void PookaNormalState::Update(PookaComponent* pooka) {
    pooka->MoveToNextWaypoint();
    std::cout << "Pooka is patrolling" << std::endl;

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

void PookaPumpedState::Enter(PookaComponent* pooka) {
    std::cout << "Pooka hit by pump. Pump hits: " << pooka->m_PumpHits << std::endl;

    m_ElapsedTime = 0.0f;

    // Play the corresponding animation based on the number of hits
    switch (pooka->m_PumpHits) {
    case 1:
        pooka->m_Owner->GetComponent<dae::AnimationComponent>()->Play("Pumped1", true);
        break;
    case 2:
        pooka->m_Owner->GetComponent<dae::AnimationComponent>()->Play("Pumped2", true);
        break;
    case 3:
        pooka->m_Owner->GetComponent<dae::AnimationComponent>()->Play("Pumped3", true);
        break;
    case 4:
        pooka->m_Owner->GetComponent<dae::AnimationComponent>()->Play("Pumped4", true);
        pooka->m_Owner->GetComponent<HitBox>()->Disable();
        m_Pumped4Duration = pooka->m_Owner->GetComponent<dae::AnimationComponent>()->GetAnimationDuration();
        break;
    }
    pooka->StartDeflationTimer();

    // Initialize the deflation timer
    m_DeflationTimer = pooka->m_DeflationTimeLimit;
}


void PookaPumpedState::Update(PookaComponent* pooka) {
    std::cout << "Pooka is being pumped" << std::endl;

    // If Pump hits is 4, track the elapsed time to switch to dead state
    if (pooka->m_PumpHits == 4) {
        m_ElapsedTime += dae::GameTime::GetDeltaTime();
        if (m_ElapsedTime >= static_cast<float>(m_Pumped4Duration)) {
            pooka->SetState(std::make_unique<PookaDeadState>());
            return;
        }
    }

    // Decrease the deflation timer
    m_DeflationTimer -= dae::GameTime::GetDeltaTime();

    if (m_DeflationTimer <= 0.0f) {
        // Deflation starts if the timer expires
        pooka->m_PumpHits--;

        if (pooka->m_PumpHits > 0) {
            // Play the corresponding deflation animation
            switch (pooka->m_PumpHits) {
            case 1:
                pooka->m_Owner->GetComponent<dae::AnimationComponent>()->Play("Pumped1", true);
                break;
            case 2:
                pooka->m_Owner->GetComponent<dae::AnimationComponent>()->Play("Pumped2", true);
                break;
            case 3:
                pooka->m_Owner->GetComponent<dae::AnimationComponent>()->Play("Pumped3", true);
                break;
            }

            // Reset the deflation timer
            m_DeflationTimer = pooka->m_DeflationTimeLimit;
        }
        else {
            // If pump hits reach 0, transition back to wandering state
            pooka->SetState(std::make_unique<PookaWandering>());
        }
    }
}


void PookaPumpedState::Exit(PookaComponent* pooka) {
    std::cout << "Pooka exiting Pumped State" << std::endl;
    pooka->DetectsPlayer();
    
}


void PookaDeadState::Enter(PookaComponent* pooka) {
    std::cout << "Pooka entered Dead State" << std::endl;

    // Determine the layer based on the Y position
    int layer = pooka->DetermineLayer(pooka->m_Owner->GetWorldPosition().y);

    // Calculate the points based on the layer
    int points = pooka->CalculatePoints(layer, "Pooka");

    // Get the last attacker and award points
    if (const auto lastAttacker = pooka->GetLastAttacker()) {
	    if (const auto playerScoreComponent = lastAttacker->GetComponent<dae::PointComponent>()) {
            playerScoreComponent->SetScore(playerScoreComponent->GetScore() + points); // Award points to the correct player
        }
    }

    // Remove the Pooka object from the scene after the death animation plays
    dae::SceneData::GetInstance().RemoveGameObject(pooka->m_Owner, dae::GameObjectType::enemy);
    GameData::GetInstance().CheckGameState();
    dae::SceneManager::GetInstance().GetActiveScene()->Remove(pooka->m_Owner);
    dae::Message message;
    message.type = dae::PlaySoundMessageType::Sound;
    message.arguments.emplace_back(static_cast<sound_id>(4));
    dae::EventQueue::Broadcast(message);
}

void PookaDeadState::Update(PookaComponent* /*pooka*/) {
    // No update logic needed for dead state
}

void PookaDeadState::Exit(PookaComponent* /*pooka*/) {
    std::cout << "Pooka exiting Dead State" << std::endl;
}

void PookaCrushedState::Enter(PookaComponent* pooka) {
    std::cout << "Enemy entered Crushed State" << std::endl;
    // Play crushed animation
    pooka->m_Owner->GetComponent<HitBox>()->Disable();
    pooka->m_Owner->GetComponent<dae::AnimationComponent>()->Play("Crushed");
    m_CrushedDuration = pooka->m_Owner->GetComponent<dae::AnimationComponent>()->GetAnimationDuration();
}

void PookaCrushedState::Update(PookaComponent* pooka) {
    m_ElapsedTime += dae::GameTime::GetDeltaTime();
    if (m_ElapsedTime >= static_cast<float>(m_CrushedDuration)) {
        pooka->SetState(std::make_unique<PookaDeadState>());
    }
}

void PookaCrushedState::Exit(PookaComponent* /*pooka*/) {
    std::cout << "Enemy exiting Crushed State" << std::endl;
}