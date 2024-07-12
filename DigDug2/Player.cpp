#include "Player.h"

#include "GameData.h"
#include "GameTime.h"
#include "SceneData.h"
#include "SceneHelpers.h"
#include "SceneManager.h"

namespace game
{
    Player::Player(dae::GameObject* gameObject)
        : m_GameObject(gameObject),
        m_timeSinceLastAction(0.0f), m_inactivityThreshold(2.0f),
        m_CurrentAnimationState(AnimationState::Dying)
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
    }

    void Player::Move(float deltaX, float deltaY)
    {
	    const float scaledDeltaX = deltaX * 20 * dae::GameTime::GetDeltaTime();
	    const float scaledDeltaY = deltaY * 20 * dae::GameTime::GetDeltaTime();

        MoveVertically(scaledDeltaY);
        MoveHorizontally(scaledDeltaX);
    }

    void Player::Attack()
    {
        if (!m_pointComponent) {
            return;
        }
        SetAnimationState(AnimationState::Attacking);
        const int newScore = m_pointComponent->GetScore() + 100;
        m_pointComponent->SetScore(newScore);
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

    void Player::Idle()
    {
        SetAnimationState(AnimationState::Idle);
    }

    void Player::Respawn()
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
        }
        else if (deltaX < 0) {
           m_animationComponent->Play("Walk_Left", true, 0);
           m_animationComponent->FlipSprite(false, false);
        }
    }

    void Player::MoveVertically(float deltaY)
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
            break;
        case AnimationState::Walk_Right:
            if (m_animationComponent)
                m_animationComponent->Play("Walk_Right", true);
            break;
        case AnimationState::Walk_Left:
            if (m_animationComponent)
                m_animationComponent->Play("Walk_Left", true);
            break;
        case AnimationState::Walk_Down:
            if (m_animationComponent)
                m_animationComponent->Play("Walk_Down", true);
            break;
        case AnimationState::Attacking:
            if (m_animationComponent)
                m_animationComponent->Play("Attacking");
            break;
        case AnimationState::Dying:
            if (m_animationComponent)
                m_animationComponent->Play("Dying");
            break;

default: ;
        }
    }
}
