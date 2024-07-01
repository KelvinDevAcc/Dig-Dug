#pragma once
#include "Component.h"

class BurgerComponent final : public dae::Component {
public:
    BurgerComponent(float length, float dropDistance);
    ~BurgerComponent() override = default;

    BurgerComponent(const BurgerComponent& other) = delete;
    BurgerComponent(BurgerComponent&& other) noexcept = delete;
    BurgerComponent& operator=(const BurgerComponent& other) = delete;
    BurgerComponent& operator=(BurgerComponent&& other) noexcept = delete;

    void Update() override;

    void StartDropping();
    bool GetFalling() const;

    std::type_info const& GetComponentType() const override { return typeid(BurgerComponent); }
private:
    bool IsPlayerOverComponent();
    float GetPlayerMovementOverComponent() const;
    void DropToNextFloor();

    dae::GameObject* m_DroppingPlayer;
    bool m_IsDropping;
    float m_Length;
    float m_DropDistance;
    float m_PlayerWalkedDistance;
    bool m_HasDropped;
};
