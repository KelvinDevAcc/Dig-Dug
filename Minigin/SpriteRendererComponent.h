#pragma once
#include <glm/vec2.hpp>

#include "Sprite.h"

#include "Component.h"


namespace dae
{

    class SpriteRendererComponent final : public Component
    {
    public:
        SpriteRendererComponent(GameObject* gameObject, const Sprite* sprite, glm::ivec2 drawCell = {});

        ~SpriteRendererComponent() override = default;

        SpriteRendererComponent(const SpriteRendererComponent& other) = delete;
        SpriteRendererComponent(SpriteRendererComponent&& other) noexcept = delete;
        SpriteRendererComponent& operator=(const SpriteRendererComponent& other) = delete;
        SpriteRendererComponent& operator=(SpriteRendererComponent&& other) noexcept = delete;

        void SetDrawCell(glm::ivec2 drawCell);
        void SetSprite(const Sprite* spritePtr);

        void Update() override;
    	const Sprite* GetSprite() const;

        void SetDimensions(float width, float height);
        glm::vec2 GetDimensions() const;
        void Setflip(bool flipx, bool flipy);

        std::type_info const& GetComponentType() const override { return typeid(SpriteRendererComponent); }

    private:
        void Render() const override;

        GameObject* m_gameObject;

        float m_width;
        float m_height;

        bool m_flipX;
        bool m_flipY;

        glm::ivec2 m_drawCell;
        const Sprite* m_spritePtr;
    };
}
