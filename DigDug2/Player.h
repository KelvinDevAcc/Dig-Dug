#pragma once
#include "AnimationComponent.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "LoadMap.h"
#include "PointComponent.h"
#include "Pump.h"

namespace game
{
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
        void Render() const override;
        void Move(float deltaX, float deltaY);
        void Die();
        void ReSpawn();
        void Idle();
        void ShootPump();

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
        void SetAnimationState(AnimationState state);
        void UpdatePumpTimer(float deltaTime);
        void AddPumpPart();
        void UpdatePumpTextures() const;
        std::unique_ptr<Pump> GetInactivePump();

        float m_timeSinceLastPumpPart{ 0.0f };
        float m_pumpPartInterval{ 0.1f };

        int m_pumpPartCount;
        AnimationState m_CurrentAnimationState;
        std::vector<std::unique_ptr<Pump>> m_pumps;
        std::vector<std::unique_ptr<Pump>> m_pumpPool;
        glm::vec3 pumpDirection{ 1, 0, 0 };
        size_t m_maxPumps;
    };
}