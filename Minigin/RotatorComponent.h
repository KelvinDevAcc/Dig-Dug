#pragma once
#include "Component.h"
#include "GameObject.h"


namespace dae
{
    class RotatorComponent final : public Component
    {
    public:
        RotatorComponent(GameObject* m_GameObject, float rotationSpeed, float centerX, float centerY, float radius);
        ~RotatorComponent() override = default;

        RotatorComponent(const RotatorComponent& other) = delete;
        RotatorComponent(RotatorComponent&& other) noexcept = delete;
        RotatorComponent& operator=(const RotatorComponent& other) = delete;
        RotatorComponent& operator=(RotatorComponent&& other) noexcept = delete;

        void Update() override;


        std::type_info const& GetComponentType() const override { return typeid(RotatorComponent); }


    private:
        float m_RotationSpeed; 
        float m_CurrentRotation; 
        float m_CenterX;
        float m_CenterY;
        float m_Radius;

        GameObject* m_GameObject;
    };
}

