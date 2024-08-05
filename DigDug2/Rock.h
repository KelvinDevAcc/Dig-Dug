#pragma once
#include "AnimationComponent.h"
#include "Component.h"
#include "Scene.h"

class Rock : public dae::Component
{
public:
    explicit Rock(dae::GameObject* owner);
    ~Rock() override = default;

    Rock(const Rock& other) = delete;
    Rock& operator=(const Rock& other) = delete;
    Rock(Rock&& other) noexcept = delete;
    Rock& operator=(Rock&& other) noexcept = delete;

    void Update() override;
    void Die();


    std::type_info const& GetComponentType() const override { return typeid(Rock); }


private:
    bool ShouldStartFalling();
    bool CheckCollisionWithWalkThrough();
    bool CheckCollisionWithObjects();
    bool NoplayerUntherneath();

    bool m_IsFalling;
    float m_FallSpeed;
    float m_Gravity; // Gravity constant
    dae::GameObject* m_Owner;
    dae::AnimationComponent* m_animationComponnent;
    float m_timer;
    bool m_PlayerMovedAway;
    bool m_IsAnimating;
    float m_animationTimer;
    bool m_dieing;
};

