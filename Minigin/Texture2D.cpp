#include <SDL.h>
#include "Texture2D.h"

#include "Renderer.h"

dae::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_texture);
}

glm::ivec2 dae::Texture2D::GetSize() const
{
	SDL_Rect dst;
	SDL_QueryTexture(GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	return { dst.w,dst.h };
}

SDL_Texture* dae::Texture2D::GetSDLTexture() const
{
	return m_texture;
}

dae::Texture2D::Texture2D(SDL_Texture* texture): m_width(0), m_height(0), m_sourceRect()
{
	m_texture = texture;
}


dae::Texture2D::Texture2D(SDL_Texture* texture, const SDL_Rect& sourceRect): m_width(0), m_height(0), m_sourceRect(sourceRect)
{
	if (SDL_Surface* surface = SDL_CreateRGBSurface(0, sourceRect.w, sourceRect.h, 32, 0, 0, 0, 0))
	{
		if (SDL_Texture* newTexture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surface))
		{
			SDL_RenderCopy(Renderer::GetInstance().GetSDLRenderer(), texture, &sourceRect, nullptr);
			m_texture = newTexture;
		}
		SDL_FreeSurface(surface);
	}
}
