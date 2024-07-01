#pragma once
#include "Component.h"
#include "TextComponent.h"

namespace dae
{
    class FPSCounterComponent final : public Component
    {
    public:
        FPSCounterComponent(TextComponent* textComponent);
        ~FPSCounterComponent() override = default;

        FPSCounterComponent(const FPSCounterComponent& other) = delete;
        FPSCounterComponent(FPSCounterComponent&& other) noexcept = delete;
        FPSCounterComponent& operator=(const FPSCounterComponent& other) = delete;
        FPSCounterComponent& operator=(FPSCounterComponent&& other) noexcept = delete;

        void Update() override;
        float GetFPS() const;

        std::type_info const& GetComponentType() const override { return typeid(FPSCounterComponent); }

    private:
        int m_frameCount = 0;
        float m_elapsedTime = 0.0f;
        float m_fps = 0.0f;
        float m_deltaTime = 0.0f; 

        TextComponent* m_TextComponent;
    };
}
