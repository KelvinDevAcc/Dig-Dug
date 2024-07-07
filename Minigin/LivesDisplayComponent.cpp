#include "LivesDisplayComponent.h"

#include <iostream>
#include <utility>

#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "SpriteRendererComponent.h"

namespace dae
{
    LivesDisplayComponent::LivesDisplayComponent(GameObject& gameObject, int spriteWidth, int spriteHeight, std::string textureName)
	    :m_healthComponent(nullptr)
        , m_spriteWidth(spriteWidth)
        , m_spriteHeight(spriteHeight)
        , m_textureName(std::move(textureName))
		,m_gameObject(gameObject)
    {
        m_healthComponent = gameObject.GetComponent<HealthComponent>();
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
            if(!m_LivesSprites.empty())
            {
                sprite.get()->Render();
            }
        }
    }

    void LivesDisplayComponent::AttachToHealthComponent(HealthComponent* healthComponent)
    {
        m_healthComponent = healthComponent;
        UpdateLivesSprites();
    }

    void LivesDisplayComponent::UpdateLivesSprites()
    {
        if (!m_healthComponent)
        {
            std::cerr << "Error: HealthComponent is not attached." << std::endl;
            return;
        }

        const int lives = m_healthComponent->GetLives();
        const int currentSpriteCount = static_cast<int>(m_LivesSprites.size());

        if (currentSpriteCount < lives)
        {
            // Add sprites
            for (int i = currentSpriteCount; i < lives; ++i)
            {
                auto lifeSprite = std::make_unique<GameObject>();
                if (auto texture = ResourceManager::GetInstance().GetTexture(m_textureName))
                {
	                if (auto spriteComponent = std::make_unique<SpriteRendererComponent>(lifeSprite.get(), texture))
                    {
                        spriteComponent->SetDimensions(static_cast<float>(m_spriteWidth), static_cast<float>(m_spriteHeight));
                        lifeSprite->AddComponent(std::move(spriteComponent));
                        const auto position  = m_gameObject.GetWorldPosition();
                        lifeSprite->SetLocalPosition(glm::vec3(position.x + i * (m_spriteWidth + 5), position.y, 0)); // Adjust position for each sprite
                        m_LivesSprites.push_back(std::move(lifeSprite));
                    }
                    else
                    {
                        std::cerr << "Error: Failed to create SpriteRendererComponent." << std::endl;
                    }
                }
                else
                {
                    // Handle error: texture not loaded
                    std::cerr << "Error: Failed to load texture 'Lives.png'" << std::endl;
                }
            }
        }
        else if (currentSpriteCount > lives)
        {
            // Remove sprites
            m_LivesSprites.resize(lives);
        }
    }
}
