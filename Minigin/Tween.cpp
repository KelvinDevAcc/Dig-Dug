#include "Tween.h"
#include "GameTime.h"
#include "Transform.h"

Tween::Tween(dae::GameObject* gameObject, const glm::vec3& endPosition, float duration, Easing::EasingFunction easingFunction, std::function<void()> onComplete)
    : m_GameObject(gameObject), m_StartPosition(gameObject->GetLocalPosition()), m_EndPosition(endPosition),
    m_Duration(duration), m_ElapsedTime(0.0f), m_EasingFunction(easingFunction), m_IsActive(true),
    m_OnComplete(onComplete) {}


void Tween::Update() {
    if (!m_IsActive)
        return;

    m_ElapsedTime += dae::GameTime::GetDeltaTime();
    float t = m_ElapsedTime / m_Duration;
    if (t >= 1.0f) {
        t = 1.0f;
        m_IsActive = false;
        if (m_OnComplete) {
            m_OnComplete();
        }
    }

    glm::vec3 newPosition = m_StartPosition + (m_EndPosition - m_StartPosition) * m_EasingFunction(t);
    m_GameObject->SetLocalPosition(newPosition);
}

bool Tween::IsActive() const {
    return m_IsActive;
}
