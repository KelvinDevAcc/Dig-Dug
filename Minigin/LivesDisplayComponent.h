#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Component.h"
#include "HealthComponent.h"


namespace dae
{
    class LivesDisplayComponent : public Component
    {
    public:
        LivesDisplayComponent(GameObject& gameObject, int spriteWidth, int spriteHeight, std::string textureName);

        void Update() override;
        void Render() const override;
        void AttachToHealthComponent(HealthComponent* healthComponent);

        std::type_info const& GetComponentType() const override { return typeid(LivesDisplayComponent); }
    private:
        void UpdateLivesSprites();

        std::vector<std::unique_ptr<GameObject>> m_LivesSprites;
        HealthComponent* m_healthComponent;
        int m_spriteWidth;
        int m_spriteHeight;
        std::string m_textureName;
        GameObject& m_gameObject;
    };
}
