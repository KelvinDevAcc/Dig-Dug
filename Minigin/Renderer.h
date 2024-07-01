#pragma once
#include <SDL.h>
#include "RenderComponent.h"
#include "Singleton.h"

namespace dae
{
	class Texture2D;

	class Renderer final : public Singleton<Renderer>
	{
		SDL_Renderer* m_renderer{};
		SDL_Window* m_window{};
		SDL_Color m_clearColor{};	
	public:
		void Init(SDL_Window* window);
		void Render() const;
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;
		void RenderTexture(const Texture2D& texture, float pos_x, float y, float width, float height, float rotation_angle) const;
		void RenderTexture(const Texture2D& texture, float x, float y, const glm::vec2& cellSize, int pixelsPerUnit) const;
		static void RenderTexture(const Texture2D& texture, glm::vec2 drawLocation, glm::vec2 srcLocation, glm::ivec2 cellSize, float width, float height, bool flipX, bool flipY);
		void RenderRect(const SDL_Rect& rect, SDL_Color color, bool filled) const;

		void RenderLine(float x1, float y1, float x2, float y2, const SDL_Color& color) const
		{
			SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
			SDL_RenderDrawLine(m_renderer, static_cast<int>(x1), static_cast<int>(y1), static_cast<int>(x2), static_cast<int>(y2));
		}

		SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }
	};
}

