#include "Texture2D.h"

#include <iostream>

#include "Renderer.h"

dae::Texture2D::Texture2D(SDL_Texture* texture)
    : m_texture(texture), m_width(0), m_height(0), m_sourceRect({ 0, 0, 0, 0 }), m_fileName("")
{
    if (m_texture)
    {
        SDL_QueryTexture(m_texture, nullptr, nullptr, &m_width, &m_height);
    }
}

dae::Texture2D::Texture2D(SDL_Texture* texture, std::string fileName)
    : m_texture(texture), m_width(0), m_height(0), m_sourceRect({ 0, 0, 0, 0 }), m_fileName(std::move(fileName))
{
    if (m_texture)
    {
        SDL_QueryTexture(m_texture, nullptr, nullptr, &m_width, &m_height);
    }
}

dae::Texture2D::Texture2D(SDL_Texture* texture, const SDL_Rect& sourceRect, std::string fileName)
    : m_texture(nullptr), m_width(sourceRect.w), m_height(sourceRect.h), m_sourceRect(sourceRect), m_fileName(std::move(fileName))
{
    if (texture)
    {
        // Create a new texture from the specified source rectangle
        if (SDL_Texture* newTexture = SDL_CreateTexture(Renderer::GetInstance().GetSDLRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, sourceRect.w, sourceRect.h))
        {
            SDL_SetRenderTarget(Renderer::GetInstance().GetSDLRenderer(), newTexture);
            SDL_RenderCopy(Renderer::GetInstance().GetSDLRenderer(), texture, &sourceRect, nullptr);
            SDL_SetRenderTarget(Renderer::GetInstance().GetSDLRenderer(), nullptr);
            SDL_SetTextureBlendMode(newTexture, SDL_BLENDMODE_BLEND); // Set blend mode for the new texture
            m_texture = newTexture;
        }
        else
        {
            std::cerr << "Error: Failed to create subtexture: " << SDL_GetError() << '\n';
        }
    }
}


dae::Texture2D::~Texture2D() {
    SDL_DestroyTexture(m_texture);
}

glm::ivec2 dae::Texture2D::GetSize() const {
    return { m_width, m_height };
}

SDL_Texture* dae::Texture2D::GetSDLTexture() const {
    return m_texture;
}


