#include "LivesDisplayComponent.h"

#include "GameObject.h"
#include "ResourceManager.h"
#include "SpriteRendererComponent.h"
#include "Texture2D.h"

namespace dae
{
    LivesDisplayComponent::LivesDisplayComponent(GameObject& gameObject, const std::string& spritePath, int spriteWidth, int spriteHeight)
        : m_healthComponent(nullptr), m_spritePath(spritePath), m_spriteWidth(spriteWidth), m_spriteHeight(spriteHeight), m_gameObject(gameObject)
    {
    }

    void LivesDisplayComponent::Update()
    {
        if (m_healthComponent)
        {
            UpdateLivesSprites();
        }
    }

    void LivesDisplayComponent::Render() const
    {
        for (const auto& sprite : m_LivesSprites)
        {
            sprite.get()->Render();
        }
    }

    void LivesDisplayComponent::AttachToHealthComponent(HealthComponent* healthComponent)
    {
        m_healthComponent = healthComponent;
        UpdateLivesSprites();
    }

    void LivesDisplayComponent::UpdateLivesSprites()
    {
        const int lives = m_healthComponent->GetLives();
        const int currentSpriteCount = static_cast<int>(m_LivesSprites.size());

        if (currentSpriteCount < lives)
        {
            // Add sprites
            for (int i = currentSpriteCount; i < lives; ++i)
            {
                auto lifeSprite = std::make_unique<GameObject>();
                auto spriteComponent = std::make_unique<SpriteRendererComponent>(*lifeSprite);
                lifeSprite->AddComponent(std::move(spriteComponent));
                lifeSprite->SetLocalPosition(glm::vec3(i * (m_spriteWidth + 5), 0, 0)); // Adjust position for each sprite
                m_LivesSprites.push_back(std::move(lifeSprite));
            }
        }
        else if (currentSpriteCount > lives)
        {
            // Remove sprites
            m_LivesSprites.resize(lives);
        }
    }
}
