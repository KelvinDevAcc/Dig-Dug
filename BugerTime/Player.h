#pragma once
#include "AnimationComponent.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "LoadMap.h"
#include "Pepper.h"
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

        void SetState(PlayerState* state);

        void Update() override;

        void Move(float deltaX, float deltaY);
        void Attack();
        void Die();
        void Idle();

        void Respawn();


        dae::GameObject* GetParentObject() const { return m_GameObject; }

        std::type_info const& GetComponentType() const override { return typeid(Player); }


        std::unique_ptr<MovingState> m_walkingState{ std::make_unique<MovingState>() };
        std::unique_ptr<AttackingState> m_attackignState{ std::make_unique<AttackingState>() };
        std::unique_ptr<IdleState> m_idleState{ std::make_unique<IdleState>() };
        std::unique_ptr<DyingState> m_dyingState{ std::make_unique<DyingState>() };

        PlayerState* m_CurrentState; 

        
        float m_deltaY{ 0 };
        float m_deltaX{ 0 };

    private:
       
        dae::GameObject* m_GameObject;

        dae::HealthComponent* m_healthComponent{};
        dae::PointComponent* m_pointComponent{};
        Pepper* m_pepperAttackComponent{};
        dae::AnimationComponent* m_animationComponent{};

        glm::vec3 m_startPosition{};

        float m_timeSinceLastAction;
        float m_inactivityThreshold;

        const LoadMap* m_gameMap;

        void CheckAndMove(float deltaX, float deltaY);

        void MoveHorizontally(float deltaX);
        void MoveVertically(float deltaY);
    };
}

