#include "Pump.h"

#include "EnemyComponent.h"
#include "GameTime.h"
#include "EnemyComponent.h"
#include "EnemyPlayer.h"
#include "FygarComponent.h"
#include "ResourceManager.h"
#include "SceneData.h"

Pump::Pump(dae::GameObject* owner, const glm::vec3& localPosition, const glm::vec3& direction, float lifetime)
    : m_owner(owner), m_LocalPosition(localPosition), m_direction(direction), m_speed(0), m_lifetime(lifetime),
    m_isActive(false), m_isPumping(false), m_connectedEnemy(nullptr), m_spriteRenderComponent(nullptr),
    m_hitBox(nullptr)
{

    float forwardOffset = 35.0f; // Adjust this value to set how far forward the pump should appear

    // Apply the forward offset based on the direction
    m_LocalPosition += forwardOffset * glm::normalize(direction);

    // Create the pump object
    auto PumpObject = std::make_unique<dae::GameObject>();
    PumpObject->SetParent(m_owner);
    PumpObject->SetLocalPosition(m_LocalPosition);
    PumpObject->SetRotation(GetRotationFromDirection(m_direction));

    // Create and add sprite render component
    auto texture = dae::ResourceManager::GetTexture(dae::HashString("PumpEnd"));
    auto spriteRenderComponent = std::make_unique<dae::SpriteRendererComponent>(PumpObject.get(), texture);
    spriteRenderComponent->SetDimensions(32, 16);
    spriteRenderComponent->SetFlip(true, false);
    m_spriteRenderComponent = spriteRenderComponent.get();
    PumpObject->AddComponent(std::move(spriteRenderComponent));

    // Create and add hitbox component
    glm::vec2 hitBoxSize = (m_direction.y == 1 || m_direction.y == -1) ? glm::vec2(16, 32) : glm::vec2(32, 16);
    auto hitBox = std::make_unique<HitBox>(hitBoxSize);
    hitBox->SetGameObject(PumpObject.get());  // Correctly set the GameObject for the hitbox
    m_hitBox = hitBox.get();
    PumpObject->AddComponent(std::move(hitBox));

    // Store the fully initialized PumpObject
    m_pumpObject = std::move(PumpObject);
}

float Pump::GetRotationFromDirection(const glm::vec3& direction)
{
    if (direction.x == 1)
        return 0.0f;
    if (direction.x == -1)
        return 180.0f;
    if (direction.y == 1)
        return 90.0f;
    if (direction.y == -1)
        return 270.0f;
    return 0.0f; // Default rotation if no direction matches
}

void Pump::Update()
{
    if (!m_isActive) return;

    if (m_isPumping)
    {
        PumpEnemy();
    }
    else
    {
	    if (IsCollidingWithEnemy())
	    {
            m_isPumping = true;
	    }

        m_lifetime -= dae::GameTime::GetDeltaTime();
        if (m_lifetime <= 0)
        {
            Deactivate();
        }
    }
}

void Pump::PumpEnemy()
{
    if (!m_connectedEnemy) return;

    if (m_connectedEnemy->GetComponent<EnemyComponent>())
    {
        m_connectedEnemy->GetComponent<EnemyComponent>()->HitByPump(m_owner);
    }
    else if (m_connectedEnemy->GetComponent<FygarComponent>())
    {
        m_connectedEnemy->GetComponent<FygarComponent>()->HitByPump(m_owner);
    }
    else if (m_connectedEnemy->GetComponent<game::EnemyPlayer>())
    {
        m_connectedEnemy->GetComponent<game::EnemyPlayer>()->HitByPump();
    }

    // Optionally keep the pump active or handle state if needed
    m_connectedEnemy = nullptr;
}

void Pump::Render() const
{
    if (!m_isActive) return;

    // Render pump object
    m_pumpObject->Render();
}

void Pump::Activate()
{
    m_isActive = true;
    m_isPumping = false;
    m_connectedEnemy = nullptr;
}

void Pump::Deactivate()
{
    m_isActive = false;
    m_isPumping = false;
    m_connectedEnemy = nullptr;
}


void Pump::SetTexture(const dae::Texture2D* texture) const
{
    m_spriteRenderComponent->SetTexture(texture);
}

void Pump::SetPosition(const glm::vec3& position)
{
    m_LocalPosition = position;
}

void Pump::SetDirection(const glm::vec3& direction)
{
    m_direction = direction;
}

void Pump::SetLifetime(float lifetime)
{
    m_lifetime = lifetime;
}

bool Pump::IsCollidingWithEnemy()
{
    if (dae::SceneData::GetInstance().isOnEnemy(*m_pumpObject))
    {
        const auto enemies = dae::SceneData::GetInstance().GetEnemies();

        for (auto& enemy : enemies)
        {
            if (enemy != m_owner && enemy->GetComponent<HitBox>()->IsColliding(*m_hitBox))
            {
                m_connectedEnemy = enemy;
                m_isPumping = true;
                return true;
            }
        }
    }
    return false;
}


