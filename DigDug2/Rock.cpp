#include "Rock.h"
#include "GameTime.h"
#include "SceneData.h"
#include "SceneManager.h"

Rock::Rock(dae::GameObject* owner):
	m_IsFalling(false)
	, m_FallSpeed(0.0f)
	, m_Gravity(9.8f)
	, m_Owner(owner), m_timer(0), m_PlayerMovedAway(false), m_IsAnimating(false), m_animationTimer(0)
{
	m_animationComponnent = m_Owner->GetComponent<dae::AnimationComponent>();
	dae::SceneData::GetInstance().AddGameObject(m_Owner, dae::GameObjectType::Rock);
}

void Rock::Update()
{
    if (m_IsAnimating)
    {
        m_animationTimer -= dae::GameTime::GetDeltaTime(); 

        if (m_animationTimer <= 0)
        {
            // Animation has finished, switch to falling mode
            m_IsAnimating = false;
            m_IsFalling = true;
            m_FallSpeed = 0.0f;
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

                // Get the duration of the "Falling" animation
                const auto animationDuration = static_cast<float>(m_animationComponnent->GetAnimationDuration()* 2);
                m_animationTimer = animationDuration;
                m_IsAnimating = true;
            }
        }
    }
    else
    {

        m_FallSpeed += m_Gravity * dae::GameTime::GetDeltaTime();
        auto newPosition = m_Owner->GetWorldPosition();
        newPosition.y += m_FallSpeed * dae::GameTime::GetDeltaTime();
        m_Owner->SetLocalPosition(newPosition);

        if (CheckCollisionWithGround() && !dae::SceneData::GetInstance().IsOnwalkthrough(*m_Owner) || CheckCollisionWithObjects())
        {
            Die();
        }
    }

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
}


void Rock::Die()
{

    m_IsFalling = false;
    m_animationComponnent->Play("Dying"); 

    // Get the duration of the "Dying" animation
    const auto animationDuration = static_cast<float>(m_animationComponnent->GetAnimationDuration());


    m_timer = animationDuration; 
}

bool Rock::ShouldStartFalling()
{
    // Get the current position of the rock
    glm::vec3 currentPosition = m_Owner->GetWorldPosition();

    // Move down slightly to check if there's a floor directly below
    glm::vec3 checkPosition = currentPosition;
    checkPosition.y += 40;

    auto& sceneData = dae::SceneData::GetInstance();
    const auto& floors = sceneData.GetWalkThrough();

    for (const auto& floor : floors)
    {
        if (floor == m_Owner) continue; // Skip self-collision

        if (auto floorHitBox = floor->GetComponent<HitBox>())
        {
            SDL_Rect floorRect = floorHitBox->GetRect();
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
    auto hitBox = m_Owner->GetComponent<HitBox>();
    if (!hitBox) return false;

    SDL_Rect rockRect = hitBox->GetRect();
    int rockLeft = rockRect.x;
    int rockRight = rockRect.x + rockRect.w;

    auto& sceneData = dae::SceneData::GetInstance();
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
    }

    // No players underneath, the rock should start falling
    return true;
}


bool Rock::CheckCollisionWithGround()
{

    glm::vec3 currentPosition = m_Owner->GetWorldPosition();


    float checkDistance = 10.0f;
    glm::vec3 checkPosition = currentPosition;
    checkPosition.y -= checkDistance;

  
    auto& sceneData = dae::SceneData::GetInstance();
    const auto& floors = sceneData.GetWalkThrough();

    for (const auto& floor : floors)
    {
        if (!floor) continue;

        auto hitBox = floor->GetComponent<HitBox>();
        if (!hitBox) continue;

        SDL_Rect floorRect = hitBox->GetRect();


        glm::vec3 floorPosition = floor->GetWorldPosition();

        if (checkPosition.x >= floorPosition.x && checkPosition.x < floorPosition.x + floorRect.w &&
            checkPosition.y >= floorPosition.y && checkPosition.y < floorPosition.y + floorRect.h)
        {
            // Found a valid floor underneath
                return true;
           
        }
    }

    // No valid ground underneath
    return false;
}

bool Rock::CheckCollisionWithObjects()
{
    auto& sceneData = dae::SceneData::GetInstance();
    const auto& objectsToCheck = sceneData.GetPlayers(); 

    for (const auto& object : objectsToCheck)
    {
        if (object == m_Owner) continue; // Skip self-collision

        const auto hitBox = m_Owner->GetComponent<HitBox>();
        const auto objectHitBox = object->GetComponent<HitBox>();
        if (hitBox && objectHitBox && hitBox->IsColliding(*objectHitBox))
        {
            return true;
        }
    }

    return sceneData.isOnEnemy(*m_Owner);
}
