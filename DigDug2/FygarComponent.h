#pragma once
#include "BreathFireComponent.h"
#include "EnemyComponent.h"

class FygarComponent : public EnemyComponent
{
    friend class EnemyBreathingFireState;
    friend class EnemyWandering;
    friend class EnemyDeadState;

public:
    FygarComponent(dae::GameObject* owner);
    ~FygarComponent() override = default;

    void Update() override;

protected:
    // Fygar-specific methods
    void UpdateFireBreath();
    bool ShouldBreatheFire() const;
    void ResetFireBreathCooldown();
    bool WasKilledFromSide() const;

private:
    glm::vec3 m_Direction;
    float m_FireBreathCooldown;  
    float m_FireBreathInterval; 
    game::BreathFire* m_breathFire;
};
