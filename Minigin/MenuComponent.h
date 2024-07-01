#pragma once
#include <vector>
#include <functional>
#include <string>
#include <memory>
#include "GameObject.h"
#include "TextComponent.h"
#include "Font.h"

namespace dae
{
    class MenuComponent final : public Component
    {
    public:
        MenuComponent(GameObject* owner, const std::vector<std::string>& options, const std::vector<std::function<void()>>& callbacks, Font* font, float textspace);
        ~MenuComponent() override = default; 

        MenuComponent(const MenuComponent& other) = default;
        MenuComponent& operator=(const MenuComponent& other) = default;
        MenuComponent(MenuComponent&& other) noexcept = default;
        MenuComponent& operator=(MenuComponent&& other) noexcept = default;

        void Render() const override;
        void Update()override;

        void NavigateUp();
        void NavigateDown();
        void SelectOption();

       void SetTextColor(const SDL_Color& color);
       void SeSelectColor(const SDL_Color& color);

        std::type_info const& GetComponentType() const override { return typeid(MenuComponent); }

    private:
        std::vector<std::string> m_Options;
        std::vector<std::function<void()>> m_Callbacks;
        std::vector<std::unique_ptr<GameObject>> m_OptionObjects;
        GameObject* m_owner;
        float m_TextSpace;
        SDL_Color m_NormalColor{ 255, 255, 255, 255 };
        SDL_Color m_SelectedColor{ 255, 0, 0, 255 };
        size_t m_SelectedOption{ 0 };
    };
}
