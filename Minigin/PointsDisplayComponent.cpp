#include "PointsDisplayComponent.h"

#include "GameObject.h"


namespace dae
{
    PointsDisplayComponent::PointsDisplayComponent(Font* font, GameObject& gameObject)
	    : m_pointComponent(nullptr), m_previousScore(0), m_ownerObject(gameObject)
    {
            m_textComponent = std::make_unique<TextComponent>("ss ", font, SDL_Color{ 255, 255, 255, 255 }, m_ownerObject);
            m_textComponent->SetText(std::to_string(m_previousScore));
    }

    PointsDisplayComponent::~PointsDisplayComponent()
    {
        CleanUp();
    }

    void PointsDisplayComponent::Update()
    {
        if (m_textComponent)
        {
            m_textComponent->Update();
        }
    }

    void PointsDisplayComponent::OnNotify()
    {
        if (m_pointComponent)
        {
            const auto currentScore = m_pointComponent->GetScore();
            if (currentScore != m_previousScore)
            {
                m_previousScore = currentScore;
                if (m_textComponent)
                {
                    m_textComponent->SetText(std::to_string(currentScore));
                }
            }
        }
    }

    void PointsDisplayComponent::Render() const
    {
        if (m_textComponent)
        {
            m_textComponent->Render();
        }
    }

    void PointsDisplayComponent::CleanUp()
    {
        if (m_pointComponent)
        {
           // m_pointComponent->Detach(this); 
            m_pointComponent = nullptr;
        }
    }

    void PointsDisplayComponent::SetPointComponent(PointComponent* pointComponent)
    {
        if (m_pointComponent)
        {
            m_pointComponent->Detach(this);
        }

        m_pointComponent = pointComponent;

        if (m_pointComponent)
        {
            m_pointComponent->Attach(this);
            OnNotify();
        }
    }
}
