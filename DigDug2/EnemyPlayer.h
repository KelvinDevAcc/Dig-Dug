#pragma once
#include "Component.h"

#include "AnimationComponent.h"
#include "BreathFireComponent.h"
#include "Player.h"

enum class Enemyanimationstate
{
    Idle,
    Walk_Right,
    Walk_Left,
    Walk_Up,
    Walk_Down,
    Attacking,
    Dying,
    Ghost
};
namespace game
{

	class EnemyPlayer : public dae::Component
    {
    public:
        EnemyPlayer(dae::GameObject* gameObject);
        virtual ~EnemyPlayer() = default;

        EnemyPlayer(const EnemyPlayer& other) = delete;
        EnemyPlayer(EnemyPlayer&& other) noexcept = delete;
        EnemyPlayer& operator=(const EnemyPlayer& other) = delete;
        EnemyPlayer& operator=(EnemyPlayer&& other) noexcept = delete;

        void Update() override;
        void Render() const override;
        void Move(float deltaX, float deltaY);
        void Attack();
        void Ghost();

        std::type_info const& GetComponentType() const override { return typeid(EnemyPlayer); }

    private:
        void MoveHorizontally(float deltaX) const;
        void MoveVertically(float deltaY) const;
        void UpdateAnimationState(Enemyanimationstate state);
        bool IsAtTileCenter(const dae::GameObject& gameObject) const;
        void SnapToTileCenter();
    	dae::GameObject* m_GameObject{ nullptr };
        float m_Speed{ 40.0f };

        dae::AnimationComponent* m_animationComponent;

        Direction m_currnentDirection = Direction::None;
        Enemyanimationstate m_currentanimationState = Enemyanimationstate::Idle;
        BreathFire* m_breathFire;
        bool m_breathingFire;
        float m_warmupAnimationtime;
        float m_cooldownAnimationtime;
    };
}
