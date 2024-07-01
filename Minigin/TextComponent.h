#pragma once
#include <string>
#include <memory>
#include "SDL_pixels.h"

#include "Component.h"
#include "Font.h"
#include "Texture2D.h"

namespace dae
{
    class TextComponent final : public Component
    {
    public:
        void Update() override;
        void Render() const override;

        void SetText(const std::string& text);
        void SetColor(const SDL_Color& color);

        TextComponent(std::string text, Font* font, const SDL_Color& color, GameObject& gameObject);
        ~TextComponent() override = default;
        TextComponent(const TextComponent& other) = delete;
        TextComponent(TextComponent&& other) = delete;
        TextComponent& operator=(const TextComponent& other) = delete;
        TextComponent& operator=(TextComponent&& other) = delete;

        std::type_info const& GetComponentType() const override { return typeid(TextComponent); }

    private:
        bool m_NeedsUpdate{};
        SDL_Color m_TextColor{ 255,255,255,255 };
        std::string m_Text{};
        Font* m_Font{ nullptr };
        std::unique_ptr<Texture2D> m_TextTexture{};
        GameObject& m_GameObject;
    };
}
