#pragma once
#include <memory>
#include "Component.h"
#include "Observer.h"
#include "PointComponent.h"
#include "TextComponent.h"


namespace dae
{
    class PointsDisplayComponent final: public Component, public Observer
    {
    public:
        PointsDisplayComponent(Font* font, GameObject& gameObject);
        ~PointsDisplayComponent() override;

        PointsDisplayComponent(const PointsDisplayComponent& other) = delete;
        PointsDisplayComponent(PointsDisplayComponent&& other) noexcept = delete;
        PointsDisplayComponent& operator=(const PointsDisplayComponent& other) = delete;
        PointsDisplayComponent& operator=(PointsDisplayComponent&& other) noexcept = delete;

        void Render() const override;
        void Update() override;
        void OnNotify() override;

        void SetPointComponent(PointComponent* pointComponent);

        std::type_info const& GetComponentType() const override { return typeid(PointsDisplayComponent); }

    private:
        void CleanUp();

        PointComponent* m_pointComponent;
        std::unique_ptr<TextComponent> m_textComponent;
        int m_previousScore;
        GameObject& m_ownerObject;
    };
}
