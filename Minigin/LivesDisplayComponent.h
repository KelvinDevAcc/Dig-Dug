#pragma once

#include <memory>
#include "Component.h"
#include "HealthComponent.h"
#include "Observer.h"
#include "TextComponent.h"


namespace dae
{
    class LivesDisplayComponent final : public Component, public Observer
    {
    public:
        LivesDisplayComponent(Font* font, GameObject& gameObject);
        ~LivesDisplayComponent() override = default;

        LivesDisplayComponent(const LivesDisplayComponent& other) = delete;
        LivesDisplayComponent(LivesDisplayComponent&& other) noexcept = delete;
        LivesDisplayComponent& operator=(const LivesDisplayComponent& other) = delete;
        LivesDisplayComponent& operator=(LivesDisplayComponent&& other) noexcept = delete;

        void Render() const override;
        void Update() override;

        void AttachToHealthComponent(HealthComponent* healthComponent);

        std::type_info const& GetComponentType() const override { return typeid(LivesDisplayComponent); }


    private:
        std::unique_ptr<TextComponent> m_textComponent;
        HealthComponent* m_healthComponent;
    };
}
