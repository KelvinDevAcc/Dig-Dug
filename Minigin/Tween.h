#pragma once

#include <functional>
#include <glm/vec3.hpp>

#include "EasingFunctions.h"
#include "GameObject.h"

class Transform;

class Tween
{
public:
    Tween(dae::GameObject* gameObject, const glm::vec3& endPosition, float duration, const Easing::EasingFunction& easingFunction, std::function<void()> onComplete = nullptr);

    void Update();

    bool IsActive() const;

private:
    dae::GameObject* m_GameObject;
    glm::vec3 m_StartPosition;
    glm::vec3 m_EndPosition;
    float m_Duration;
    float m_ElapsedTime;
    Easing::EasingFunction m_EasingFunction;
    bool m_IsActive;
    std::function<void()> m_OnComplete;
};
