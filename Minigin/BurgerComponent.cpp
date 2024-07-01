#include "BurgerComponent.h"

#include "EnventQueue.h"
#include "SceneData.h"
#include "HitBox.h"
#include "PointComponent.h"
#include "sound_system.h"
#include "SpriteRendererComponent.h"

namespace game
{
	class Player;
}

BurgerComponent::BurgerComponent(float length, float dropDistance)
	: m_DroppingPlayer(nullptr), m_IsDropping(false), m_Length(length), m_DropDistance(dropDistance),
	  m_PlayerWalkedDistance(0.0f), m_HasDropped(false)
{
}

void BurgerComponent::Update() {
    if (m_IsDropping) {
        DropToNextFloor();
    }
    else if (!m_IsDropping) {
        if (IsPlayerOverComponent()) {
            m_IsDropping = true;
            if (m_DroppingPlayer)
            {
                const int score = m_DroppingPlayer->GetComponent<dae::PointComponent>()->GetScore();
                m_DroppingPlayer->GetComponent<dae::PointComponent>()->SetScore(score + 50);
                m_DroppingPlayer = nullptr;
            }
        }
        else {
            m_PlayerWalkedDistance = 0.0f; 
        }
    }
}

bool BurgerComponent::IsPlayerOverComponent() 
{
    const auto& sceneData = dae::SceneData::GetInstance();
    const auto players = sceneData.GetPlayers();
    if (players.empty()) return false;

    const auto burgerPosition = GetGameObject()->GetWorldPosition();
    float minDistance = std::numeric_limits<float>::max();
    dae::GameObject* closestPlayer = nullptr;

    for (const auto player : players)
    {
        const auto playerHitBox = player->GetComponent<HitBox>();
        if (!playerHitBox) continue;

        const auto playerPosition = player->GetWorldPosition();
        const float distance = glm::distance(playerPosition, burgerPosition);
        if (distance < minDistance)
        {
            minDistance = distance;
            closestPlayer = player;
        }
    }

    if (!closestPlayer) return false;
    m_DroppingPlayer = closestPlayer;
    const auto burgerHitBox = GetGameObject()->GetComponent<HitBox>();
    return burgerHitBox && closestPlayer->GetComponent<HitBox>() && burgerHitBox->IsColliding(*closestPlayer->GetComponent<HitBox>());
}

float BurgerComponent::GetPlayerMovementOverComponent() const
{
	const auto& sceneData = dae::SceneData::GetInstance();
    const auto player = sceneData.GetPlayer();
    if (!player) {
        return 0.0f;
    }

    const glm::vec3 playerMovement = player->GetWorldPosition();
    const auto playerHitBox = player->GetComponent<HitBox>();
    const auto burgerHitBox = GetGameObject()->GetComponent<HitBox>();

    if (!playerHitBox) {
        return 0.0f;
    }
    if (!burgerHitBox) {
        return 0.0f;
    }

    if (playerHitBox->IsColliding(*burgerHitBox)) {
        return glm::length(playerMovement); 
    }

    return 0.0f;
}


void BurgerComponent::StartDropping() {
    m_IsDropping = true;

}

bool BurgerComponent::GetFalling() const
{
    return m_IsDropping;
}


void BurgerComponent::DropToNextFloor() {
    const auto& sceneData = dae::SceneData::GetInstance();
    auto* burger = GetGameObject();

    const auto burgerHitBox = burger->GetComponent<HitBox>();
    if (!burgerHitBox) {
        m_IsDropping = false;
        return;
    }

    constexpr glm::vec3 dropVector(0.0f, 20.0f, 0.0f); 
    const glm::vec3 currentPosition = burger->GetWorldPosition();
    const glm::vec3 newPosition = currentPosition + dropVector;
    burger->SetLocalPosition(newPosition);


    // Re-check conditions after moving
    const bool isOnFloor = sceneData.IsOnFloor(*burger);
    const bool isInBasket = sceneData.IsInBasket(*burger);
    const bool isOnBurgerParts = sceneData.IsBurgerPartColliding(*burger);
    const bool isOnEnemy = sceneData.isOnEnemy(*burger);

    int enemyCount = 0;

    if (isOnEnemy)
    {
	    // Increment enemy count
        enemyCount++;
    }

    if (isInBasket) {
        //int points = 500 * (1 << (enemyCount - 1)); // Using bitwise left shift for exponential calculation
        m_IsDropping = false;
    }
    // Check conditions
    else if (isOnBurgerParts && !isInBasket) {
        const auto& burgerParts = sceneData.GetBurgerParts();
        for (auto part : burgerParts) {
            if (part != burger && sceneData.IsBurgerPartColliding(*part)) {
                auto* partBurgerComponent = part->GetComponent<BurgerComponent>();
                if (partBurgerComponent) {
                    partBurgerComponent->StartDropping();
                   // int points = 500 * (1 << (enemyCount - 1)); 
                    m_IsDropping = false;
                }
            }
        }
    }
    else if (isOnFloor) {
       // int points = 500 * (1 << (enemyCount - 1)); 
        m_IsDropping = false;
        dae::Message message;

        message.type = dae::PlaySoundMessageType::Sound;

        message.arguments.emplace_back(static_cast<sound_id>(1));

        dae::EventQueue::Broadcast(message);
    }

}
