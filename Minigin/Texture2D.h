#pragma once
#include <SDL_rect.h>
#include <glm/vec2.hpp>

struct SDL_Texture;
namespace dae
{
	class Texture2D final
	{
	public:
		SDL_Texture* GetSDLTexture() const;
		explicit Texture2D(SDL_Texture* texture);
		Texture2D(SDL_Texture* texture, const SDL_Rect& sourceRect);
		~Texture2D();

		glm::ivec2 GetSize() const;

		Texture2D(const Texture2D &) = delete;
		Texture2D(Texture2D &&) = delete;
		Texture2D & operator= (const Texture2D &) = delete;
		Texture2D & operator= (const Texture2D &&) = delete;
	private:
		SDL_Texture* m_texture;
		int m_width;
		int m_height;
		SDL_Rect m_sourceRect; 
	};
}
