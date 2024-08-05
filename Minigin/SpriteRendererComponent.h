#pragma once
#include <glm/vec2.hpp>

#include "Component.h"

namespace dae
{
    class GameObject;
    class Sprite;
    class Texture2D; // Forward declaration of Texture2D

    class SpriteRendererComponent : public Component
    {
    public:
        SpriteRendererComponent(GameObject* gameObject, const Sprite* sprite);
        SpriteRendererComponent(GameObject* gameObject, const Texture2D* texture);

        void SetDrawCell(glm::ivec2 drawCell);
        void SetSprite(const Sprite* spritePtr);
        void SetTexture(const Texture2D* texture);
        const Sprite* GetSprite() const;

        void SetDimensions(float width, float height);
        glm::vec2 GetDimensions() const;

        void SetFlip(bool flipX, bool flipY);


        int GetLastRenderOrder() const { return m_lastRenderOrder; }
        void UpdateLastRenderOrder(int newOrder) { m_lastRenderOrder = newOrder; }

        void Update() override;
        void Render() const override;

        std::type_info const& GetComponentType() const override { return typeid(SpriteRendererComponent); }

    private:
        GameObject* m_gameObject;
        const Sprite* m_spritePtr;
        const Texture2D* m_texturePtr;
        glm::ivec2 m_drawCell;
        float m_width;
        float m_height;
        bool m_flipX;
        bool m_flipY;
        float m_rotation;
        int m_renderOrder;
        int m_lastRenderOrder;
    };
}
