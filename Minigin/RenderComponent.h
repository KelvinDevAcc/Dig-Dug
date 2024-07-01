#pragma once

#include <memory>
#include <vector>
#include "Component.h"
#include "Texture2D.h"

namespace dae
{
    class RenderComponent final : public Component
    {
    public:
        RenderComponent(GameObject* gameObject);
        ~RenderComponent() override = default;

        RenderComponent(const RenderComponent& other) = delete;
        RenderComponent(RenderComponent&& other) noexcept = delete;
        RenderComponent& operator=(const RenderComponent& other) = delete;
        RenderComponent& operator=(RenderComponent&& other) noexcept = delete;

        void Update() override;
        void Render() const override;

        void SetTexture(const std::unique_ptr<Texture2D> texture);
        void SetDimensions(float width, float height);

        void SetAnimationFrames(int frameWidth, int frameHeight, int numFrames);
        void SetAnimationSpeed(float framesPerSecond);

        std::type_info const& GetComponentType() const override { return typeid(RenderComponent); }


    private:
        enum class RenderMode {
            Static,
            Animated
        };

        GameObject* m_GameObject;

        RenderMode m_renderMode;

        // For static images
        std::unique_ptr<Texture2D> m_texture;
        float m_width;
        float m_height;

        // For animations
        std::vector<std::unique_ptr<Texture2D>> m_animationFrames;
        float m_animationSpeed;
        int m_currentFrame;

        float m_animationTimer = 0.0f;
        float m_frameDuration = 0.1f;
    };
}