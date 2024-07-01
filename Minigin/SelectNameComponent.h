#pragma once
#include <string>
#include <memory>
#include "TextComponent.h"
#include "GameObject.h"
#include "Font.h"
#include "SDL.h"

class SelectNameComponent : public dae::Component
{
public:
    SelectNameComponent(dae::GameObject* owner, size_t maxLength, dae::Font* font, const SDL_Color& color, int playerId);

    void Update() override;
    void Render() const override;
    void AddLetter();
    void SubtractLetter();
    void ConfirmLetter();

    int GetPlayerId() const;

    std::string GetCurrentName() const;
    char GetCurrentLetter() const;

    std::type_info const& GetComponentType() const override { return typeid(SelectNameComponent); }

private:
    int m_PlayerId;
    size_t m_MaxLength;
    char m_CurrentLetter;
    std::string m_CurrentName;

    static constexpr char FIRST_LETTER = 'A';
    static constexpr char LAST_LETTER = 'Z';

    SDL_Color m_Color;
    dae::GameObject* m_owner;
    std::unique_ptr<dae::GameObject> m_NameObject;
    std::unique_ptr<dae::GameObject> m_LetterObject;
    std::unique_ptr<dae::TextComponent> m_NameTextComponent;
    std::unique_ptr<dae::TextComponent> m_LetterTextComponent;
};
