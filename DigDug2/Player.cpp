#include "Player.h"
#include "EnemyComponent.h"
#include "GameData.h"
#include "GameTime.h"
#include "ResourceManager.h"
#include "SceneData.h"
#include "SceneHelpers.h"
#include "SceneManager.h"

namespace game
{
    Player::Player(dae::GameObject* gameObject)
        : m_GameObject(gameObject),
        m_timeSinceLastAction(0.0f), m_inactivityThreshold(6.0f),
        m_CurrentAnimationState(AnimationState::Idle),
        pumpDirection(glm::vec3(1, 0, 0)),m_maxPumps(4),m_deathTimer(0),m_isDying(false)
    {
        m_animationComponent = m_GameObject->GetComponent<dae::AnimationComponent>();
        m_healthComponent = m_GameObject->GetComponent<dae::HealthComponent>();
        m_pointComponent = m_GameObject->GetComponent<dae::PointComponent>();
        m_startPosition = m_GameObject->GetWorldPosition();

        SetAnimationState(AnimationState::Idle);
    }

    void Player::Update()
    {
        const float deltaTime = dae::GameTime::GetDeltaTime();

        m_timeSinceLastAction += deltaTime;
        if (m_timeSinceLastAction >= m_inactivityThreshold)
        {
            Idle();
            m_timeSinceLastAction = 0;
        }

        if (m_isDying)
        {
            m_deathTimer -= deltaTime;
            if (m_deathTimer <= 0.0f) 
            {
                const int playerID = GetPlayerID();  
                if (playerID != -1) 
                {
                    PlayerData playerData = GameData::GetInstance().GetPlayerData(playerID);
                    playerData.lives = m_healthComponent->GetLives();  
                    GameData::GetInstance().UpdatePlayerData(playerID, playerData);
                }

                if (m_healthComponent->GetLives() < 0)
                {
                    const auto playerSize = dae::SceneData::GetInstance().GetPlayers().size();
                    if (playerSize > 1)
                    {
                        GameData::GetInstance().CheckGameState();
                        dae::SceneData::GetInstance().RemoveGameObject(GetParentObject(), dae::GameObjectType::Player);
                        dae::SceneManager::GetInstance().GetActiveScene()->Remove(GetParentObject());
                    }
                    else
                    {
                        GameData::GetInstance().CheckGameState();
                    }
                }
                else
                    dae::SceneManager::GetInstance().RestartCurrentSceneWithPersistentObjects();
            }
        }

        for (const auto& pump : m_pumps)
        {
            if (pump && pump->IsActive())
            {
                pump->Update();
            }
        }

        UpdatePumpTimer(deltaTime);

      
    }

    void Player::Die()
    {
        if (!m_healthComponent) return;

        m_GameObject->GetComponent<HitBox>()->Disable();
        SetAnimationState(AnimationState::Dying);
       
        m_healthComponent->SetLives(m_healthComponent->GetLives() - 1);
        m_deathTimer =static_cast<float>(m_animationComponent->GetAnimationDuration());
        m_isDying = true;
    }

    void Player::Render() const
    {
        for (const auto& pump : m_pumps)
        {
            if (pump && pump->IsActive())
            {
                pump->Render();
            }
        }
    }

    void Player::ShootPump()
    {
        if (!m_pointComponent || m_isDying) {
            return;
        }

        SetAnimationState(AnimationState::Attacking);
        if (!m_pumps.empty() && m_pumps.back()->IsActive())
        {
            // Iterate through the pump parts to check for collisions with enemies
            for (auto& pumpPart : m_pumps)
            {
                if (pumpPart->IsActive() && pumpPart->IsCollidingWithEnemy())
                {
                    pumpPart->PumpEnemy();
                    return;  // Stop further processing since the enemy is being pumped
                }
            }
            return;
        }

        dae::Message message;
        message.type = dae::PlaySoundMessageType::Sound;
        message.arguments.emplace_back(static_cast<sound_id>(7));
        dae::EventQueue::Broadcast(message);

        m_timeSinceLastPumpPart = 0.0f; // Reset the timer for adding new pump parts
        AddPumpPart(); // Start by adding the first pump part immediately

    }

    void Player::UpdatePumpTimer(float deltaTime)
    {
        if (m_pumps.empty() || m_pumpPartCount > m_maxPumps || m_isDying)
        {
            return;
        }

        m_timeSinceLastPumpPart += deltaTime;
        if (m_timeSinceLastPumpPart >= m_pumpPartInterval)
        {
            m_timeSinceLastPumpPart = 0.0f; // Reset the timer
            AddPumpPart(); // Add a new pump part
        }
    }

    void Player::AddPumpPart()
    {
        if (m_isDying || m_pumpPartCount == m_maxPumps)
            return;

        glm::vec3 offsetPosition(0.0f);
        if (!m_pumps.empty())
        {
            if (m_pumps.back())
            {
                constexpr auto textureSize = glm::vec2(36, 16); // Assuming you have a method to get texture size
                offsetPosition += pumpDirection * glm::vec3(textureSize.x, textureSize.y * 2, 0) * static_cast<float>(m_pumpPartCount);
            }
        }

        glm::vec3 pumpPosition = offsetPosition;

        // Create a temporary game object for collision checking
        auto tempPumpObject = std::make_unique<dae::GameObject>();
        tempPumpObject->SetLocalPosition(pumpPosition + m_GameObject->GetWorldPosition());

        auto hitbox = std::make_unique<HitBox>(glm::vec2(32, 16)); // Assuming this matches the pump's size
        hitbox->SetGameObject(tempPumpObject.get());
        tempPumpObject->AddComponent(std::move(hitbox));

        // Check for collisions before adding a new pump part
        bool isCollidingWithWalkthrough = dae::SceneData::GetInstance().IsOnwalkthrough(*tempPumpObject);

        if (!isCollidingWithWalkthrough)
        {
            m_pumps.clear();
            m_pumpPartCount = 0; 
            return; 
        }

        // Check if it collides with an enemy
        bool isCollidingWithEnemy = dae::SceneData::GetInstance().isOnEnemy(*tempPumpObject);
        if (isCollidingWithEnemy)
        {
            return; 
        }

        // Proceed with adding a new pump part if no collision is detected
        auto pump = std::make_unique<Pump>(m_GameObject, pumpPosition, pumpDirection, 2.0f);
        pump->SetGameObject(m_GameObject);
        pump->Activate();

        m_pumps.push_back(std::move(pump));
        ++m_pumpPartCount;

        UpdatePumpTextures();
    }

    void Player::UpdatePumpTextures() const
    {
        for (size_t i = 0; i < m_pumps.size(); ++i)
        {
            if (i == m_pumps.size() - 1)
            {
                m_pumps[i]->SetTexture(dae::ResourceManager::GetTexture(dae::HashString("PumpEnd")));
            }
            else
            {
                m_pumps[i]->SetTexture(dae::ResourceManager::GetTexture(dae::HashString("PumpLine")));
            }
        }
    }

    void Player::Idle()
    {
        SetAnimationState(AnimationState::Idle);
    }

    void Player::ReSpawn()
    {
        m_GameObject->SetLocalPosition(m_startPosition);
        m_GameObject->GetComponent<HitBox>()->Enable();
        m_isDying = false;
        Idle();
    }


    void Player::Move(float deltaX, float deltaY)
    {
	    if (m_isDying)
            return;

        m_pumps.clear();
        m_pumpPartCount = 0; 

        // Check if the direction changed
        Direction newDirection = Direction::None;
        if (deltaX > 0)
            newDirection = Direction::Right;
        else if (deltaX < 0)
            newDirection = Direction::Left;
        else if (deltaY > 0)
            newDirection = Direction::Down;
        else if (deltaY < 0)
            newDirection = Direction::Up;
       

        if (newDirection != m_CurrentDirection)
        {
            SnapToGrid();
            m_CurrentDirection = newDirection;
        }

        if (deltaX != 0.f && dae::SceneData::GetInstance().CanEntityMove(deltaX,deltaY,*m_GameObject))
        {
            MoveHorizontally(deltaX);
        }
        if (deltaY != 0.f && dae::SceneData::GetInstance().CanEntityMove(deltaX, deltaY, *m_GameObject))
        {
            MoveVertically(deltaY);
        }
    }

    void Player::SnapToGrid() const
    {
        glm::vec3 position = m_GameObject->GetWorldPosition();
        position.x = SnapToGridLineX(position.x);
        position.y = SnapToGridLineY(position.y);
        m_GameObject->SetLocalPosition(position);
    }

    glm::ivec2 Player::PositionToGrid(const glm::vec3& position)
    {
        float gridStartX = SceneHelpers::GetMinCoordinates().x;
        float gridStartY = SceneHelpers::GetMinCoordinates().y;
        float cellSizeX = SceneHelpers::GetCellSize().x;
        float cellSizeY = SceneHelpers::GetCellSize().y;

        return {
            static_cast<int>(std::floor((position.x - gridStartX) / cellSizeX)),
            static_cast<int>(std::floor((position.y - gridStartY) / cellSizeY))
        };
    }

    glm::vec3 Player::GridToPosition(const glm::ivec2& gridCoords)
    {
        float gridStartX = SceneHelpers::GetMinCoordinates().x;
        float gridStartY = SceneHelpers::GetMinCoordinates().y;
        float cellSizeX = SceneHelpers::GetCellSize().x;
        float cellSizeY = SceneHelpers::GetCellSize().y;

        return {
            (cellSizeX * gridCoords.x) + gridStartX,
            (cellSizeY * gridCoords.y) + gridStartY,
            1.0f
        };
    }


    float Player::SnapToGridLineX(float xPosition)
    {
        float gridStartX = SceneHelpers::GetMinCoordinates().x;
        float cellSizeX = SceneHelpers::GetCellSize().x;

        float relativeX = xPosition - gridStartX;
        float snappedX = std::round(relativeX / cellSizeX) * cellSizeX;
        return snappedX + gridStartX;
    }

    float Player::SnapToGridLineY(float yPosition)
    {
        float gridStartY = SceneHelpers::GetMinCoordinates().y;
        float cellSizeY = SceneHelpers::GetCellSize().y;

        float relativeY = yPosition - gridStartY;
        float snappedY = std::round(relativeY / cellSizeY) * cellSizeY;
        return snappedY + gridStartY;
    }

    void Player::MoveHorizontally(float deltaX)
    {
        if (deltaX == 0.0f) return;

        glm::vec3 position = m_GameObject->GetWorldPosition();
        float newPosX = position.x + deltaX * dae::GameTime::GetDeltaTime() * m_Speed;
        SetAnimationState(deltaX > 0 ? AnimationState::Walk_Right : AnimationState::Walk_Left);
        UpdateTunnelType(position, true, deltaX > 0);
        m_GameObject->SetLocalPosition(glm::vec3(newPosX, position.y, position.z));
    }

    void Player::MoveVertically(float deltaY)
    {
        if (deltaY == 0.0f) return;

        glm::vec3 position = m_GameObject->GetWorldPosition();
        float newPosY = position.y + deltaY * dae::GameTime::GetDeltaTime() * m_Speed;
        SetAnimationState(deltaY > 0 ? AnimationState::Walk_Down : AnimationState::Walk_Up);
        UpdateTunnelType(position, false, deltaY > 0);
        m_GameObject->SetLocalPosition(glm::vec3(position.x, newPosY, position.z));
    }

    void Player::UpdateTunnelType(const glm::vec3& position, bool isHorizontal, bool isPositiveDirection) {
        const TunnelType currentTile = SceneHelpers::GetTileTypeAtPosition(position);

        if (currentTile == TunnelType::Empty) {
            SetEndType(position, isHorizontal, isPositiveDirection);
        }
        else if (isHorizontal) {
            HandleHorizontalTunnel(position, currentTile);
        }
        else {
            HandleVerticalTunnel(position, currentTile);
        }

        CheckAndSetCornerTypes(position, isHorizontal, isPositiveDirection);
    }

    void Player::SetEndType(const glm::vec3& position, bool isHorizontal, bool isPositiveDirection) {
        TunnelType endType = isHorizontal ? (isPositiveDirection ? TunnelType::rightEnd : TunnelType::leftEnd)
            : (isPositiveDirection ? TunnelType::bottomEnd : TunnelType::topEnd);
        SceneHelpers::SetTileTypeAtPosition(position, endType);
    }

    void Player::HandleHorizontalTunnel(const glm::vec3& position, TunnelType currentTile) {
        if (currentTile == TunnelType::rightEnd) {
            AdjustPreviousTileForHorizontal(position, glm::vec3(-SceneHelpers::GetCellSize().x, 0, 0));
        }
        else if (currentTile == TunnelType::leftEnd) {
            AdjustPreviousTileForHorizontal(position, glm::vec3(SceneHelpers::GetCellSize().x, 0, 0));
        }
    }

    void Player::HandleVerticalTunnel(const glm::vec3& position, TunnelType currentTile) {
        if (currentTile == TunnelType::topEnd) {
            AdjustPreviousTileForVertical(position, glm::vec3(0, SceneHelpers::GetCellSize().y, 0));
        }
        else if (currentTile == TunnelType::bottomEnd) {
            AdjustPreviousTileForVertical(position, glm::vec3(0, -SceneHelpers::GetCellSize().y, 0));
        }
    }

    void Player::AdjustPreviousTileForHorizontal(const glm::vec3& position, const glm::vec3& offset) {
        glm::vec3 previousPosition = position + offset;
        TunnelType previousTile = SceneHelpers::GetTileTypeAtPosition(previousPosition);
        AdjustTileTypeForHorizontal(previousPosition, previousTile);
        SceneHelpers::SetTileTypeAtPosition(position, TunnelType::rightEnd);
    }

    void Player::AdjustTileTypeForHorizontal(const glm::vec3& position, TunnelType previousTile) {
        if (previousTile == TunnelType::walkThroughUp) {
            SceneHelpers::SetTileTypeAtPosition(position, TunnelType::MiddleBlock);
        }
        else if (previousTile == TunnelType::rightEnd || previousTile == TunnelType::leftEnd) {
            SceneHelpers::SetTileTypeAtPosition(position, TunnelType::walkThroughLeft);
        }
    }

    void Player::AdjustPreviousTileForVertical(const glm::vec3& position, const glm::vec3& offset) {
        glm::vec3 previousPosition = position + offset;
        TunnelType previousTile = SceneHelpers::GetTileTypeAtPosition(previousPosition);
        AdjustTileTypeForVertical(previousPosition, previousTile);
        SceneHelpers::SetTileTypeAtPosition(position, TunnelType::topEnd);
    }

    void Player::AdjustTileTypeForVertical(const glm::vec3& position, TunnelType previousTile) {
        if (previousTile == TunnelType::walkThroughLeft) {
            SceneHelpers::SetTileTypeAtPosition(position, TunnelType::MiddleBlock);
        }
        else if (previousTile == TunnelType::topEnd || previousTile == TunnelType::bottomEnd) {
            SceneHelpers::SetTileTypeAtPosition(position, TunnelType::walkThroughUp);
        }
    }

    void Player::CheckAndSetCornerTypes(const glm::vec3& position, bool isHorizontal, bool isPositiveDirection) {
        glm::vec3 left = position + glm::vec3(-SceneHelpers::GetCellSize().x, 0, 0);
        glm::vec3 right = position + glm::vec3(SceneHelpers::GetCellSize().x, 0, 0);
        glm::vec3 above = position + glm::vec3(0, -SceneHelpers::GetCellSize().y, 0);
        glm::vec3 below = position + glm::vec3(0, SceneHelpers::GetCellSize().y, 0);

        if (isHorizontal) {
            CheckHorizontalCorners(position, left, right, isPositiveDirection);
        }
        else {
            CheckVerticalCorners(position, above, below, isPositiveDirection);
        }
    }

    void Player::CheckHorizontalCorners(const glm::vec3& position, const glm::vec3& left, const glm::vec3& right, bool isPositiveDirection) {
        if (isPositiveDirection && SceneHelpers::GetTileTypeAtPosition(left) == TunnelType::topEnd) {
            SceneHelpers::SetTileTypeAtPosition(left, TunnelType::UpRight);
        }
        else if (!isPositiveDirection && SceneHelpers::GetTileTypeAtPosition(right) == TunnelType::topEnd) {
            SceneHelpers::SetTileTypeAtPosition(right, TunnelType::UpLeft);
        }

        if (isPositiveDirection && SceneHelpers::GetTileTypeAtPosition(left) == TunnelType::bottomEnd) {
            SceneHelpers::SetTileTypeAtPosition(left, TunnelType::UpRight);
        }
        else if (!isPositiveDirection && SceneHelpers::GetTileTypeAtPosition(right) == TunnelType::bottomEnd) {
            SceneHelpers::SetTileTypeAtPosition(right, TunnelType::UpLeft);
        }

        CheckHorizontalWalkThrough(position, left, right);
    }

    void Player::CheckVerticalCorners(const glm::vec3& position, const glm::vec3& above, const glm::vec3& below, bool /*isPositiveDirection*/) {
        // You can apply the same pattern here as in CheckHorizontalCorners
        CheckVerticalWalkThrough(position, above, below);
    }

    void Player::CheckHorizontalWalkThrough(const glm::vec3& position, const glm::vec3& left, const glm::vec3& right) {
        if (SceneHelpers::GetTileTypeAtPosition(right) == TunnelType::walkThroughUp) {
            SceneHelpers::SetTileTypeAtPosition(left, TunnelType::walkThroughLeft);
            SceneHelpers::SetTileTypeAtPosition(position, TunnelType::walkThroughLeft);
        }

        if (SceneHelpers::GetTileTypeAtPosition(left) == TunnelType::walkThroughUp) {
            SceneHelpers::SetTileTypeAtPosition(right, TunnelType::walkThroughLeft);
            SceneHelpers::SetTileTypeAtPosition(position, TunnelType::walkThroughLeft);
        }
    }

    void Player::CheckVerticalWalkThrough(const glm::vec3& position, const glm::vec3& above, const glm::vec3& below) {
        if (SceneHelpers::GetTileTypeAtPosition(above) == TunnelType::walkThroughLeft) {
            SceneHelpers::SetTileTypeAtPosition(below, TunnelType::walkThroughUp);
            SceneHelpers::SetTileTypeAtPosition(position, TunnelType::walkThroughUp);
        }

        if (SceneHelpers::GetTileTypeAtPosition(below) == TunnelType::walkThroughLeft) {
            SceneHelpers::SetTileTypeAtPosition(above, TunnelType::walkThroughUp);
            SceneHelpers::SetTileTypeAtPosition(position, TunnelType::walkThroughUp);
        }
    }


    void Player::SetAnimationState(AnimationState state)
    {
        if (m_CurrentAnimationState == state)
            return;

        m_CurrentAnimationState = state;

        switch (state)
        {
        case AnimationState::Idle:
            if (m_animationComponent)
            {
                if (pumpDirection.x > 0)
                {
                    m_animationComponent->Play("Idle");
                    m_animationComponent->FlipSprite(false, false); // Facing right
                }
                else if (pumpDirection.x < 0)
                {
                    m_animationComponent->Play("Idle");
                    m_animationComponent->FlipSprite(true, false); // Facing left
                }
                else if (pumpDirection.y < 0)
                {
                    m_animationComponent->Play("IdleUp");
                    m_animationComponent->FlipSprite(false, false); // Facing up
                }
                else if (pumpDirection.y > 0)
                {
                    m_animationComponent->Play("IdleUp");
                    m_animationComponent->FlipSprite(false, true); // Facing down
                }
            }
            break;
        case AnimationState::Walk_Up:
            if (m_animationComponent)
            {
                m_animationComponent->FlipSprite(false, false);
                m_animationComponent->Play("Walk_Up", true);
            }
            pumpDirection = glm::vec3(0, -1, 0);
            break;
        case AnimationState::Walk_Right:
            if (m_animationComponent)
            {
                m_animationComponent->FlipSprite(false, false);
                m_animationComponent->Play("Walk_Right", true);
            }

            pumpDirection = glm::vec3(1, 0, 0);
            break;
        case AnimationState::Walk_Left:
            if (m_animationComponent)
            {
                m_animationComponent->FlipSprite(false, false);
                m_animationComponent->Play("Walk_Left", true);
            }
            pumpDirection = glm::vec3(-1, 0, 0);
            break;
        case AnimationState::Walk_Down:
            if (m_animationComponent)
            {
                m_animationComponent->FlipSprite(false, false);
                m_animationComponent->Play("Walk_Down", true);
            }
            pumpDirection = glm::vec3(0, 1, 0);
            break;
        case AnimationState::Attacking:
            if (m_animationComponent)
            {
                if (pumpDirection.x > 0)
                {
                    m_animationComponent->Play("Attacking");
                    m_animationComponent->FlipSprite(false, false); // Facing right
                }
                else if (pumpDirection.x < 0)
                {
                    m_animationComponent->Play("Attacking");
                    m_animationComponent->FlipSprite(true, false); // Facing left
                }
                else if (pumpDirection.y < 0)
                {
                    m_animationComponent->Play("AttackingUp");
                    m_animationComponent->FlipSprite(false, false); // Facing up
                }
                else if (pumpDirection.y > 0)
                {
                    m_animationComponent->Play("AttackingUp");
                    m_animationComponent->FlipSprite(false, true); // Facing down
                }
            }
            break;
        case AnimationState::Dying:
            if (m_animationComponent)
                m_animationComponent->Play("Dying");
            break;
        case AnimationState::Digging:
            if (m_animationComponent)
                m_animationComponent->Play("Digging", true);
            break;
        }
    }

}
