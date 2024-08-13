#include "EnemyPlayer.h"

#include <iostream>

#include "BreathFireComponent.h"
#include "GameData.h"
#include "GameObject.h"
#include "GameTime.h"
#include "SceneData.h"
#include "SceneHelpers.h"
#include "SceneManager.h"


namespace game
{
    EnemyPlayer::EnemyPlayer(dae::GameObject* gameObject)
	    : m_GameObject(gameObject), m_breathingFire(false), m_warmupAnimationtime(0), m_cooldownAnimationtime(0),
	      m_PumpHits(0), m_isBeingPumpt(false), m_DeflationTimer(0), m_DeflationTimeLimit(2.f),
	      m_markedForDeletion(false),
	      m_deletionDelay(0)
    {
	    m_startPosition = m_GameObject->GetWorldPosition();
	    m_animationComponent = m_GameObject->GetComponent<dae::AnimationComponent>();
	    auto breathFire = std::make_unique<BreathFire>(m_GameObject, glm::vec3(1, 0, 0), 100.0f, 3.0f);
	    m_breathFire = breathFire.get();
	    m_GameObject->AddComponent(std::move(breathFire));
    }

    void EnemyPlayer::Update()
    {
        if (m_markedForDeletion)
        {
            m_deletionDelay -= dae::GameTime::GetDeltaTime();
            if (m_deletionDelay <= 0.0f)
            {
                dae::SceneData::GetInstance().RemoveGameObject(m_GameObject, dae::GameObjectType::enemy);
                dae::SceneData::GetInstance().RemoveGameObject(m_GameObject, dae::GameObjectType::enemyPlayers);
                GameData::GetInstance().CheckGameState();
                dae::SceneManager::GetInstance().GetActiveScene()->Remove(m_GameObject);
                return; // Exit update method after removal
            }
        }

        if (m_breathingFire)
        {
            m_warmupAnimationtime -= dae::GameTime::GetDeltaTime();

            if (m_warmupAnimationtime <= 0)
            {
                glm::vec3 direction;

                // Set the direction based on the current movement direction
                switch (m_currnentDirection)
                {
                case Direction::Right:
                    direction = glm::vec3(1.0f, 0.0f, 0.0f); // Moving right
                    break;
                case Direction::Left:
                    direction = glm::vec3(-1.0f, 0.0f, 0.0f); // Moving left
                    break;
                default:
                    direction = glm::vec3(1.0f, 0.0f, 0.0f); // Default or handle unexpected case
                    break;
                }

                if (m_breathFire)
                {
                    m_breathFire->Activate(direction);
                }

                m_cooldownAnimationtime = 3.0f; // Reset cooldown time
                m_breathingFire = false; // Disable breathing fire after activation
            }
        }
        else
        {
            // Handle cooldown timer
            if (m_cooldownAnimationtime > 0)
            {
                m_cooldownAnimationtime -= dae::GameTime::GetDeltaTime();
            }
        }

        if (m_isBeingPumpt)
        {
            deflating();
        }
    }

    void EnemyPlayer::Render() const
    {
		
    }

    void EnemyPlayer::Move(float deltaX, float deltaY)
    {
	    if (m_breathingFire || m_isBeingPumpt)
            return;
        m_breathFire->Deactivate();
        // Update the current direction based on movement
        if (deltaX > 0)
            m_currnentDirection = Direction::Right;
        else if (deltaX < 0)
            m_currnentDirection = Direction::Left;
        else if (deltaY > 0)
            m_currnentDirection = Direction::Down;
        else if (deltaY < 0)
            m_currnentDirection = Direction::Up;

        // Update the animation state unless in Ghost state
        if (m_currentanimationState != Enemyanimationstate::Ghost)
        {
            if (m_currnentDirection == Direction::Right)
                UpdateAnimationState(Enemyanimationstate::Walk_Right);
            else if (m_currnentDirection == Direction::Left)
                UpdateAnimationState(Enemyanimationstate::Walk_Left);
            else if (m_currnentDirection == Direction::Up)
                UpdateAnimationState(Enemyanimationstate::Walk_Up);
            else if (m_currnentDirection == Direction::Down)
                UpdateAnimationState(Enemyanimationstate::Walk_Down);
        }

        // Handle movement and check for tile center
        if (m_currentanimationState == Enemyanimationstate::Ghost)
        {
            // Move the character
            MoveHorizontally(deltaX);
            MoveVertically(deltaY);

            // Check if the enemy is at the center of a walkthrough tile
            if (dae::SceneData::GetInstance().IsOnwalkthrough(*m_GameObject) &&IsAtTileCenter(*m_GameObject))
            {
                SnapToTileCenter(); // Snap the enemy to the center of the tile
                UpdateAnimationState(Enemyanimationstate::Idle); // Transition to Idle state
            }
        }
        else
        {
            // Move the character horizontally if allowed
            if (deltaX != 0.f && dae::SceneData::GetInstance().CanEnemyMove(deltaX, deltaY, *m_GameObject))
            {
                MoveHorizontally(deltaX);
            }

            // Move the character vertically if allowed
            if (deltaY != 0.f && dae::SceneData::GetInstance().CanEnemyMove(deltaX, deltaY, *m_GameObject))
            {
                MoveVertically(deltaY);
            }
        }

        
    }
    

    void EnemyPlayer::MoveHorizontally(float deltaX) const
    {
        if (deltaX == 0.0f) return;

        glm::vec3 position = m_GameObject->GetWorldPosition();
        float newPosX = position.x + deltaX * dae::GameTime::GetDeltaTime() * m_Speed;
        m_GameObject->SetLocalPosition(glm::vec3(newPosX, position.y, position.z));
    }

    void EnemyPlayer::MoveVertically(float deltaY) const
    {
        if (deltaY == 0.0f) return;

        glm::vec3 position = m_GameObject->GetWorldPosition();
        float newPosY = position.y + deltaY * dae::GameTime::GetDeltaTime() * m_Speed;
        m_GameObject->SetLocalPosition(glm::vec3(position.x, newPosY, position.z));
    }


    void EnemyPlayer::Attack()
    {
        if (m_isBeingPumpt)
            return;

        UpdateAnimationState(Enemyanimationstate::Attacking);
        if (m_breathFire)
        {
            m_warmupAnimationtime = static_cast<float>(m_animationComponent->GetAnimationDuration()/2);
            m_breathingFire = true;
           
        }
    }

    void EnemyPlayer::Ghost()
    {
        if (m_isBeingPumpt)
            return;
        UpdateAnimationState(Enemyanimationstate::Ghost);

    }


    bool EnemyPlayer::IsAtTileCenter(const dae::GameObject& gameObject) const
    {
        glm::vec3 currentPosition = gameObject.GetWorldPosition();
        glm::vec3 tileCenter = SceneHelpers::GetCenterOfTile(currentPosition);

        // Check if the enemy's current position is close enough to the tile center
        const float epsilon = 3.0f; // Small threshold to account for floating-point precision
        return glm::length(currentPosition - tileCenter) < epsilon;
    }

    void EnemyPlayer::SnapToTileCenter()
    {
	    const glm::vec3 position = m_GameObject->GetWorldPosition();
        const glm::vec2 tileCenter = SceneHelpers::GetCenterOfTile(position); // Assuming GetTileCenter returns the center of the tile
        m_GameObject->SetLocalPosition(glm::vec3(tileCenter.x, tileCenter.y, position.z));
    }

    void EnemyPlayer::ReSpawn()
    {
        m_GameObject->SetLocalPosition(m_startPosition);
        SnapToTileCenter();
        m_breathFire->Deactivate();
        m_PumpHits = 0;
        UpdateAnimationState(Enemyanimationstate::Idle);
    }

    void EnemyPlayer::HitByPump() {
        ++m_PumpHits;
        m_DeflationTimer = m_DeflationTimeLimit;
        m_isBeingPumpt = true; 
        HandlePumpHits();
    }

    void EnemyPlayer::HandlePumpHits()
    {
        switch (m_PumpHits)
        {
        case 1:
            m_animationComponent->Play("Pumped1", true);
            m_isBeingPumpt = true;
            break;
        case 2:
            m_animationComponent->Play("Pumped2", true);
            break;
        case 3:
            m_animationComponent->Play("Pumped3", true);
            break;
        case 4:
            m_animationComponent->Play("Pumped4", true);
            m_deletionDelay =static_cast<float>(m_animationComponent->GetAnimationDuration());
            m_markedForDeletion = true;
            break;
        default:
            break;
        }
    }

    void EnemyPlayer::deflating()
    {

        // Decrease the deflation timer
        m_DeflationTimer -= dae::GameTime::GetDeltaTime();

        if (m_DeflationTimer <= 0.0f) {
            // Deflation starts if the timer expires
            m_PumpHits--;

            if (m_PumpHits >= 0) {
                // Play the corresponding deflation animation
                switch (m_PumpHits) {
                case 0:
                    m_animationComponent->Play("Normal");
                    m_isBeingPumpt = false;

                    break;
                case 1:
                    m_animationComponent->Play("Pumped1", true);
                    break;
                case 2:
                    m_animationComponent->Play("Pumped2", true);
                    break;
                case 3:
                    m_animationComponent->Play("Pumped3", true);
                    break;
                }

                // Reset the deflation timer
                m_DeflationTimer = m_DeflationTimeLimit;
            }
        }
    }

    void EnemyPlayer::UpdateAnimationState(Enemyanimationstate state)
    {
        if (m_currentanimationState == state)
            return;

        m_currentanimationState = state;

        switch (state)
        {
        case Enemyanimationstate::Idle:
            if (m_animationComponent)
            {
                m_animationComponent->Play("Normal");
                m_animationComponent->FlipSprite(false, false); 

            }
            break;
        case Enemyanimationstate::Walk_Up:
            if (m_animationComponent)
            {
                m_animationComponent->FlipSprite(false, false);
                m_animationComponent->Play("Walk_Up", true);
            }
            break;
        case Enemyanimationstate::Walk_Right:
            if (m_animationComponent)
            {
                m_animationComponent->FlipSprite(false, false);
                m_animationComponent->Play("Walk_Right", true);
            }
            break;
        case Enemyanimationstate::Walk_Left:
            if (m_animationComponent)
            {
                m_animationComponent->FlipSprite(true, false);
                m_animationComponent->Play("Walk_Left", true);
            }
            break;
        case Enemyanimationstate::Walk_Down:
            if (m_animationComponent)
            {
                m_animationComponent->FlipSprite(false, false);
                m_animationComponent->Play("Walk_Down", true);
            }
            break;
        case Enemyanimationstate::Attacking:
	        if (m_currnentDirection == Direction::Left)
	        {
                m_animationComponent->Play("Attacking");
                m_animationComponent->FlipSprite(false, false);
	        }
            else
                m_animationComponent->FlipSprite(true, false);
                m_animationComponent->Play("Attacking");
            break;
        case Enemyanimationstate::Dying:
            if (m_animationComponent)
                m_animationComponent->Play("Dying");
            break;
        case Enemyanimationstate::Ghost:
            if (m_animationComponent)
                m_animationComponent->Play("Ghost", true);
            break;
        default:;
        }
    }
   
}
