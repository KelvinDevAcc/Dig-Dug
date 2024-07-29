#include "TextComponent.h"

#include <algorithm>
#include <stdexcept>

#include "Font.h"
#include "GameObject.h"
#include "Renderer.h"
#include "SDL_ttf.h"
#include "Texture2D.h"

dae::TextComponent::TextComponent(std::string text, dae::Font* font, const SDL_Color& color, dae::GameObject& gameObject)
	: m_NeedsUpdate(true)
	  , m_TextColor(color)
	  , m_Text(std::move(text))
	  , m_Font(font)
	  , m_GameObject(gameObject), m_RenderOrder(0)
{
}

void dae::TextComponent::Update()
{
    if (m_NeedsUpdate)
    {
        SDL_Surface* surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), m_TextColor);
        if (!surf)
        {
            throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
        SDL_FreeSurface(surf);

        if (!texture)
        {
            throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
        }

        m_TextTexture = std::make_unique<dae::Texture2D>(texture);
        m_NeedsUpdate = false;
    }
}

void dae::TextComponent::Render() const
{
    if (m_TextTexture)
    {
        const auto& pos = m_GameObject.GetWorldPosition();
        const float textureWidth = static_cast<float>(m_TextTexture->GetSize().x);
        const float posX = pos.x - textureWidth / 2.0f;
        dae::Renderer::GetInstance().RenderTexture(*m_TextTexture, posX, pos.y);
    }
}

void dae::TextComponent::SetText(const std::string& text)
{
    if (text != m_Text)
    {
        m_Text = text;
        m_NeedsUpdate = true;
    }
}

void dae::TextComponent::SetColor(const SDL_Color& color)
{
    if (color.r != m_TextColor.r || color.g != m_TextColor.g || color.b != m_TextColor.b || color.a != m_TextColor.a)
    {
        m_TextColor = color;
        m_NeedsUpdate = true;
    }
}


