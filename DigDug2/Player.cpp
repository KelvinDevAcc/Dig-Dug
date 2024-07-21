#include "Player.h"
#include "EnemyComponent.h"
#include <iostream>
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
        m_timeSinceLastAction(0.0f), m_inactivityThreshold(4.0f),
        m_CurrentAnimationState(AnimationState::Idle),
        pumpDirection(glm::vec3(1, 0, 0))
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

        for (const auto& pump : m_pumps)
        {
            if (pump && pump->IsActive())
            {
                pump->Update();
            }
        }

        const float deltaTime = dae::GameTime::GetDeltaTime();
        UpdatePumpTimer(deltaTime);

        if (m_healthComponent->GetLives() < 0)
        {
            m_healthComponent->SetLives(-1);
            const auto playerSize = dae::SceneData::GetInstance().GetPlayers().size();
            if (playerSize == 1)
            {
                GameData::GetInstance().FindAndStorePlayerData();
                dae::SceneData::GetInstance().RemoveGameObject(GetParentObject(), dae::GameObjectType::Player);
                dae::SceneManager::GetInstance().GetActiveScene()->Remove(GetParentObject());
            }
            else if (playerSize > 1)
            {
                GameData::GetInstance().FindAndStorePlayerData();
                dae::SceneData::GetInstance().RemoveGameObject(GetParentObject(), dae::GameObjectType::Player);
                dae::SceneManager::GetInstance().GetActiveScene()->Remove(GetParentObject());
            }
        }

        //std::cout << "Pump Direction: " << pumpDirection.x << ":" << pumpDirection.y << ":" << pumpDirection.z << std::endl;
    }

    void Player::Move(float deltaX, float deltaY)
    {
        const float scaledDeltaX = deltaX * 50 * dae::GameTime::GetDeltaTime();
        const float scaledDeltaY = deltaY * 50 * dae::GameTime::GetDeltaTime();

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
        for (const auto& pump : m_pumps)
        {
            if (pump && pump->IsActive())
            {
                pump->Render();
            }
        }
    }

    void Player::ShootPump()
    {
        m_pumps.clear();
        m_pumpPartCount = 0;
        if (!m_pumps.empty() && m_pumps.back()->IsActive())
        {
            return; // A pump is already active, so don't shoot a new one
        }

        m_timeSinceLastPumpPart = 0.0f; // Reset the timer for adding new pump parts
        AddPumpPart(); // Start by adding the first pump part immediately
        if (!m_pointComponent) {
            return;
        }
        SetAnimationState(AnimationState::Attacking);
    }

    void Player::UpdatePumpTimer(float deltaTime)
    {
        if (m_pumps.empty() || m_pumpPartCount > 5)
        {
            return; // No active pumps to update
        }
        m_timeSinceLastPumpPart += deltaTime;
        if (m_timeSinceLastPumpPart >= m_pumpPartInterval)
        {
            m_timeSinceLastPumpPart = 0.0f; // Reset the timer
            AddPumpPart(); // Add a new pump part
        }
    }


    void Player::AddPumpPart()
    {
        glm::vec3 offsetPosition(0.0f);
        if (!m_pumps.empty())
        {
            const auto& prevPump = m_pumps.back();
            if (prevPump)
            {
                constexpr auto textureSize = glm::vec2(36, 16); // Assuming you have a method to get texture size
                offsetPosition += pumpDirection * glm::vec3(textureSize.x, textureSize.y * 2, 0) * static_cast<float>(m_pumpPartCount);
            }
        }

        glm::vec3 pumpPosition = offsetPosition;

        auto pump = std::make_unique<Pump>(m_GameObject, pumpPosition, pumpDirection, 4.0f); // Adjust speed and lifetime as needed
        pump->SetGameObject(m_GameObject);
        pump->Activate();

        // Add the new arrow part to the list
        m_pumps.push_back(std::move(pump));
        ++m_pumpPartCount; // Increment the arrow part count

        // Update textures for the arrows to ensure the last one remains "ArrowEnd"
        UpdatePumpTextures();
    }

    void Player::UpdatePumpTextures() const
    {
        for (size_t i = 0; i < m_pumps.size(); ++i)
        {
            if (i == m_pumps.size() - 1)
            {
                m_pumps[i]->SetTexture(dae::ResourceManager::GetTexture("PumpEnd"));
            }
            else
            {
                m_pumps[i]->SetTexture(dae::ResourceManager::GetTexture("PumpLine"));
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

    void Player::MoveHorizontally(float deltaX)
    {
        glm::vec3 currentPosition = m_GameObject->GetWorldPosition();
        currentPosition.x += deltaX;
        m_GameObject->SetLocalPosition(currentPosition);
        if (deltaX > 0) {
            m_animationComponent->Play("Walk_Right", true, 0);
            m_animationComponent->FlipSprite(true, false);
            pumpDirection = glm::vec3(1, 0, 0);
        }
        else if (deltaX < 0) {
            m_animationComponent->Play("Walk_Left", true, 0);
            m_animationComponent->FlipSprite(false, false);
            pumpDirection = glm::vec3(-1, 0, 0);
        }
    }

    void Player::MoveVertically(float deltaY)
    {
        glm::vec3 currentPosition = m_GameObject->GetWorldPosition();
        currentPosition.y += deltaY;
        m_GameObject->SetLocalPosition(currentPosition);
        if (deltaY > 0) {
            m_animationComponent->Play("Walk_Down", true, 0);
            pumpDirection = glm::vec3(0, 1, 0);
        }
        else if (deltaY < 0) {
            m_animationComponent->Play("Walk_Up", true, 0);
            pumpDirection = glm::vec3(0, -1, 0);
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
                m_animationComponent->Play("Walk_Up", true);
            pumpDirection = glm::vec3(0, -1, 0);
            break;
        case AnimationState::Walk_Right:
            if (m_animationComponent)
                m_animationComponent->Play("Walk_Right", true);
            pumpDirection = glm::vec3(1, 0, 0);
            break;
        case AnimationState::Walk_Left:
            if (m_animationComponent)
                m_animationComponent->Play("Walk_Left", true);
            pumpDirection = glm::vec3(-1, 0, 0);
            break;
        case AnimationState::Walk_Down:
            if (m_animationComponent)
                m_animationComponent->Play("Walk_Down", true);
            pumpDirection = glm::vec3(0, 1, 0);
            break;
        case AnimationState::Attacking:
            if (m_animationComponent)
                m_animationComponent->Play("Attacking");
            m_animationComponent->FlipSprite(true, false);
            break;
        case AnimationState::Dying:
            if (m_animationComponent)
                m_animationComponent->Play("Dying");
            break;
        }
    }
}