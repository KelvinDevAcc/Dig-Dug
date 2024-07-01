#pragma once
#include "GameObject.h"
#include "SpriteRendererComponent.h"


namespace dae
{
    class AnimationComponent final : public Component
    {
    public:
        AnimationComponent(GameObject* gameObject, SpriteRendererComponent* spriteRenderer, const std::string& defaultAnimation);
        ~AnimationComponent() override = default;

        AnimationComponent(const AnimationComponent& other) = delete;
        AnimationComponent(AnimationComponent&& other) noexcept = delete;
        AnimationComponent& operator=(const AnimationComponent& other) = delete;
        AnimationComponent& operator=(AnimationComponent&& other) noexcept = delete;

        void Update() override;
        void Play(const std::string& name, bool looping = false, float startFrameTime = 0.0f);
        void Stop();
        void SetRenderScale(float scaleX, float scaleY);
        void FlipSprite(bool flipHorizontally, bool flipVertically) const;

        std::type_info const& GetComponentType() const override { return typeid(AnimationComponent); }

    private:
        GameObject* m_gameObject;
        SpriteRendererComponent* m_spriteRenderer;
        const SpriteAnimation* m_activeAnimation;
        std::string m_activeAnimationName;
        bool m_playingAnimation;
        bool m_LoopingAnimation;
        float m_frameTime;
        float m_renderScaleX;
        float m_renderScaleY;
    };
}
