#include "PointsDisplayComponent.h"


namespace dae
{
    PointsDisplayComponent::PointsDisplayComponent(Font* font, GameObject& gameObject)
        : m_pointComponent(nullptr), m_textComponent(nullptr)
    {
        m_textComponent = std::make_unique<TextComponent>("ss ", font, SDL_Color{ 255, 255, 255, 255 }, gameObject); 
    }

    void PointsDisplayComponent::Update()
    {
        m_textComponent->Update();
        if (m_pointComponent)
        {
	        const std::string text = "Points: " + std::to_string(m_pointComponent->GetScore());
            m_textComponent->SetText(text);
        }
    }

    void PointsDisplayComponent::Render() const
    {
        m_textComponent->Render(); 
    }
   
    void PointsDisplayComponent::AttachToPointComponent(PointComponent* pointComponent)
    {
        m_pointComponent = pointComponent;
    }
    
}
