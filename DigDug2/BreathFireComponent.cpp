#include "BreathFireComponent.h"
#include "GameTime.h"
#include "ResourceManager.h"
#include "SceneData.h"

namespace game
{
    BreathFire::BreathFire(dae::GameObject* owner, const glm::vec3& direction, float speed, float lifetime)
        : m_direction(direction), m_speed(speed), m_lifetime(lifetime), m_isActive(false),
        m_owner(owner), m_spriterendererComponent(nullptr), m_hitBox(nullptr), m_stage(1)
    {
        // Create the BreathFire game object
        auto breathFireObject = std::make_unique<dae::GameObject>();
        breathFireObject->SetParent(m_owner);
        breathFireObject->SetLocalPosition(glm::vec3(0, 0, 0.f));

        // Set up the sprite renderer
        const unsigned int BreathFireStage1ID = dae::HashString("BreathFireStage1");
        auto spriteRenderComponent = std::make_unique<dae::SpriteRendererComponent>(
            breathFireObject.get(), dae::ResourceManager::GetTexture(BreathFireStage1ID)
        );
        spriteRenderComponent->SetDimensions(
            static_cast<float>(dae::ResourceManager::GetTexture(BreathFireStage1ID)->GetSize().x),
            static_cast<float>(dae::ResourceManager::GetTexture(BreathFireStage1ID)->GetSize().y)
        );
        m_spriterendererComponent = spriteRenderComponent.get(); // Store reference
        breathFireObject->AddComponent(std::move(spriteRenderComponent));

        // Set up the hitbox (optional size adjustment per stage)
        auto hitBox = std::make_unique<HitBox>(glm::vec2(m_spriterendererComponent->GetDimensions().x,
            m_spriterendererComponent->GetDimensions().y));
        m_hitBox = hitBox.get();
        hitBox->SetGameObject(breathFireObject.get());
        breathFireObject->AddComponent(std::move(hitBox));

        dae::SceneData::GetInstance().AddGameObject(breathFireObject.get(), dae::GameObjectType::fire);
        m_breathFireObject = std::move(breathFireObject);
    }

    BreathFire::~BreathFire()
    {
        // Safely remove the breathFireObject from the scene
        if (m_breathFireObject)
        {
            dae::SceneData::GetInstance().RemoveGameObject(m_breathFireObject.get(), dae::GameObjectType::fire);
        }
        m_breathFireObject.reset(); // Ensures the unique pointer releases ownership and destroys the object
    }

    void BreathFire::Update()
    {
        if (!m_isActive)
            return;

        m_lifetime -= dae::GameTime::GetDeltaTime();
        if (m_lifetime <= 0.0f)
        {
            Deactivate();
            return;
        }

        UpdateSizeAndAnimation(); // Update size and animation based on the current stage

        m_breathFireObject->Update();
    }

    void BreathFire::Render() const
    {
        if (m_isActive)
        {
            m_breathFireObject->Render();
        }
    }

    void BreathFire::Activate(const glm::vec3& direction)
    {
        m_isActive = true;
        m_lifetime = 3.0f; // Duration for the fire breath
        m_stage = 1; // Start from stage 1
        m_direction = direction; // Set the direction

        dae::Message message;
        message.type = dae::PlaySoundMessageType::Sound;
        message.arguments.emplace_back(static_cast<sound_id>(16));
        dae::EventQueue::Broadcast(message);
        UpdateSizeAndAnimation(); // Initial setup for stage 1
        m_hitBox->Enable();
        m_spriterendererComponent->SetTexture(dae::ResourceManager::GetTexture(dae::HashString("BreathFireStage1")));
        if (m_direction.x > 0)
            m_spriterendererComponent->SetFlip(false, false);
        else if (m_direction.x < 0) // Left
            m_spriterendererComponent->SetFlip(true, false);
        m_hitBox->SetSize(glm::vec2(static_cast<float>(dae::ResourceManager::GetTexture(dae::HashString("BreathFireStage1"))->GetSize().x),
            static_cast<float>(dae::ResourceManager::GetTexture(dae::HashString("BreathFireStage1"))->GetSize().y)));
        // Adjust position based on direction
        AdjustPosition();
    }


    void BreathFire::Deactivate()
    {
        m_isActive = false;
        m_stage = 1; // Reset stage on deactivation
        m_hitBox->Disable();
    }

    void BreathFire::AdjustPosition()
    {
        // Get the current sprite dimensions
        const auto& dimensionsowner = m_owner->GetComponent<dae::SpriteRendererComponent>()->GetDimensions();
        float spriteWidthowner = dimensionsowner.x;

        const auto& dimensions = m_spriterendererComponent->GetDimensions();
        float spriteWidth = dimensions.x;

        glm::vec3 offsetPosition(0.0f);
        offsetPosition += m_direction * glm::vec3(spriteWidth/2+spriteWidthowner/2, 0, 0);
        
        m_breathFireObject->SetLocalPosition(offsetPosition);
    }

    void BreathFire::UpdateSizeAndAnimation()
    {
        switch (m_stage)
        {
        case 1:
            if (m_lifetime < 2.0f)
            {
                m_stage = 2;
                const unsigned int BreathFireStage2ID = dae::HashString("BreathFireStage2");
                m_spriterendererComponent->SetTexture(dae::ResourceManager::GetTexture(BreathFireStage2ID));
                m_spriterendererComponent->SetDimensions(
                    static_cast<float>(dae::ResourceManager::GetTexture(BreathFireStage2ID)->GetSize().x),
                    static_cast<float>(dae::ResourceManager::GetTexture(BreathFireStage2ID)->GetSize().y)
                );
                if (m_direction.x > 0)
                    m_spriterendererComponent->SetFlip(false, false);
                else if (m_direction.x < 0) // Left
                    m_spriterendererComponent->SetFlip(true, false);
                m_hitBox->SetSize(glm::vec2(static_cast<float>(dae::ResourceManager::GetTexture(BreathFireStage2ID)->GetSize().x),
                    static_cast<float>(dae::ResourceManager::GetTexture(BreathFireStage2ID)->GetSize().y)));
                AdjustPosition();
            }
            break;
        case 2:
            if (m_lifetime < 1.0f)
            {
                m_stage = 3;
                const unsigned int BreathFireStage3ID = dae::HashString("BreathFireStage3");
                m_spriterendererComponent->SetTexture(dae::ResourceManager::GetTexture(BreathFireStage3ID));
                m_spriterendererComponent->SetDimensions(
                    static_cast<float>(dae::ResourceManager::GetTexture(BreathFireStage3ID)->GetSize().x),
                    static_cast<float>(dae::ResourceManager::GetTexture(BreathFireStage3ID)->GetSize().y)
                );
                if (m_direction.x > 0)
                    m_spriterendererComponent->SetFlip(false, false);
                else if (m_direction.x < 0) // Left
                    m_spriterendererComponent->SetFlip(true, false);

                m_hitBox->SetSize(glm::vec2(static_cast<float>(dae::ResourceManager::GetTexture(BreathFireStage3ID)->GetSize().x),
                    static_cast<float>(dae::ResourceManager::GetTexture(BreathFireStage3ID)->GetSize().y)));
                AdjustPosition();
            }
            break;
        default:
            break;
        }
    }
}
