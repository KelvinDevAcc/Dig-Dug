#include "SpriteRendererComponent.h"
#include "Sprite.h"
#include "GameObject.h"
#include "Renderer.h"

dae::SpriteRendererComponent::SpriteRendererComponent(dae::GameObject* gameObject, const Sprite* sprite, glm::ivec2 drawCell) :
    m_gameObject(gameObject),m_drawCell(drawCell)
{
    SetSprite(sprite);
}

void dae::SpriteRendererComponent::SetDrawCell(glm::ivec2 drawCell)
{
    m_drawCell = drawCell;
}

void dae::SpriteRendererComponent::SetSprite(const Sprite* spritePtr)
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

void dae::SpriteRendererComponent::Update()
{
}

const dae::Sprite* dae::SpriteRendererComponent::GetSprite() const
{
	return m_spritePtr;
}

void dae::SpriteRendererComponent::SetDimensions(float width, float height)
{
    m_width = width;
    m_height = height;
}

glm::vec2 dae::SpriteRendererComponent::GetDimensions() const
{
    return {m_width, m_height};
}

void dae::SpriteRendererComponent::Setflip(bool flipx, bool flipy) 
{
    m_flipX = flipx;
    m_flipY = flipy;
}

void dae::SpriteRendererComponent::Render() const
{
    if (m_spritePtr == nullptr)
        return;

    const glm::vec2& pos = m_gameObject->GetWorldPosition();

    glm::vec2 MidlePoint = pos;
    MidlePoint.x -= m_width / 2.0f;
    MidlePoint.y -= m_height / 2.0f;


    const glm::ivec2& cellSize = m_spritePtr->cellSize;

    Renderer::GetInstance().RenderTexture(m_spritePtr->GetTexture(), // Passing texture pointer
        MidlePoint,                         // Position
        m_drawCell,                // Source location
        cellSize,                    // Cell size
        m_width,         // Width (for scaling)
        m_height,        // Height (for scaling)
        m_flipX,                     // FlipX
        m_flipY                      // FlipY
    );

}