#include "PointsDisplayComponent.h"


namespace dae
{
    PointsDisplayComponent::PointsDisplayComponent(Font* font, GameObject& gameObject, PointComponent* pointComponent)
	    : m_pointComponent(pointComponent), m_previousScore(0), m_GameObject(gameObject)
    {
	    m_textComponent = std::make_unique<TextComponent>("ss ", font, SDL_Color{255, 255, 255, 255}, gameObject);
        m_textComponent->SetText(std::to_string(m_previousScore));
    }

    void PointsDisplayComponent::Update()
    {
        if (m_pointComponent)
        {
	        const auto currentScore = m_pointComponent->GetScore();
            if (currentScore != m_previousScore)
            {
                m_previousScore = currentScore;
                m_textComponent->SetText(std::to_string(currentScore));
            }
            m_textComponent->Update();
        }
    }

    void PointsDisplayComponent::Render() const
    {
        m_textComponent->Render();
    }
}
