#include "Pepper.h"
#include "GameObject.h"
#include "GameTime.h"
#include "SceneData.h"
#include <iostream>

#include "ResourceManager.h"

namespace game
{
    Pepper::Pepper(dae::GameObject* owner, const glm::vec3& direction, float speed, float lifetime)
        : m_direction(direction), m_speed(speed), m_lifetime(lifetime), m_isActive(false), m_hitBox(nullptr),
        m_animationComponent(nullptr), m_owner(owner)
    {
        auto peperObject = std::make_unique<dae::GameObject>();
        auto spriteRenderComponent = std::make_unique<dae::SpriteRendererComponent>(peperObject.get(), dae::ResourceManager::GetSprite("Pepper"));
        spriteRenderComponent->SetDimensions(40, 40);
        peperObject->AddComponent(std::move(spriteRenderComponent));

        auto animationComponent = std::make_unique<dae::AnimationComponent>(peperObject.get(), peperObject->GetComponent<dae::SpriteRendererComponent>(), "ShakeCloud");
        m_animationComponent = animationComponent.get();
        peperObject->AddComponent(std::move(animationComponent));


        auto  hitBox = std::make_unique<HitBox>(glm::vec2(m_owner->GetDimensions().first, m_owner->GetDimensions().second));
        peperObject->AddComponent(std::move(hitBox));
        peperObject->SetLocalPosition(glm::vec3(10, 0, 0.f));

        peperObject->SetParent(m_owner);
        m_perperObject = std::move(peperObject);
    }

    void Pepper::Update()
    {
        if (!m_isActive)
            return;

        if (!m_hitBox)
        {
            m_hitBox = m_perperObject->GetComponent<HitBox>();
            if (!m_hitBox)
            {
                std::cerr << "Error: Pepper Attack HitBox is nullptr\n";
                return;
            }
        }

        m_perperObject->Update();

        m_lifetime -= dae::GameTime::GetDeltaTime();
        if (m_lifetime <= 0.0f)
        {
            Deactivate();
        }
    }

    void Pepper::Render() const
    {
        if (!m_isActive)
            return;

        m_perperObject->Render();
    }

    void Pepper::Activate()
    {
        m_isActive = true;
        m_lifetime = 1.0f;
        m_animationComponent->Play("ShakeCloud2", true);
    }

    void Pepper::Deactivate()
    {
        m_isActive = false;
    }
}
  