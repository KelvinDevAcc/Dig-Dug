#pragma once

#include "GameObject.h"
#include "AnimationComponent.h"
#include "SpriteRendererComponent.h"
#include "HitBox.h"
#include "Scene.h"

namespace game
{
    class BreathFire : public dae::Component
    {
    public:
        BreathFire(dae::GameObject* owner, const glm::vec3& direction, float speed, float lifetime);

        void Update() override;
        void Render() const override;

        void Activate(const glm::vec3& direction);
        void Deactivate();
        bool IsActive() const { return m_isActive; }
        void AdjustPosition();

        std::type_info const& GetComponentType() const override { return typeid(BreathFire); }

    private:
        void UpdateSizeAndAnimation();

        glm::vec3 m_direction;
        float m_speed;
        float m_lifetime;
        bool m_isActive;

        dae::GameObject* m_owner;
        std::unique_ptr<dae::GameObject> m_breathFireObject;
        dae::SpriteRendererComponent* m_spriterendererComponent;
        HitBox* m_hitBox;
        int m_stage; // Keeps track of the current stage of the fire
    };
}
