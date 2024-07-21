#include "SpriteRendererComponent.h"
#include "Sprite.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Texture2D.h" // Include Texture2D header

using namespace dae;

SpriteRendererComponent::SpriteRendererComponent(GameObject* gameObject, const Sprite* sprite)
    : m_gameObject(gameObject), m_spritePtr(sprite), m_texturePtr(nullptr), m_drawCell(0, 0), m_width(0.0f), m_height(0.0f), m_flipX(false), m_flipY(false), m_rotation(0.0f)
{
    SetSprite(sprite);
}

SpriteRendererComponent::SpriteRendererComponent(GameObject* gameObject, const Texture2D* texture)
    : m_gameObject(gameObject), m_spritePtr(nullptr), m_texturePtr(texture), m_drawCell(0, 0), m_width(0.0f), m_height(0.0f), m_flipX(false), m_flipY(false), m_rotation(0.0f)
{
    SetTexture(texture);
}

void SpriteRendererComponent::SetDrawCell(glm::ivec2 drawCell)
{
    m_drawCell = drawCell;
}

void SpriteRendererComponent::SetSprite(const Sprite* spritePtr)
{
    if (m_spritePtr == spritePtr)
        return;

    m_spritePtr = spritePtr;

    if (m_spritePtr)
    {
        const auto size = m_spritePtr->GetTexture().GetSize();
        m_width = static_cast<float>(size.x);
        m_height = static_cast<float>(size.y);
    }
}

void SpriteRendererComponent::SetTexture(const Texture2D* texture)
{
    if (m_texturePtr == texture)
        return;

    m_texturePtr = texture;

    if (m_texturePtr)
    {
        const auto size = m_texturePtr->GetSize();
        m_width = static_cast<float>(size.x);
        m_height = static_cast<float>(size.y);
    }
}


const Sprite* SpriteRendererComponent::GetSprite() const
{
    return m_spritePtr;
}

void SpriteRendererComponent::SetDimensions(float width, float height)
{
    m_width = width;
    m_height = height;
}

glm::vec2 SpriteRendererComponent::GetDimensions() const
{
    return { m_width, m_height };
}

void SpriteRendererComponent::SetFlip(bool flipX, bool flipY)
{
    m_flipX = flipX;
    m_flipY = flipY;
}

void SpriteRendererComponent::Update()
{
    // Implement update logic if needed
}

void SpriteRendererComponent::Render() const
{
    const Texture2D* texture = nullptr;
    glm::ivec2 cellSize(0, 0);

    if (m_spritePtr) {
        texture = &m_spritePtr->GetTexture();
        cellSize = m_spritePtr->cellSize;
    }
    else if (m_texturePtr) {
        texture = m_texturePtr;
        cellSize = texture->GetSize();
    }

    if (texture) {
        const glm::vec2& pos = m_gameObject->GetWorldPosition();
        const glm::vec2 middlePoint = pos - glm::vec2(m_width / 2.0f, m_height / 2.0f);
        float rotation = m_gameObject->GetRotation();

        Renderer::GetInstance().RenderTexture(*texture,
            middlePoint,
            m_drawCell,
            cellSize,
            m_width,
            m_height,
            m_flipX,
            m_flipY,
            rotation
        );
    }
}


