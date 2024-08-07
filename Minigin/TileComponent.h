#pragma once
#include <string>
#include <glm/vec3.hpp>

#include "Component.h"
#include "SpriteRendererComponent.h"
#include "TunnelType.h"


class TileComponent : public dae::Component
{
public:
    TileComponent(dae::GameObject* m_owner,TunnelType type);

    TunnelType GetType() const;
    void SetType(TunnelType type);

    const glm::vec3& GetPosition() const;

    void UpdateTexture(const std::string& name);


    void Update() override;
    std::type_info const& GetComponentType() const override { return typeid(TileComponent); }

private:
    TunnelType m_Type;
    glm::vec3 m_Position;
    dae::SpriteRendererComponent* m_spriterenderer;
    dae::GameObject* m_owner;
};

