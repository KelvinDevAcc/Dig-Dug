#pragma once
#include "BreathFireComponent.h"
#include "EnemyComponent.h"

class FygarComponent : public EnemyComponent
{
    friend class EnemyBreathingFireState;
    friend class EnemyWandering;

public:
    FygarComponent(dae::GameObject* owner);
    ~FygarComponent() override = default;

    void Update() override;

protected:
    // Fygar-specific methods
    void UpdateFireBreath();
    bool ShouldBreatheFire() const;
    void BreatheFire();
    void ResetFireBreathCooldown();

private:
    glm::vec3 m_Direction;
    float m_FireBreathCooldown;  // Timer for when Fygar can breathe fire again
    float m_FireBreathInterval;  // Interval between fire breaths
    game::BreathFire* m_breathFire;
};
