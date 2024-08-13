#include <random>

#include "EnemyComponent.h"

#include "AnimationComponent.h"
#include "FygarComponent.h"
#include "GameData.h"
#include "GameObject.h"
#include "GameTime.h"
#include "Player.h"
#include "PointComponent.h"
#include "SceneData.h"
#include "SceneManager.h"



void EnemyGhostState::Enter(EnemyComponent* enemy) {
    enemy->EnableGhostMode();
    enemy->m_Owner->GetComponent<dae::AnimationComponent>()->Play("Ghost", true);
    enemy->DetectsPlayer();

    if (!enemy->m_DetectedPlayers.empty()) {
        // Find the closest player and start pursuing
        enemy->m_TargetPlayerPosition = enemy->m_DetectedPlayers.front()->GetWorldPosition();
        enemy->m_GhostModePursuitTimer = enemy->m_GhostModePursuitDuration;
    }
}

void EnemyGhostState::Update(EnemyComponent* enemy) {
    // Decrement the pursuit timer
    enemy->m_GhostModePursuitTimer -= dae::GameTime::GetDeltaTime();

    // Check if the ghost is at a walkthrough location
    if (enemy->IsAtWalkthroughLocation()) {
        // Move the ghost to the center of the walkthrough location
        glm::vec3 walkthroughCenter = enemy->GetWalkthroughCenter();
        if (enemy->ReachedDestination()) {
            enemy->m_Destination = walkthroughCenter;
            enemy->MoveToNextWaypointTowards(walkthroughCenter);
        }

        // Check if the ghost reached the center of the walkthrough location
        if (enemy->ReachedDestination()) {
            // Transition to wandering state
            enemy->SetState(std::make_unique<EnemyWandering>());
            return;
        }
    }

    // If the pursuit timer expired or target position is invalid
    if (enemy->m_GhostModePursuitTimer <= 0.0f || enemy->m_TargetPlayerPosition == glm::vec3(0, 0, 0)) {
        // Move to the nearest valid spot
        const glm::vec3 nearestValidSpot = enemy->GetWalkthroughCenter();
        if (nearestValidSpot != enemy->m_CurrentPosition) {
            enemy->m_Destination = nearestValidSpot;
            enemy->MoveToNextWaypointTowards(nearestValidSpot);
        }

        // Check if the ghost reached the nearest valid spot
        if (enemy->ReachedDestination()) {
            // Transition to wandering state
            enemy->SetState(std::make_unique<EnemyWandering>());
        }
        return; // Skip further processing in this update cycle
    }

    // Continuously detect the player
    enemy->DetectsPlayer();

    if (!enemy->m_DetectedPlayers.empty()) {
        // Update the target player position to the closest detected player
        enemy->m_TargetPlayerPosition = enemy->m_DetectedPlayers.front()->GetWorldPosition();
    }

    if (enemy->m_TargetPlayerPosition != glm::vec3(0, 0, 0)) {
        glm::vec3 direction = enemy->m_TargetPlayerPosition - enemy->m_CurrentPosition;
        const float distance = glm::length(direction);

        if (distance > 1.0f) {
            // Continue pursuing the player
            enemy->MoveToNextWaypointTowards(enemy->m_TargetPlayerPosition);
        }
        else {
            // Player reached, transition to wandering state
            enemy->SetState(std::make_unique<EnemyWandering>());
        }
    }
}

void EnemyGhostState::Exit(EnemyComponent* enemy) {
    enemy->DisableGhostMode();
}


void EnemyWandering::Enter(EnemyComponent* enemy) {
    if (auto fygarComponent = enemy->m_Owner->GetComponent<FygarComponent>()) {
        fygarComponent->m_breathFire->Deactivate();
    }
	enemy->ChooseRandomDirection();
    // Array of direction to animation mapping
   
}

void EnemyWandering::Update(EnemyComponent* enemy) {
    if (enemy->ReachedDestination())
    {
        enemy->ChooseRandomDirection();
    }
    else
    {
        enemy->MoveToNextWaypoint();

          const std::unordered_map<Direction, std::string> directionToAnimation = {
         { Direction::Up, "Walk_Up" },
         { Direction::Down, "Walk_Down" },
         { Direction::Left, "Walk_Left" },
         { Direction::Right, "Walk_Right" }
          };

          // Get the animation component
          auto animationComponent = enemy->m_Owner->GetComponent<dae::AnimationComponent>();

          // Get the animation name based on the current direction
          auto it = directionToAnimation.find(enemy->m_currentdirection);
          if (it != directionToAnimation.end()) {
              animationComponent->Play(it->second, true);
          }

          // Handle sprite flipping based on direction
          switch (enemy->m_currentdirection) {
          case Direction::Left:
              animationComponent->FlipSprite(true, false);
              break;
          case Direction::Right:
              animationComponent->FlipSprite(false, false);
              break;
          default:
              // For Up and Down, no flip is needed
              break;
          }

          if (enemy->ShouldEnterGhostMode()) {
              enemy->SetState(std::make_unique<EnemyGhostState>());
          }
          if (auto fygarComponent = enemy->m_Owner->GetComponent<FygarComponent>()) {
              fygarComponent->UpdateFireBreath(); // Check if it's time to breathe fire
              if (fygarComponent->ShouldBreatheFire()) {
                  enemy->SetState(std::make_unique<EnemyBreathingFireState>());
              }
          }
    }
}

void EnemyWandering::Exit(EnemyComponent* /*enemy*/) {
}

void EnemyPumpedState::Enter(EnemyComponent* Enemy) {

    m_ElapsedTime = 0.0f;

    // Play the corresponding animation based on the number of hits
    switch (Enemy->m_PumpHits) {
    case 2:
        Enemy->m_Owner->GetComponent<dae::AnimationComponent>()->Play("Pumped1", true);
        break;
    case 3:
        Enemy->m_Owner->GetComponent<dae::AnimationComponent>()->Play("Pumped2", true);
        break;
    case 4:
        Enemy->m_Owner->GetComponent<dae::AnimationComponent>()->Play("Pumped3", true);
        break;
    case 5:
        Enemy->m_Owner->GetComponent<dae::AnimationComponent>()->Play("Pumped4", true);
        Enemy->m_Owner->GetComponent<HitBox>()->Disable();
        m_Pumped4Duration = Enemy->m_Owner->GetComponent<dae::AnimationComponent>()->GetAnimationDuration();
        break;
default: break;
    }
    Enemy->StartDeflationTimer();

    // Initialize the deflation timer
    m_DeflationTimer = Enemy->m_DeflationTimeLimit;
}


void EnemyPumpedState::Update(EnemyComponent* enemy) {

    // If Pump hits is 4, track the elapsed time to switch to dead state
    if (enemy->m_PumpHits == 5) {
        m_ElapsedTime += dae::GameTime::GetDeltaTime();
        if (m_ElapsedTime >= static_cast<float>(m_Pumped4Duration)) {
            enemy->SetState(std::make_unique<EnemyDeadState>());
            return;
        }
    }

    // Decrease the deflation timer
    m_DeflationTimer -= dae::GameTime::GetDeltaTime();

    if (m_DeflationTimer <= 0.0f) {
        // Deflation starts if the timer expires
        enemy->m_PumpHits--;

        if (enemy->m_PumpHits > 0) {
            // Play the corresponding deflation animation
            switch (enemy->m_PumpHits) {
            case 1:
                enemy->m_Owner->GetComponent<dae::AnimationComponent>()->Play("Pumped1", true);
                break;
            case 2:
                enemy->m_Owner->GetComponent<dae::AnimationComponent>()->Play("Pumped2", true);
                break;
            case 3:
                enemy->m_Owner->GetComponent<dae::AnimationComponent>()->Play("Pumped3", true);
                break;
            }

            // Reset the deflation timer
            m_DeflationTimer = enemy->m_DeflationTimeLimit;
        }
        else {
            // If pump hits reach 0, transition back to wandering state
            enemy->SetState(std::make_unique<EnemyWandering>());
        }
    }
}


void EnemyPumpedState::Exit(EnemyComponent* enemy) {
    enemy->DetectsPlayer();
    
}


void EnemyDeadState::Enter(EnemyComponent* enemy) {

    // Determine the layer based on the Y position
    int layer = enemy->DetermineLayer(enemy->m_Owner->GetWorldPosition().y);

    // Calculate points
    int points = enemy->CalculatePoints(layer);

    // Check if the enemy is a Fygar and if it was killed from the side
    if (auto fygarComponent = enemy->m_Owner->GetComponent<FygarComponent>()) {
        if (fygarComponent->WasKilledFromSide()) {
            points *= 2; // Double points for Fygar killed from the side
        }
    }

    // Get the last attacker and award points
    if (const auto lastAttacker = enemy->GetLastAttacker()) {
        if (const auto playerScoreComponent = lastAttacker->GetComponent<dae::PointComponent>()) {
            // Award points to the player
            playerScoreComponent->SetScore(playerScoreComponent->GetScore() + points);

            if (const auto playerComponent = lastAttacker->GetComponent<game::Player>()) {
                int playerID = playerComponent->GetPlayerID();  // Assumes GetPlayerID() exists

                // Get the player data from GameData
                PlayerData playerData = GameData::GetInstance().GetPlayerData(playerID);

                // Add points to the player's score
                playerData.score += points;

                // Update the player data in GameData
                GameData::GetInstance().UpdatePlayerData(playerID, playerData);
            }
        }
    }

    // Play death sound effect
    dae::Message message;
    message.type = dae::PlaySoundMessageType::Sound;
    message.arguments.emplace_back(static_cast<sound_id>(4));
    dae::EventQueue::Broadcast(message);

    // Remove the enemy object from the scene
    auto& sceneData = dae::SceneData::GetInstance();
    sceneData.RemoveGameObject(enemy->m_Owner, dae::GameObjectType::enemy);
    GameData::GetInstance().CheckGameState();
    dae::SceneManager::GetInstance().GetActiveScene()->Remove(enemy->m_Owner);
}


void EnemyDeadState::Update(EnemyComponent* /*enemy*/) {
    // No update logic needed for dead state
}

void EnemyDeadState::Exit(EnemyComponent* /*enemy*/) {
}

void EnemyCrushedState::Enter(EnemyComponent* enemy) {
    // Play crushed animation
    enemy->m_Owner->GetComponent<HitBox>()->Disable();
    enemy->m_Owner->GetComponent<dae::AnimationComponent>()->Play("Crushed");
    m_CrushedDuration = enemy->m_Owner->GetComponent<dae::AnimationComponent>()->GetAnimationDuration();
}

void EnemyCrushedState::Update(EnemyComponent* enemy) {
    m_ElapsedTime += dae::GameTime::GetDeltaTime();
    if (m_ElapsedTime >= static_cast<float>(m_CrushedDuration)) {
        enemy->SetState(std::make_unique<EnemyDeadState>());
    }
}

void EnemyCrushedState::Exit(EnemyComponent* /*enemy*/) {
}


void EnemyBreathingFireState::Enter(EnemyComponent* enemy) {
    // Play the breathing fire animation
    if (const auto animationComponent = enemy->m_Owner->GetComponent<dae::AnimationComponent>()) {
        switch (enemy->m_currentdirection) {
        case Direction::Right:
            animationComponent->Play("Attacking");  // Play the animation, not looping
            m_FireAnimationDuration = static_cast<float>(animationComponent->GetAnimationDuration()); // Get the animation duration
            animationComponent->FlipSprite(true, false);
            break;
        case Direction::Left:
            animationComponent->Play("Attacking");  // Play the animation, not looping
            m_FireAnimationDuration = static_cast<float>(animationComponent->GetAnimationDuration()); // Get the animation duration
            animationComponent->FlipSprite(false, false);
            break;
        }
    }

    m_ElapsedTime = 0.0f; // Reset the elapsed time
    m_FireBreathTriggered = false; // Flag to determine if fire breath has been triggered
}

void EnemyBreathingFireState::Update(EnemyComponent* enemy) {
    const float deltaTime = dae::GameTime::GetDeltaTime();
    m_ElapsedTime += deltaTime;

    if (m_ElapsedTime >= m_FireAnimationDuration) {
        if (!m_FireBreathTriggered) {
            // Trigger the fire breath if it hasn't been triggered yet
            glm::vec3 direction;
            switch (enemy->m_currentdirection) {
            case Direction::Right:
                direction = glm::vec3(1.0f, 0.0f, 0.0f); // Moving right
                enemy->m_Owner->GetComponent<dae::AnimationComponent>()->FlipSprite(true, false);
                break;
            case Direction::Left:
                direction = glm::vec3(-1.0f, 0.0f, 0.0f); // Moving left
                enemy->m_Owner->GetComponent<dae::AnimationComponent>()->FlipSprite(false, false);
                break;
            default:
                direction = glm::vec3(-1.0f, 0.0f, 0.0f); // Default direction
                break;
            }

            if (auto fygarComponent = dynamic_cast<FygarComponent*>(enemy)) {
                if (fygarComponent->m_breathFire) {
                    fygarComponent->m_breathFire->Activate(direction);
                }
            }
            m_FireBreathTriggered = true;
        }
        if (auto fygarComponent = dynamic_cast<FygarComponent*>(enemy)) {
            if (fygarComponent->m_breathFire->GetLifetime() <= 0.0f) {
                enemy->SetState(std::make_unique<EnemyWandering>());
            }
        }
    }
}

void EnemyBreathingFireState::Exit(EnemyComponent* enemy) {
    if (auto fygarComponent = dynamic_cast<FygarComponent*>(enemy)) {
            fygarComponent->ResetFireBreathCooldown();
    }
}