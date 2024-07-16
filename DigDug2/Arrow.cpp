#include "Arrow.h"

#include <iostream>

#include "GameTime.h"
#include "ResourceManager.h"

Arrow::Arrow(dae::GameObject* owner, const glm::vec3& localPosition, const glm::vec3& direction, float lifetime)
	: m_owner(owner), m_direction(direction), m_LocalPosition(localPosition), m_speed(0), m_lifetime(lifetime), m_isActive(false),
	  m_isEnd(false),
	  m_spriteRenderComponent(nullptr), m_hitBox(nullptr)
{
	auto arrowObject = std::make_unique<dae::GameObject>();
	arrowObject->SetParent(m_owner);
	arrowObject->SetLocalPosition(m_LocalPosition);
    arrowObject->SetRotation(90.f);
	auto texture = dae::ResourceManager::GetTexture("ArrowEnd");
	auto spriteRenderComponent = std::make_unique<dae::SpriteRendererComponent>(arrowObject.get(), texture);
	spriteRenderComponent->SetDimensions(32, 16);
	m_spriteRenderComponent = spriteRenderComponent.get();
	m_spriteRenderComponent->SetFlip(true, false);
	arrowObject->AddComponent(std::move(spriteRenderComponent));

	auto hitBox = std::make_unique<HitBox>(glm::vec2(32, 16));
	arrowObject->AddComponent(std::move(hitBox));
	m_arrowObject = std::move(arrowObject);
}

void Arrow::Update()
{
    if (!m_isActive)
        return;

    if (!m_hitBox)
    {
        m_hitBox = m_arrowObject->GetComponent<HitBox>();
        if (!m_hitBox)
        {
            std::cerr << "Error: Attack HitBox is nullptr\n";
            return;
        }
    }

    m_lifetime -= dae::GameTime::GetDeltaTime();
    if (m_lifetime <= 0.0f)
    {
        Deactivate();
    }
}

void Arrow::Render() const
{
    if (!m_isActive)
        return;

    m_arrowObject->Render();
}

void Arrow::Activate()
{
    m_isActive = true;
}

void Arrow::Deactivate()
{
    m_isActive = false;
}

void Arrow::SetTexture(const dae::Texture2D* texture) const
{
    m_spriteRenderComponent->SetTexture(texture);
}


