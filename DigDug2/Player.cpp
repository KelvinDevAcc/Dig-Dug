#include "Player.h"

#include "GameData.h"
#include "GameTime.h"
#include "ResourceManager.h"
#include "SceneData.h"
#include "SceneHelpers.h"
#include "SceneManager.h"

namespace game
{
    Player::Player(dae::GameObject* gameObject)
        : m_GameObject(gameObject),
        m_timeSinceLastAction(0.0f), m_inactivityThreshold(2.0f),
        m_CurrentAnimationState(AnimationState::Idle), // Initialize to Idle state
        arrowDirection(glm::vec3(1, 0, 0))
    {
        m_animationComponent = m_GameObject->GetComponent<dae::AnimationComponent>();
        m_healthComponent = m_GameObject->GetComponent<dae::HealthComponent>();
        m_pointComponent = m_GameObject->GetComponent<dae::PointComponent>();
        m_startPosition = m_GameObject->GetWorldPosition();

        SetAnimationState(AnimationState::Idle);
    }

    void Player::Update()
    {
        m_timeSinceLastAction += dae::GameTime::GetDeltaTime();
        if (m_timeSinceLastAction >= m_inactivityThreshold)
        {
            Idle();
            m_timeSinceLastAction = 0;
        }

        for (const auto& arrow : m_arrows)
        {
            if (arrow && arrow->IsActive())
            {
                arrow->Update();
            }
        }

        const float deltaTime = dae::GameTime::GetDeltaTime();

        UpdateArrowTimer(deltaTime);

        if (m_healthComponent->GetLives() < 0)
        {
            m_healthComponent->SetLives(-1);

            const auto playerSize = dae::SceneData::GetInstance().GetPlayers().size();
            if (playerSize == 1)
            {
                GameData::GetInstance().FindAndStorePlayerData();
                dae::SceneData::GetInstance().RemoveGameObject(GetParentObject(), dae::GameObjectType::Player);
                dae::SceneManager::GetInstance().GetActiveScene()->Remove(GetParentObject());
                //dae::SceneManager::GetInstance().SetActiveScene("SaveScoreScene");
            }
            else if (playerSize > 1)
            {
                //dae::SceneManager::GetInstance().SetActiveScene("SaveScoreScene");
                GameData::GetInstance().FindAndStorePlayerData();
                dae::SceneData::GetInstance().RemoveGameObject(GetParentObject(), dae::GameObjectType::Player);
                dae::SceneManager::GetInstance().GetActiveScene()->Remove(GetParentObject());
                //dae::SceneManager::GetInstance().SetActiveScene("SaveScoreScene");
            }
        }
    }

    void Player::Move(float deltaX, float deltaY) const
    {
	    const float scaledDeltaX = deltaX * 20 * dae::GameTime::GetDeltaTime();
	    const float scaledDeltaY = deltaY * 20 * dae::GameTime::GetDeltaTime();

        MoveVertically(scaledDeltaY);
        MoveHorizontally(scaledDeltaX);
    }

    void Player::Die()
    {
        if (!m_healthComponent) {
            return;
        }
        SetAnimationState(AnimationState::Dying);
        dae::Message message;

        message.type = dae::PlaySoundMessageType::Sound;

        message.arguments.emplace_back(static_cast<sound_id>(2));

        dae::EventQueue::Broadcast(message);

        const int newHealth = m_healthComponent->GetHealth() - 100;
        m_healthComponent->SetHealth(newHealth);
    }

    void Player::Render() const
    {
        for (const auto& arrow : m_arrows)
        {
            if (arrow && arrow->IsActive())
            {
                arrow->Render();
            }
        }
    }

    void Player::ShootArrow()
    {
        if (!m_arrows.empty() && m_arrows.back()->IsActive())
        {
            return; // An arrow is already active, so don't shoot a new one
        }

        // Reset the timer for adding new arrow parts
        m_timeSinceLastArrowPart = 0.0f;

        // Start by adding the first arrow part immediately
        AddArrowPart();
        if (!m_pointComponent) {
            return;
        }
        SetAnimationState(AnimationState::Attacking);
        const int newScore = m_pointComponent->GetScore() + 100;
        m_pointComponent->SetScore(newScore);
    }

    void Player::UpdateArrowTimer(float deltaTime)
    {
        if (m_arrows.empty() || m_arrowPartCount > 5)
        {
            return; // No active arrows to update
        }
        m_timeSinceLastArrowPart += deltaTime;
        if (m_timeSinceLastArrowPart >= m_arrowPartInterval)
        {
            m_timeSinceLastArrowPart = 0.0f; // Reset the timer
            AddArrowPart(); // Add a new arrow part
        }
    }

    void Player::AddArrowPart()
    {
        // Calculate position offset based on the size of the previous arrow's texture
        glm::vec3 offsetPosition(0.0f);
        if (!m_arrows.empty())
        {
            const auto& prevArrow = m_arrows.back(); // Get reference to the unique_ptr
            if (prevArrow)
            {
                constexpr auto textureSize = glm::vec2(36, 16); // Assuming you have a method to get texture size
                offsetPosition.x += m_arrowPartCount  * textureSize.x ;
            }
        }

        glm::vec3 arrowPosition = offsetPosition;

        auto arrow = std::make_unique<Arrow>(m_GameObject, arrowPosition, arrowDirection, 10.0f); // Adjust speed and lifetime as needed
        arrow->SetGameObject(m_GameObject);
        arrow->Activate();

        // Add the new arrow part to the list
        m_arrows.push_back(std::move(arrow));
        ++m_arrowPartCount; // Increment the arrow part count

        // Update textures for the arrows to ensure the last one remains "ArrowEnd"
        UpdateArrowTextures();
    }

    void Player::UpdateArrowTextures() const
    {
        for (size_t i = 0; i < m_arrows.size(); ++i)
        {
            if (i == m_arrows.size() - 1)
            {
                // Last arrow should keep "ArrowEnd" texture
                m_arrows[i]->SetTexture(dae::ResourceManager::GetTexture("ArrowEnd"));
            }
            else
            {
                // Previous arrows should use "ArrowLine" texture
                m_arrows[i]->SetTexture(dae::ResourceManager::GetTexture("ArrowLine"));
            }
        }
    }

    void Player::Idle()
    {
        SetAnimationState(AnimationState::Idle);
    }

    void Player::ReSpawn()
    {
        m_GameObject->SetLocalPosition(m_startPosition);
        Idle();
    }

    void Player::MoveHorizontally(float deltaX) const
    {
        glm::vec3 currentPosition = m_GameObject->GetWorldPosition();
        currentPosition.x += deltaX;
        m_GameObject->SetLocalPosition(currentPosition);
        if (deltaX > 0) {
            m_animationComponent->Play("Walk_Right", true, 0);
            m_animationComponent->FlipSprite(true, false);
        }
        else if (deltaX < 0) {
           m_animationComponent->Play("Walk_Left", true, 0);
           m_animationComponent->FlipSprite(false, false);
        }
    }

    void Player::MoveVertically(float deltaY) const
    {
        glm::vec3 currentPosition = m_GameObject->GetWorldPosition();
        currentPosition.y += deltaY;
        m_GameObject->SetLocalPosition(currentPosition);
        if (deltaY > 0) {
            m_animationComponent->Play("Walk_Down", true, 0);
        }
        else if (deltaY < 0) {
            m_animationComponent->Play("Walk_Up", true, 0);
        }
    }

    void Player::SetAnimationState(AnimationState state)
    {
        if (m_CurrentAnimationState == state)
            return;

        m_CurrentAnimationState = state;

        switch (state)
        {
        case AnimationState::Idle:
            if (m_animationComponent)
                m_animationComponent->Play("Idle", true);
            break;
        case AnimationState::Walk_Up:
            if (m_animationComponent)
                m_animationComponent->Play("Walk_Up",true);
            arrowDirection = glm::vec3(0, -1, 0);
            break;
        case AnimationState::Walk_Right:
            if (m_animationComponent)
                m_animationComponent->Play("Walk_Right", true);
            arrowDirection = glm::vec3(1, 0, 0);
            break;
        case AnimationState::Walk_Left:
            if (m_animationComponent)
                m_animationComponent->Play("Walk_Left", true);
            arrowDirection = glm::vec3(-1, 0, 0);
            break;
        case AnimationState::Walk_Down:
            if (m_animationComponent)
                m_animationComponent->Play("Walk_Down", true);
            arrowDirection = glm::vec3(0, 1, 0);
            break;
        case AnimationState::Attacking:
           if (m_animationComponent)
               m_animationComponent->Play("Attacking");
            break;
        case AnimationState::Dying:
            if (m_animationComponent)
                m_animationComponent->Play("Dying");
            break;
        }
    }
}
