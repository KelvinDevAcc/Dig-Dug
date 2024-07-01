#include "RenderComponent.h"
#include "GameObject.h"
#include "GameTime.h"
#include "Renderer.h"

namespace dae
{
    RenderComponent::RenderComponent(GameObject* gameObject)
	    : m_GameObject(gameObject), m_renderMode(RenderMode::Static), m_width(0), m_height(0), m_animationSpeed(0),
	      m_currentFrame(0)
    {
    }

    void  RenderComponent::Update()
    {
        if (m_renderMode == RenderMode::Animated) {
            m_animationTimer += GameTime::GetDeltaTime();
            if (m_animationTimer >= m_frameDuration) {
                m_currentFrame = (m_currentFrame + 1) % static_cast<int>(m_animationFrames.size());
                m_animationTimer -= m_frameDuration;
            }
        }
    }


    void RenderComponent::Render() const
    {
        if (m_texture)
        {
            if (m_renderMode == RenderMode::Static && m_texture) {
                if (m_GameObject) {
                    const auto& pos = m_GameObject->GetWorldPosition();
                    const float posX = pos.x - m_width / 2.0f;
                    const float posY = pos.y - m_height / 2.0f;
                    Renderer::GetInstance().RenderTexture(*m_texture, posX, posY, m_width, m_height);
                }
            }
            else if (m_renderMode == RenderMode::Animated && !m_animationFrames.empty()) {
                if (m_GameObject) {
                    const auto& pos = m_GameObject->GetWorldPosition();
                    const float posX = pos.x - m_width / 2.0f;
                    const float posY = pos.y - m_height / 2.0f;
                    Renderer::GetInstance().RenderTexture(*m_animationFrames[m_currentFrame], posX, posY, m_width, m_height);
                }
            }
        }
    }

    void RenderComponent::SetTexture(std::unique_ptr<Texture2D> texture)
    {
        m_texture = std::move(texture);
        if (m_texture)
        {
            // Retrieve the width and height of the texture
            const auto size = m_texture->GetSize();
            m_width = static_cast<float>(size.x);
            m_height = static_cast<float>(size.y);
        }
        m_renderMode = RenderMode::Static;
    }

    void RenderComponent::SetDimensions(float width, float height)
    {
        m_width = width;
        m_height = height;
    }

    void RenderComponent::SetAnimationFrames(int frameWidth, int frameHeight, int numFrames) {
        m_animationFrames.clear();
        m_animationFrames.reserve(numFrames);

        if (m_texture) {
            for (int i = 0; i < numFrames; ++i) {
                // Calculate source rectangle for each frame
                SDL_Rect srcRect;
                srcRect.x = i * frameWidth; 
                srcRect.y = 0; 
                srcRect.w = frameWidth;
                srcRect.h = frameHeight;
                
                auto frameTexture = std::make_unique<Texture2D>(m_texture->GetSDLTexture(), srcRect);
                m_animationFrames.push_back(std::move(frameTexture));
            }
            m_renderMode = RenderMode::Animated;
            m_currentFrame = 0;
        }
    }

    void RenderComponent::SetAnimationSpeed(float framesPerSecond)
    {
        m_animationSpeed = 1.0f / framesPerSecond;
    }

}