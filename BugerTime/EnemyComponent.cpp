#include "EnemyComponent.h"
#include "GameTime.h"
#include "MrHotDogAIComponent.h"
#include "MrPickleAIComponent.h"
#include "Scene.h"
#include "SceneData.h"


namespace game {

    EnemyComponent::EnemyComponent(dae::GameObject* owner, dae::Scene* scene)
	    : m_spriteRenderer(nullptr), m_animationComponent(nullptr), m_owner(owner), m_scene(scene), m_deathTimer(0),
	      m_direction(glm::vec3(0.f,0.f,0.f))
    {
	    m_spriteRenderer = owner->GetComponent<dae::SpriteRendererComponent>();
	    m_animationComponent = owner->GetComponent<dae::AnimationComponent>();

    }

    void EnemyComponent::Update() {
        if (!m_spriteRenderer || !m_animationComponent) {
            return;
        }

        if (m_owner->HasComponent<MrHotDogAIComponent>()) {
            m_direction = m_owner->GetComponent<MrHotDogAIComponent>()->GetMoveDirection();
        }
        if (m_owner->HasComponent<MrPickleAIComponent>()) {
            m_direction = m_owner->GetComponent<MrPickleAIComponent>()->GetMoveDirection();
        }

        UpdateAnimation();

        if (m_deathTimer > 0.0f) {
            m_deathTimer -= dae::GameTime::GetDeltaTime();

            // Check if the delay has elapsed
            if (m_deathTimer <= 0.0f) {
                // Remove the enemy from the scene
                m_scene->Remove(m_owner);
            }
        }
    }


    void EnemyComponent::UpdateAnimation() const
    {
        // Update animation based on movement direction
        if (glm::length(m_direction) > 0.0f) {
            if (m_direction.x > 0.0f) {
                m_animationComponent->Play("Walk_Right", true);
                m_animationComponent->FlipSprite(true, false);
            }
            else if (m_direction.x < 0.0f) {
                m_animationComponent->Play("Walk_Left", true);
                m_animationComponent->FlipSprite(false, false);
            }
            else if (m_direction.y > 0.0f) {
                m_animationComponent->Play("Walk_Up", true);
            }
            else if (m_direction.y < 0.0f) {
                m_animationComponent->Play("Walk_Down", true);
            }
        }
        else {
            // If not moving, play idle animation
            m_animationComponent->Play("Idle", true);
        }
    }

void EnemyComponent::Die()
{
    // Play death animation
    m_animationComponent->Play("Dying");

    // Set a delay before removing the enemy
    const float totalTime = 2.f;

    m_deathTimer = totalTime;
}

}
