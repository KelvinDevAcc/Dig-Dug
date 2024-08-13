#include "FygarComponent.h"
#include "GameTime.h"
#include "SceneData.h"
#include "GameObject.h"
#include <random>
#include "BreathFireComponent.h"

FygarComponent::FygarComponent(dae::GameObject* owner)
    : EnemyComponent(owner), m_Direction(0, 0, 0), m_FireBreathCooldown(0.0f), m_FireBreathInterval(20.0f)
{
    // Initialize fire breath timer with a random interval
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(10.0f, 20.0f);
    m_FireBreathInterval = dis(gen);
    m_FireBreathCooldown = m_FireBreathInterval;

    // Initialize the breath fire component
    auto breathFire = std::make_unique<game::BreathFire>(owner, glm::vec3(1, 0, 0), 100.0f, 3.0f);
    m_breathFire = breathFire.get();
    owner->AddComponent(std::move(breathFire));
}

void FygarComponent::Update()
{
    static glm::vec3 lastPosition = m_Owner->GetWorldPosition();
    const glm::vec3 currentPosition = m_Owner->GetWorldPosition();
    m_Direction = currentPosition - lastPosition;

    // Optional: Normalize the direction if used for directional purposes
    if (glm::length(m_Direction) > 0.0f)
        m_Direction = glm::normalize(m_Direction);

    lastPosition = currentPosition;

    EnemyComponent::Update();
}

void FygarComponent::UpdateFireBreath() {
    const float deltaTime = dae::GameTime::GetDeltaTime();

    // Reduce the cooldown timer
    m_FireBreathCooldown -= deltaTime;

    // Ensure the cooldown is clamped to zero or above
    if (m_FireBreathCooldown < 0.0f) {
        m_FireBreathCooldown = 0.0f;
    }
}

bool FygarComponent::ShouldBreatheFire() const
{
    // Check if the cooldown timer has reached zero and Fygar is not in ghost mode
    return m_FireBreathCooldown <= 0.0f && !m_GhostModeEnabled;
}

void FygarComponent::ResetFireBreathCooldown() {
    // Reset the fire breath cooldown timer with a new random interval
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(10.0f, 20.0f);
    m_FireBreathInterval = dis(gen);
    m_FireBreathCooldown = m_FireBreathInterval;
}

bool FygarComponent::WasKilledFromSide() const
{
    // Ensure that there is a valid last attacker
    const auto* lastAttacker = GetLastAttacker();
    if (!lastAttacker) {
        return false; // No attacker, can't determine side
    }

    // Get the killer's position and Fygar's position
    const glm::vec3 killerPosition = lastAttacker->GetWorldPosition();
    const glm::vec3 fygarPosition = m_Owner->GetWorldPosition();

    // Define the margin for side detection
    constexpr float margin = 40.0f;

    // Calculate the absolute difference in x-coordinates
    float xDifference = glm::abs(killerPosition.x - fygarPosition.x);

    // Check if the difference is greater than the margin
    bool isSide = xDifference > margin;

    return isSide;
}


