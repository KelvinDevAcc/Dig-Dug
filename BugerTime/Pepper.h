#pragma once
#include "AnimationComponent.h"
#include "HitBox.h"

namespace game
{
    class Pepper final : public dae::Component
    {
    public:
        Pepper(dae::GameObject* owner, const glm::vec3& direction, float speed, float lifetime);

        ~Pepper() override = default;

        Pepper(const Pepper& other) = delete;
        Pepper(Pepper&& other) noexcept = delete;
        Pepper& operator=(const Pepper& other) = delete;
        Pepper& operator=(Pepper&& other) noexcept = delete;

        void Update() override;
        void Render() const override;

        void Activate();
        void Deactivate();

        const std::type_info& GetComponentType() const override { return typeid(Pepper); }

    private:
        glm::vec3 m_direction;
        float m_speed;
        float m_lifetime;
        bool m_isActive;
        HitBox* m_hitBox;
        dae::AnimationComponent* m_animationComponent;

        dae::GameObject* m_owner;

        std::unique_ptr<dae::GameObject> m_perperObject;
    };
}

