#include "MenuComponent.h"
#include <iostream>
#include "ResourceManager.h"

namespace dae
{
    MenuComponent::MenuComponent(GameObject* owner, const std::vector<std::string>& options, const std::vector<std::function<void()>>& callbacks, Font* font, float textSpace)
        : m_Options(options), m_Callbacks(callbacks), m_owner(owner), m_TextSpace(textSpace)
    {
        float yPos = m_owner->GetWorldPosition().y;
        for (const auto& option : options)
        {
            auto optionObject = std::make_unique<GameObject>();
            auto textComponent = std::make_unique<TextComponent>(option, font, m_NormalColor, *optionObject);
            optionObject->AddComponent(std::move(textComponent));

            optionObject->SetLocalPosition(glm::vec3(m_owner->GetWorldPosition().x, yPos, 0.f));

            yPos += m_TextSpace;
            m_OptionObjects.push_back(std::move(optionObject));
        }
    }

    void MenuComponent::Render() const
    {
        for (size_t i = 0; i < m_OptionObjects.size(); ++i)
        {
            const auto textComponent = m_OptionObjects[i]->GetComponent<TextComponent>();
            if (textComponent == nullptr)
            {
                std::cerr << "Error: TextComponent is null\n";
                continue;
            }
            if (i == m_SelectedOption)
            {
                textComponent->SetColor(m_SelectedColor);
            }
            else
            {
                textComponent->SetColor(m_NormalColor);
            }
            textComponent->Render();
        }
    }

    void MenuComponent::Update()
    {
        for (auto& optionObject : m_OptionObjects)
        {
            if (optionObject)
            {
                optionObject->Update();
            }
            else
            {
                std::cerr << "Error: OptionObject is null\n";
            }
        }
    }

    void MenuComponent::SetTextColor(const SDL_Color& color)
    {
        for (const auto& optionObject : m_OptionObjects)
        {
	        if (const auto textComponent = optionObject->GetComponent<TextComponent>())
            {
                textComponent->SetColor(color);
                m_NormalColor = color;
                textComponent->Render();
            }
            else
            {
                std::cerr << "Error: TextComponent is null\n";
            }
        }
    }

    void MenuComponent::SeSelectColor(const SDL_Color& color)
    {
        m_SelectedColor = color;
    }



    void MenuComponent::NavigateUp()
    {
        if (m_SelectedOption > 0)
        {
            --m_SelectedOption;
        }
    }

    void MenuComponent::NavigateDown()
    {
        if (m_SelectedOption < m_Options.size() - 1)
        {
            ++m_SelectedOption;
        }
    }

    void MenuComponent::SelectOption()
    {
        if (m_SelectedOption < m_Callbacks.size())
        {
            m_Callbacks[m_SelectedOption]();
            m_SelectedOption = 0;
        }
    }
}
