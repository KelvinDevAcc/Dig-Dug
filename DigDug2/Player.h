#pragma once
#include "AnimationComponent.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "LoadMap.h"
#include "PlayerState.h" 
#include "PointComponent.h"

namespace game
{
    class Player : public dae::Component
    {

        friend class MovingState;
        friend class AttackingState;
        friend class IdleState;
        friend class DyingState;

    public:
        Player(dae::GameObject* gameObject);


        ~Player() override = default;

        Player(const Player&) = delete;
        Player(Player&&) noexcept = delete;
        Player& operator=(const Player&) = delete;
        Player& operator=(Player&&) noexcept = delete;


        void Update() override;
        void Move(float deltaX, float deltaY);
        void Attack();
        void Die();
        void Respawn();
        void Idle();


        dae::GameObject* GetParentObject() const { return m_GameObject; }

        std::type_info const& GetComponentType() const override { return typeid(Player); }


        float m_deltaY{ 0 };
        float m_deltaX{ 0 };

    private:

        dae::GameObject* m_GameObject;
        glm::vec3 m_startPosition;
        float m_timeSinceLastAction;
        float m_inactivityThreshold;

        dae::AnimationComponent* m_animationComponent;
        dae::HealthComponent* m_healthComponent;
        dae::PointComponent* m_pointComponent;

        void MoveHorizontally(float deltaX);
        void MoveVertically(float deltaY);

        enum class AnimationState
        {
            Idle,
            Walk_Right,
            Walk_Left,
            Walk_Up,
            Walk_Down,
            Attacking,
            Dying
        };

        AnimationState m_CurrentAnimationState;
        void SetAnimationState(AnimationState state);
    };
}

