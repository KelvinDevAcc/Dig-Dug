#pragma once
#include <string>
#include <memory>
#include <unordered_map>
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
        ~ResourceManager() override = default;

        static Font* LoadFont(unsigned int id, const std::string& file, unsigned int size);
        static Font* GetFont(unsigned int id);

        static Texture2D* LoadTexture(unsigned int id, const std::string& fileName);
        static Texture2D* LoadTexture(unsigned int id, const std::string& fileName, const SDL_Rect& sourceRect);
        static Texture2D* GetTexture(unsigned int id);

        static Sprite* LoadSprite(unsigned int id, const std::string& fileName, int rowCount = 1, int colCount = 1, const std::map<std::string, SpriteAnimation>& animations = {});
        static Sprite* GetSprite(unsigned int id);

    private:
        friend class Singleton<ResourceManager>;
        ResourceManager() = default;
        static std::string m_dataPath;

        // Hash maps for resource management
        inline static std::unordered_map<unsigned int, std::unique_ptr<Font>> m_FontMap{};
        inline static std::unordered_map<unsigned int, std::unique_ptr<Sprite>> m_SpriteMap{};
        inline static std::unordered_map<unsigned int, std::unique_ptr<Texture2D>> m_LoadedTextures{};
    };

    // Hash function to generate unique IDs for resource names
    inline unsigned int HashString(const std::string& str)
    {
        unsigned int hash = 5381;
        for (char c : str)
        {
            hash = ((hash << 5) + hash) + c; // hash * 33 + c
        }
        return hash;
    }
}
