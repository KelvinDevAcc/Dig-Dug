#include "TileComponent.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "SpriteRendererComponent.h"

TileComponent::TileComponent(dae::GameObject* owner, TunnelType type)
    : m_Type(type), m_owner(owner)
{
    if (m_owner)
    {
        m_Position = m_owner->GetWorldPosition();
        m_spriterenderer = m_owner->GetComponent<dae::SpriteRendererComponent>();
        SetType(m_Type);  // Ensure initial texture is set based on type
    }
}

TunnelType TileComponent::GetType() const
{
    return m_Type;
}

void TileComponent::SetType(TunnelType type)
{
    m_Type = type;
    switch (m_Type)
    {
    case TunnelType::Empty:
        UpdateTexture("empty");
        break;
    case TunnelType::topEnd:
        UpdateTexture("topEnd");
        break;
    case TunnelType::bottomEnd:
        UpdateTexture("bottomEnd");
        break;
    case TunnelType::leftEnd:
        UpdateTexture("leftEnd");
        break;
    case TunnelType::rightEnd:
        UpdateTexture("rightEnd");
        break;
    case TunnelType::walkThroughUp:
        UpdateTexture("walkTroughUp");
        break;
    case TunnelType::walkThroughLeft:
        UpdateTexture("walkTroughLeft");
        break;
    case TunnelType::UpRight:
        UpdateTexture("UpRight");
        break;
    case TunnelType::UpLeft:
        UpdateTexture("UpLeft");
        break;
    case TunnelType::BottomRight:
        UpdateTexture("BottomRight");
        break;
    case TunnelType::BottomLeft:
        UpdateTexture("BottomLeft");
        break;
    case TunnelType::MiddleBlock:
        UpdateTexture("MiddleBlock");
        break;
    default:
        break;
    }
}

const glm::vec3& TileComponent::GetPosition() const
{
    return m_Position;
}

void TileComponent::UpdateTexture(const std::string& name)
{
    if (m_spriterenderer)
    {
        m_spriterenderer->SetTexture(dae::ResourceManager::GetTexture(dae::HashString(name)));
        m_spriterenderer->SetDimensions(40, 40);
    }
}

void TileComponent::Update()
{
    // Implementation for Update, if needed
}
