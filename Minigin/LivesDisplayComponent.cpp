#include "LivesDisplayComponent.h"

namespace dae
{
    LivesDisplayComponent::LivesDisplayComponent(Font* font, GameObject& gameObject)
        : m_textComponent(nullptr), m_healthComponent(nullptr)
    {
        m_textComponent = std::make_unique<TextComponent>("ss ", font,SDL_Color{ 255, 255, 255, 255 },gameObject);
    }

    void LivesDisplayComponent::Update()
    {
        m_textComponent->Update();

        if (m_healthComponent)
        {
	        const std::string text = "Lives: " + std::to_string(m_healthComponent->GetLives());
            m_textComponent->SetText(text);
        }
    }

    void LivesDisplayComponent::Render() const
    {
        m_textComponent->Render(); 
    }

    void LivesDisplayComponent::AttachToHealthComponent(HealthComponent* healthComponent)
    {
        m_healthComponent = healthComponent;
    }
}
