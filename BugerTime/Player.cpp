#include "Player.h"
#include "GameTime.h"
#include "Pepper.h"
#include "SceneData.h"
#include "SceneHelpers.h"

namespace game
{
    Player::Player(dae::GameObject* gameObject)
        : m_CurrentState(nullptr), m_GameObject(gameObject),
        m_timeSinceLastAction(0.0f), m_inactivityThreshold(1.0f),
        m_gameMap(nullptr)
    {

        m_animationComponent = m_GameObject->GetComponent<dae::AnimationComponent>();
        m_healthComponent = m_GameObject->GetComponent<dae::HealthComponent>();
        m_pointComponent = m_GameObject->GetComponent<dae::PointComponent>();
        m_pepperAttackComponent = m_GameObject->GetComponent<Pepper>();
        m_startPosition = m_GameObject->GetWorldPosition();

        SetState(m_idleState.get());
    }

    void Player::SetState(PlayerState* state)
    {
	    if (m_CurrentState == state)
            return;

        if (m_CurrentState)
        {
            m_CurrentState->OnExitState(*this);
        }
        m_CurrentState = state;
        if (m_CurrentState)
        {
            m_CurrentState->OnEnterState(*this);
            m_timeSinceLastAction = 0.0f;
        }
    }

    void Player::Update()
    {
        if (m_CurrentState)
        {
            m_CurrentState->Update(*this);

            m_timeSinceLastAction += dae::GameTime::GetDeltaTime();

            if (m_timeSinceLastAction >= m_inactivityThreshold)
            {
                Idle();
            }

        }
    }

    void Player::Move(float deltaX, float deltaY)
    {
        CheckAndMove(deltaX, deltaY);
    }

    void Player::Attack() {
        if (!m_pointComponent) {
            return;
        }
        m_pepperAttackComponent->Activate();
        SetState(m_attackignState.get());
    }

    void Player::Die() {
        if (!m_healthComponent) {
            return;
        }
        SetState(m_dyingState.get());
    }

    void Player::Idle() {
        SetState(m_idleState.get());
    }

    void Player::Respawn()
    {
        m_GameObject->SetLocalPosition(m_startPosition);
        Idle();
    }

    void Player::CheckAndMove(float deltaX, float deltaY)
    {
        const auto& sceneData = dae::SceneData::GetInstance();

        const bool isOnLadderUp = sceneData.IsOnLadderUp(*m_GameObject);
        const bool isOnLadderUpDown = sceneData.IsOnLadderUpDown(*m_GameObject);
        const bool isOnLadderDown = sceneData.IsOnLadderDown(*m_GameObject);
        const bool isOnSolidLadder = sceneData.IsOnSolidLadder(*m_GameObject);
        const bool isOnFloor = sceneData.IsOnFloor(*m_GameObject);

        m_deltaX = deltaX * 20 * dae::GameTime::GetDeltaTime();
        m_deltaY = deltaY * 20 * dae::GameTime::GetDeltaTime();

        bool hasMoved = false;

        if (isOnLadderUp && deltaY < 0 && sceneData.CanEntityMove(m_deltaX, m_deltaY, *m_GameObject)) {
            MoveVertically(m_deltaY);
            hasMoved = true;
        }

        else if (isOnLadderDown && deltaY > 0 && sceneData.CanEntityMove(m_deltaX, m_deltaY, *m_GameObject)) {
            MoveVertically(m_deltaY);
            hasMoved = true;
        }

        else if (isOnSolidLadder && sceneData.CanEntityMove(0.0f, m_deltaY, *m_GameObject)) {
            MoveVertically(m_deltaY);
            hasMoved = true;
        }

        else if (isOnLadderUpDown && sceneData.CanEntityMove(0.0f, m_deltaY, *m_GameObject)) {
            MoveVertically(m_deltaY);
            hasMoved = true;
        }

        if ((isOnLadderUp || isOnLadderDown || isOnLadderUpDown || isOnFloor) && sceneData.CanEntityMove(m_deltaX, 0.0f, *m_GameObject)) {
            MoveHorizontally(m_deltaX);
            hasMoved = true;
        }

        if (!hasMoved) {
            SetState(m_idleState.get());
        }
    }

    void Player::MoveHorizontally(float deltaX)
    {
        glm::vec3 currentPosition = m_GameObject->GetWorldPosition();
        currentPosition.x += deltaX ; 
        m_GameObject->SetLocalPosition(currentPosition);
        SetState(m_walkingState.get());
    }

    void Player::MoveVertically(float deltaY)
    {
        glm::vec3 currentPosition = m_GameObject->GetWorldPosition();
        currentPosition.y += deltaY; 
        m_GameObject->SetLocalPosition(currentPosition);
        SetState(m_walkingState.get());
    }
}
