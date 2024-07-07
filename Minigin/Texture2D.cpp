#include "Texture2D.h"
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
	: m_texture(nullptr), m_width(0), m_height(0), m_sourceRect(sourceRect), m_fileName(std::move(fileName))
{
	if (texture)
	{
		if (SDL_Surface* surface = SDL_CreateRGBSurface(0, sourceRect.w, sourceRect.h, 32, 0, 0, 0, 0))
		{
			if (SDL_Texture* newTexture = SDL_CreateTextureFromSurface(dae::Renderer::GetInstance().GetSDLRenderer(), surface))
			{
				SDL_RenderCopy(dae::Renderer::GetInstance().GetSDLRenderer(), texture, &sourceRect, nullptr);
				m_texture = newTexture;
				SDL_QueryTexture(m_texture, nullptr, nullptr, &m_width, &m_height);
			}
			SDL_FreeSurface(surface);
		}
	}
}

dae::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_texture);
}

glm::ivec2 dae::Texture2D::GetSize() const
{
	return { m_width, m_height };
}

SDL_Texture* dae::Texture2D::GetSDLTexture() const
{
	return m_texture;
}


