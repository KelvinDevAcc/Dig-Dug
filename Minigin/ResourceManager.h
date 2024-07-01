#pragma once
#include <string>
#include <memory>
#include "Font.h"
#include "Singleton.h"
#include "Sprite.h"
#include "Texture2D.h"

namespace dae
{
	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		static void Init(const std::string& data);

		static Font* LoadFont(const std::string& name, const std::string& file, unsigned int size);

		static Texture2D* LoadTexture(const std::string& fileName);
		static Sprite* LoadSprite(const std::string& name, const std::string& fileName, int rowCount = 1, int colCount = 1,const std::map<std::string, SpriteAnimation>& animations = {});

		static Sprite* GetSprite(const std::string& name);
		static Font* GetFont(const std::string& name);

	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;
		static std::string m_dataPath;

		inline static std::map<std::string, std::unique_ptr<Font>> m_FontMap{};
		inline static std::map<std::string, std::unique_ptr<Sprite>> m_SpriteMap{};
		inline static std::vector<std::unique_ptr<Texture2D>> m_LoadedTextures{};
	};
}
