#pragma once
#include "AnimationComponent.h"
#include "GameObject.h"
#include "HitBox.h"

class Arrow : public dae::Component
{
public:
    Arrow(dae::GameObject* owner, const glm::vec3& localPosition, const glm::vec3& direction, float lifetime);
    ~Arrow() override = default;

    Arrow(const Arrow& other) = delete;
    Arrow(Arrow&& other) noexcept = delete;
    Arrow& operator=(const Arrow& other) = delete;
    Arrow& operator=(Arrow&& other) noexcept = delete;

    void Update() override;
    void Render() const override;
    void Activate();
    void Deactivate();
    bool IsActive() const { return m_isActive; }

    void SetTexture(const dae::Texture2D* texture) const;

    std::type_info const& GetComponentType() const override { return typeid(Arrow); }
private:
    dae::GameObject* m_owner;
    glm::vec3 m_direction;
    glm::vec3 m_LocalPosition;
    float m_speed;
    float m_lifetime;
    bool m_isActive;
    bool m_isEnd;
    std::unique_ptr<dae::GameObject> m_arrowObject;
    dae::SpriteRendererComponent* m_spriteRenderComponent;
    HitBox* m_hitBox;


};
