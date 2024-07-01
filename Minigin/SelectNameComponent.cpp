#include "SelectNameComponent.h"
#include <iostream>

SelectNameComponent::SelectNameComponent(dae::GameObject* owner, size_t maxLength, dae::Font* font, const SDL_Color& color, int playerId)
	: m_PlayerId(playerId), m_MaxLength(maxLength), m_CurrentLetter('A'), m_Color(color), m_owner(owner)
{
	const float yPos = m_owner->GetWorldPosition().y;
	if (m_owner)
	{
		// Create GameObject for name
		m_NameObject = std::make_unique<dae::GameObject>();
		m_NameTextComponent = std::make_unique<dae::TextComponent>("", font, color, *m_NameObject);
		m_NameObject->SetLocalPosition(glm::vec3(m_owner->GetWorldPosition().x, yPos, 0.f));
		m_NameObject->AddComponent(std::move(m_NameTextComponent));

		// Create GameObject for current letter
		m_LetterObject = std::make_unique<dae::GameObject>();
		m_LetterTextComponent = std::make_unique<dae::TextComponent>(std::string(1, m_CurrentLetter), font, color,
		                                                             *m_LetterObject);
		m_LetterObject->SetLocalPosition(glm::vec3(m_owner->GetWorldPosition().x, yPos + 50, 0.f));
		m_LetterObject->AddComponent(std::move(m_LetterTextComponent));
	}
	else
	{
		std::cerr << "Error: Owner GameObject is null!" << std::endl;
	}
}

void SelectNameComponent::Update() {
    // Update text components with the current state
    if (const auto textComponent = m_NameObject->GetComponent<dae::TextComponent>()) {
        textComponent->SetText("NAME: " + m_CurrentName);
    }

    // Update only if the maximum length hasn't been reached
    if (m_CurrentName.size() < m_MaxLength) {
        if (const auto textComponent = m_LetterObject->GetComponent<dae::TextComponent>()) {
            textComponent->SetText(std::string(1, m_CurrentLetter));
            m_LetterObject->Update();
        }
    }
    else {
        // Hide the letter object when the maximum length is reached
    }

    m_NameObject->Update();
}
void SelectNameComponent::Render() const
{
    m_NameObject->Render();
    if (m_CurrentName.size() < m_MaxLength) {
        if (const auto textComponent = m_LetterObject->GetComponent<dae::TextComponent>()) {
            m_LetterObject->Render();
        }
    }
}

int SelectNameComponent::GetPlayerId() const {
    return m_PlayerId;
}

void SelectNameComponent::AddLetter() {
    if (m_CurrentLetter < 'Z') {
        ++m_CurrentLetter;
    }
    else {
        m_CurrentLetter = 'A';
    }
}

void SelectNameComponent::SubtractLetter() {
    if (m_CurrentLetter > 'A') {
        --m_CurrentLetter;
    }
    else {
        m_CurrentLetter = 'Z';
    }
}

void SelectNameComponent::ConfirmLetter() {
    if (m_CurrentName.size() < m_MaxLength) {
        m_CurrentName += m_CurrentLetter;
        m_CurrentLetter = 'A'; 
    }
}



std::string SelectNameComponent::GetCurrentName() const {
    return m_CurrentName;
}

char SelectNameComponent::GetCurrentLetter() const {
    return m_CurrentLetter;
}