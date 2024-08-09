#include "LivesDisplayComponent.h"

#include <iostream>
#include <utility>

#include "GameObject.h"
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
    }

    LivesDisplayComponent::~LivesDisplayComponent()
    {
        CleanUp();
    }

    void LivesDisplayComponent::Update()
    {
       
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

    void LivesDisplayComponent::OnNotify()
    {
        if (!m_healthComponent)
        {
            std::cerr << "Error: HealthComponent is not attached." << std::endl;
            return;
        }

        const int lives = m_healthComponent->GetLives();
        const int currentSpriteCount = static_cast<int>(m_LivesSprites.size());

        if (lives < 0)
        {
            return;
        }

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
                        const auto position = m_gameObject.GetWorldPosition();
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
                    std::cerr << "Error: Failed to load texture 'lives'" << std::endl;
                }
            }
        }
        else if (currentSpriteCount > lives)
        {
            // Remove sprites
            m_LivesSprites.resize(lives);
        }
    }

    void LivesDisplayComponent::SetHealthComponent(HealthComponent* healthComponent)
    {
        if (m_healthComponent)
        {
            m_healthComponent->Detach(this);
        }

        m_healthComponent = healthComponent;

        if (m_healthComponent)
        {
            m_healthComponent->Attach(this);
            OnNotify();
        }
    }

    void LivesDisplayComponent::CleanUp()
    {
        if (m_healthComponent)
        {
            //m_healthComponent->Detach(this); // Detach from the health component
            m_healthComponent = nullptr;
        }
        m_LivesSprites.clear();
    }
}
