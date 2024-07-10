#pragma once
#include <SDL_rect.h>
#include <string>
#include <glm/vec2.hpp>

struct SDL_Texture;
namespace dae
{
	class Texture2D final
	{
	public:
		explicit Texture2D(SDL_Texture* texture);
		Texture2D(SDL_Texture* texture, std::string fileName);
		Texture2D(SDL_Texture* texture, const SDL_Rect& sourceRect, std::string fileName);
		~Texture2D();
		
		Texture2D(const Texture2D&) = delete;
		Texture2D(Texture2D&&) = delete;
		Texture2D& operator= (const Texture2D&) = delete;
		Texture2D& operator= (const Texture2D&&) = delete;

		glm::ivec2 GetSize() const;
		const std::string& GetFileName() const { return m_fileName; }
		SDL_Texture* GetSDLTexture() const;

	private:
		SDL_Texture* m_texture;
		int m_width;
		int m_height;
		SDL_Rect m_sourceRect;
		std::string m_fileName;
	};
}
