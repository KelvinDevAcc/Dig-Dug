#pragma once
#include "GameObject.h"
#include "HitBox.h"
#include "SpriteRendererComponent.h"

class Pump : public dae::Component{
public:
    Pump(dae::GameObject* owner, const glm::vec3& localPosition, const glm::vec3& direction, float lifetime);
    void Update() override;
    void Render() const override;
    void Activate();
    bool IsActive() const { return m_isActive; }
    dae::GameObject* GetPumpObject() const { return m_pumpObject.get(); }
    void Deactivate();
    void SetTexture(const dae::Texture2D* texture) const;
    void SetPosition(const glm::vec3& position);
    void SetDirection(const glm::vec3& direction);
    static float GetRotationFromDirection(const glm::vec3& direction);
    void SetLifetime(float lifetime);
    bool IsCollidingWithEnemy();
    void PumpEnemy();


    std::type_info const& GetComponentType() const override { return typeid(Pump); }

private:

    dae::GameObject* m_owner;
    std::unique_ptr<dae::GameObject> m_pumpObject;
    glm::vec3 m_LocalPosition;
    glm::vec3 m_direction;
    float m_speed;
    float m_lifetime;
    bool m_isActive;
    bool m_isPumping;
    dae::GameObject* m_connectedEnemy;
    dae::SpriteRendererComponent* m_spriteRenderComponent;
    HitBox* m_hitBox;
};
