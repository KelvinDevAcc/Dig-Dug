#include <stdexcept>
#include "Renderer.h"

#include <chrono>

#include "SceneManager.h"
#include "Texture2D.h"

#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl2.h"

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void dae::Renderer::Init(SDL_Window* window)
{
	m_window = window;
	m_renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL3_Init();
}


void dae::Renderer::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);
	SceneManager::GetInstance().Render();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();


	// Render ImGui
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SDL_RenderPresent(m_renderer);
}

void dae::Renderer::Destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, float pos_x, float pos_y, float width, float height, float rotation_angle) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(pos_x);
	dst.y = static_cast<int>(pos_y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);

	// Render texture with rotation
	SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst, rotation_angle, nullptr, SDL_FLIP_NONE);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, float x, float y, const glm::vec2& cellSize,
	int pixelsPerUnit) const
{
	SDL_Rect destRect{
		static_cast<int>(x),
		static_cast<int>(y),
		static_cast<int>(cellSize.x * static_cast<float>(pixelsPerUnit)),
		static_cast<int>(cellSize.y * static_cast<float>(pixelsPerUnit))
	};

	SDL_RenderCopy(m_renderer, texture.GetSDLTexture(), nullptr, &destRect);
}

void dae::Renderer::RenderTexture(const dae::Texture2D& texture, glm::vec2 drawLocation,
	glm::vec2 srcLocation, glm::ivec2 cellSize, float width, float height, bool flipX, bool flipY)
{
	SDL_Renderer* sdlRenderer = GetInstance().GetSDLRenderer();

	if (sdlRenderer == nullptr || texture.GetSDLTexture() == nullptr)
		return; 

	SDL_Rect srcRect;
	srcRect.x = static_cast<int>(srcLocation.x * cellSize.x);
	srcRect.y = static_cast<int>(srcLocation.y * cellSize.y);
	srcRect.w = static_cast<int>(cellSize.x);
	srcRect.h = static_cast<int>(cellSize.y);

	SDL_Rect dstRect;
	dstRect.x = static_cast<int>(drawLocation.x);
	dstRect.y = static_cast<int>(drawLocation.y);
	dstRect.w = static_cast<int>(width);
	dstRect.h = static_cast<int>(height);

	SDL_RendererFlip flip = SDL_FLIP_NONE;
	if (flipX and flipY)
		flip = static_cast<SDL_RendererFlip>(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
	else if (flipX)
		flip = SDL_FLIP_HORIZONTAL;
	else if (flipY)
		flip = SDL_FLIP_VERTICAL;

	const SDL_Point center{};

	SDL_RenderCopyEx(sdlRenderer, texture.GetSDLTexture(), &srcRect, &dstRect, 0.0f, &center, flip);
}

void dae::Renderer::RenderRect(const SDL_Rect& rect, SDL_Color color, bool filled) const
{
	const SDL_Rect centeredRect = rect;

	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	if (filled) {
		SDL_RenderFillRect(m_renderer, &centeredRect);
	}
	else {
		SDL_RenderDrawRect(m_renderer, &centeredRect);
	}
}

SDL_Renderer* dae::Renderer::GetSDLRenderer() const { return m_renderer; }
