#include "TextComponent.h"

#include <algorithm>
#include <stdexcept>

#include "Font.h"
#include "GameObject.h"
#include "Renderer.h"
#include "SDL_ttf.h"
#include "Texture2D.h"

dae::TextComponent::TextComponent(std::string text, dae::Font* font, const SDL_Color& color, dae::GameObject& gameObject)
    : m_NeedsUpdate(true), m_TextColor(color), m_Text(std::move(text)), m_Font(font), m_TextTexture(nullptr), m_GameObject(gameObject)
{
}

void dae::TextComponent::Update()
{
    if (m_NeedsUpdate)
    {
        const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), m_TextColor);
        if (surf == nullptr)
        {
            throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
        }
        auto* texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
        if (texture == nullptr)
        {
            throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
        }
        SDL_FreeSurface(surf);
        m_TextTexture = std::make_unique<Texture2D>(texture);
        m_NeedsUpdate = false;
    }
}

void dae::TextComponent::Render() const
{
	if (m_TextTexture != nullptr)
	{
        const auto& pos = m_GameObject.GetWorldPosition();
        const float textureWidth = static_cast<float>(m_TextTexture->GetSize().x);
        const float posX = pos.x - textureWidth / 2.0f;

        Renderer::GetInstance().RenderTexture(*m_TextTexture, posX, pos.y);
	}
}

void dae::TextComponent::SetText(const std::string& text)
{
   if (text != m_Text) {
        m_Text = text;
        m_NeedsUpdate = true;
   }
}

void dae::TextComponent::SetColor(const SDL_Color& color)
{
   if (color.r != m_TextColor.r || color.g != m_TextColor.g || color.b != m_TextColor.b || color.a != m_TextColor.a) {
        m_TextColor = color;
        m_NeedsUpdate = true;
    }
}


