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
        LivesDisplayComponent(GameObject& gameObject, const std::string& spritePath, int spriteWidth, int spriteHeight);

        void Update() override;
        void Render() const override;
        void AttachToHealthComponent(HealthComponent* healthComponent);

    private:
        void UpdateLivesSprites();

        std::vector<std::unique_ptr<GameObject>> m_LivesSprites;
        HealthComponent* m_healthComponent;
        std::string m_spritePath;
        int m_spriteWidth;
        int m_spriteHeight;
        GameObject& m_gameObject;
    };
}
