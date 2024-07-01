#include "RotatorComponent.h"
#include <cmath>
#include <SDL_stdinc.h>

#include "GameTime.h"


dae::RotatorComponent::RotatorComponent(GameObject* gameObject, float rotationSpeed, float centerX, float centerY, float radius)
    : m_RotationSpeed(rotationSpeed), m_CurrentRotation(0.0f), m_CenterX(centerX), m_CenterY(centerY), m_Radius(radius), m_GameObject(gameObject)
{}

void dae::RotatorComponent::Update()
{
    m_CurrentRotation += m_RotationSpeed * GameTime::GetDeltaTime();

    // Wrap the angle within the range [0, 2π]
    const float TwoPi = 2.0f * static_cast<float>(M_PI);
    m_CurrentRotation = std::fmod(m_CurrentRotation, TwoPi);


    // Apply rotation around the specified center point
    if (m_GameObject)
    {
        float newX = m_CenterX + m_Radius * std::cos(m_CurrentRotation);
        float newY = m_CenterY + m_Radius * std::sin(m_CurrentRotation);

        m_GameObject->SetLocalPosition(glm::vec3(newX, newY, 0.0f));
    }
}