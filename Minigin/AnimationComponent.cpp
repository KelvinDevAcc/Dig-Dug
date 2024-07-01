#include "AnimationComponent.h"
#include "GameTime.h"

namespace dae
{
    AnimationComponent::AnimationComponent(GameObject* gameObject, SpriteRendererComponent* spriteRenderer, const std::string& defaultAnimation)
        : m_gameObject(gameObject), m_spriteRenderer(spriteRenderer), m_activeAnimation(nullptr),
        m_activeAnimationName(defaultAnimation), m_playingAnimation(false), m_LoopingAnimation(false),
        m_frameTime(0.0f), m_renderScaleX(1.0f), m_renderScaleY(1.0f)
    {
        m_activeAnimation = m_spriteRenderer->GetSprite()->GetAnimation(defaultAnimation);
    }

    void AnimationComponent::Update()
    {
        if (!m_playingAnimation || m_activeAnimation == nullptr)
            return;

        const float animationDuration = 1.0f / m_activeAnimation->framesPerSecond;
        const float normalizedDeltaTime = GameTime::GetDeltaTime() / animationDuration;

        m_frameTime += normalizedDeltaTime;

        if (m_LoopingAnimation)
        {
            // If looping, wrap the frame time within the animation duration
            m_frameTime = std::fmod(m_frameTime, 1.0f);
        }
        else
        {
            if (m_frameTime >= 1.0f)
            {
                m_playingAnimation = false;
                m_frameTime = 1.0f;
            }
        }

        m_spriteRenderer->SetDrawCell(m_activeAnimation->GetCellFromNormalizedTime(m_frameTime));
    }

    void AnimationComponent::Play(const std::string& name, bool looping, float startFrameTime)
    {

        if (auto* animation = m_spriteRenderer->GetSprite()->GetAnimation(name))
        {
           
	        if (m_activeAnimation == animation)
                return;

            m_playingAnimation = true;
            m_LoopingAnimation = looping;
            m_frameTime = startFrameTime;
            m_activeAnimationName = name;
            m_activeAnimation = animation;

        }
    }


    void AnimationComponent::Stop()
    {
        m_playingAnimation = false;
    }

    void AnimationComponent::SetRenderScale(float scaleX, float scaleY)
    {
        m_renderScaleX = scaleX;
        m_renderScaleY = scaleY;
    }

    void AnimationComponent::FlipSprite(bool flipHorizontally, bool flipVertically) const
    {
        m_spriteRenderer->Setflip(flipHorizontally, flipVertically);
    }
}