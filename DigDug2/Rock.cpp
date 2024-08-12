#include "Rock.h"
#include "GameTime.h"
#include "Player.h"
#include "EnemyComponent.h"
#include "SceneData.h"
#include "SceneHelpers.h"
#include "SceneManager.h"

Rock::Rock(dae::GameObject* owner):
	m_IsFalling(false)
	, m_FallSpeed(0.0f)
	, m_Gravity(9.8f)
	, m_Owner(owner), m_timer(0), m_PlayerMovedAway(false), m_IsAnimating(false), m_animationTimer(0), m_dieing(false)
{
	m_animationComponnent = m_Owner->GetComponent<dae::AnimationComponent>();
}

void Rock::Update()
{

    // Handle the timer for destruction after the animation
    if (m_timer > 0)
    {
        m_timer -= dae::GameTime::GetDeltaTime();
        if (m_timer <= 0)
        {
            dae::SceneData::GetInstance().RemoveGameObject(m_Owner, dae::GameObjectType::Rock);
            dae::SceneManager::GetInstance().GetActiveScene()->Remove(m_Owner);
        }
    }

	if (m_dieing)
        return;

    if (m_IsAnimating)
    {
        m_animationTimer -= dae::GameTime::GetDeltaTime(); 

        if (m_animationTimer <= 0)
        {
            // Animation has finished, switch to falling mode
            m_IsAnimating = false;
            m_IsFalling = true;
            m_FallSpeed = 100.0f;
        }
    }
    else if (!m_IsFalling)
    {
        // Check if the rock should start falling
        if (ShouldStartFalling())
        {
            if (NoplayerUntherneath())
            {
                // Player has moved away, start jittering animation
                m_PlayerMovedAway = true;
                m_animationComponnent->Play("Falling", true);
                dae::Message message;
                message.type = dae::PlaySoundMessageType::Sound;
                message.arguments.emplace_back(static_cast<sound_id>(11));
                dae::EventQueue::Broadcast(message);
                // Get the duration of the "Falling" animation
                const auto animationDuration = static_cast<float>(m_animationComponnent->GetAnimationDuration()* 2);
                m_animationTimer = animationDuration;
                m_IsAnimating = true;
            }
        }
    }
    else
    {
        m_animationComponnent->Play("Idle", true);
        m_FallSpeed += m_Gravity * dae::GameTime::GetDeltaTime();
        auto newPosition = m_Owner->GetWorldPosition();
        newPosition.y += m_FallSpeed * dae::GameTime::GetDeltaTime();
        m_Owner->SetLocalPosition(newPosition);
        CheckCollisionWithObjects();
        if (!CheckCollisionWithWalkThrough())
        {
            Die();
        }
    }
}


void Rock::Die()
{
    m_dieing = true;
    m_IsFalling = false;
    dae::Message message;
    message.type = dae::PlaySoundMessageType::Sound;
    message.arguments.emplace_back(static_cast<sound_id>(9));
    dae::EventQueue::Broadcast(message);
    m_animationComponnent->Play("Dying");
    // Get the duration of the "Dying" animation
    const auto animationDuration = static_cast<float>(m_animationComponnent->GetAnimationDuration());
    m_timer = animationDuration;
    m_Owner->GetComponent<HitBox>()->Disable();
}

bool Rock::ShouldStartFalling()
{
    // Get the current position of the rock
    const glm::vec3 currentPosition = m_Owner->GetWorldPosition();

    // Move down slightly to check if there's a floor directly below
    glm::vec3 checkPosition = currentPosition;
    checkPosition.y += SceneHelpers::GetCellSize().y;

    const auto& sceneData = dae::SceneData::GetInstance();
    const auto& floors = sceneData.GetWalkThrough();

    for (const auto& floor : floors)
    {
        if (floor == m_Owner) continue; // Skip self-collision

        if (const auto floorHitBox = floor->GetComponent<HitBox>())
        {
	        const SDL_Rect floorRect = floorHitBox->GetRect();
            // Check if the check position is within the floor bounds
            if (checkPosition.x >= floorRect.x && checkPosition.x < floorRect.x + floorRect.w &&
                checkPosition.y >= floorRect.y && checkPosition.y < floorRect.y + floorRect.h)
            {
                return true; // There's a floor underneath, do not start falling
            }
        }
    }
    return false;
}


bool Rock::NoplayerUntherneath()
{
    // Get the current position of the rock
    const auto hitBox = m_Owner->GetComponent<HitBox>();
    if (!hitBox) return false;

    const SDL_Rect rockRect = hitBox->GetRect();
    const int rockLeft = rockRect.x;
    const int rockRight = rockRect.x + rockRect.w;

    const auto& sceneData = dae::SceneData::GetInstance();
    const auto& players = sceneData.GetPlayers();

    for (const auto& player : players)
    {
        if (!player) continue;

        const auto playerHitBox = player->GetComponent<HitBox>();
        if (!playerHitBox) continue;

        const SDL_Rect playerRect = playerHitBox->GetRect();
        const int playerLeft = playerRect.x - 1;
        const int playerRight = playerRect.x + playerRect.w + 1;

        if ((playerLeft < rockRight && playerRight > rockLeft))
        {
            // Player is still underneath
            return false;
        }
        else
        {
            // The player has moved away, mark them as the one who triggered the fall
            m_PlayerThatTriggeredFall = player;
        }
    }

    // No players underneath, the rock should start falling
    return true;
}



bool Rock::CheckCollisionWithWalkThrough()
{
    const glm::vec3 currentPosition = m_Owner->GetWorldPosition();
    const float checkDistance = SceneHelpers::GetCellSize().y;
    glm::vec3 checkPosition = currentPosition;
    checkPosition.y += checkDistance; // Move downwards by the size of one cell

    const auto& sceneData = dae::SceneData::GetInstance();
    const auto& walkThroughs = sceneData.GetWalkThrough();

    for (const auto& walkThrough : walkThroughs)
    {
        if (!walkThrough) continue;

        const auto hitBox = walkThrough->GetComponent<HitBox>();
        if (!hitBox) continue;

        const SDL_Rect walkThroughRect = hitBox->GetRect();
        const glm::vec3 walkThroughPosition = walkThrough->GetWorldPosition();

        // Check if the position is within the bounds of the walkThrough rectangle
        if (checkPosition.x >= walkThroughPosition.x &&
            checkPosition.x < walkThroughPosition.x + walkThroughRect.w &&
            checkPosition.y >= walkThroughPosition.y &&
            checkPosition.y < walkThroughPosition.y + walkThroughRect.h)
        {
            return true; // There's a `WalkThrough` directly beneath the rock
        }
    }

    return false; // No `WalkThrough` found beneath the rock
}

bool Rock::CheckCollisionWithObjects()
{
    const auto& sceneData = dae::SceneData::GetInstance();
    const auto& enemies = sceneData.GetEnemies();

    int enemiesKilled = 0;  // Track the number of enemies killed

    for (const auto& enemy : enemies)
    {
        const auto hitBox = m_Owner->GetComponent<HitBox>();
        const auto enemyHitBox = enemy->GetComponent<HitBox>();
        if (hitBox && enemyHitBox && hitBox->IsColliding(*enemyHitBox))
        {

            dae::Message message;
            message.type = dae::PlaySoundMessageType::Sound;
            message.arguments.emplace_back(static_cast<sound_id>(10));
            dae::EventQueue::Broadcast(message);
            // Notify the enemy about being crushed
            enemy->GetComponent<EnemyComponent>()->OnCrushed();  // Call OnCrushed on the enemy
            ++enemiesKilled;  // Increment the kill count
        }
    }

    if (enemiesKilled > 0 && m_PlayerThatTriggeredFall)
    {
        // Calculate the score based on the number of enemies killed
        int points = 0;
        switch (enemiesKilled)
        {
        case 1: points = 1000; break;
        case 2: points = 2500; break;
        case 3: points = 4000; break;
        case 4: points = 6000; break;
        case 5: points = 8000; break;
        case 6: points = 10000; break;
        case 7: points = 12000; break;
        case 8: points = 15000; break;
        default: points = 15000; break;  // Cap at 15000 for more than 8 enemies
        }

        // Award the points to the player who triggered the fall
        if (const auto playerScoreComponent = m_PlayerThatTriggeredFall->GetComponent<dae::PointComponent>())
        {
            playerScoreComponent->SetScore(playerScoreComponent->GetScore() + points);
        }
    }



    return enemiesKilled > 0;  // Return true if any enemies were killed
}