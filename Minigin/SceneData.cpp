#include "SceneData.h"

#include <SDL_rect.h>
#include "SceneHelpers.h"
#include "../DigDug2/Player.h"

namespace dae {
    SceneData& SceneData::GetInstance() {
        static SceneData instance;
        return instance;
    }

    void SceneData::AddGameObject(GameObject* gameObject, GameObjectType type) {
        switch (type) {
        case GameObjectType::Floor: m_floors.push_back(gameObject); break;
        case GameObjectType::Player: m_players.push_back(gameObject); break;
        case GameObjectType::enemy: m_enemys.push_back(gameObject); break;
        case GameObjectType::enemyPlayers: m_enemyPlayers.push_back(gameObject); break;
        case GameObjectType::WalkThrough: m_WalkThrough.push_back(gameObject); break;
        case GameObjectType::Rock: m_rocks.push_back(gameObject); break;
        case GameObjectType::fire: m_fire.push_back(gameObject); break;
        case GameObjectType::Empty: m_empty.push_back(gameObject); break;
        default:;
        }
    }

    void SceneData::RemoveAllGameObjects()
    {
        m_floors.clear();
        m_WalkThrough.clear();
        m_players.clear();
        m_enemyPlayers.clear();
        m_enemys.clear();
        m_empty.clear();
        m_rocks.clear();
        m_fire.clear();
    }

    void SceneData::RemoveGameObject(GameObject* gameObject, GameObjectType type)
    {
        switch (type)
        {
        case GameObjectType::Player:
            RemoveGameObjectFromList(gameObject, m_players);
            break;
        case GameObjectType::enemy:
            RemoveGameObjectFromList(gameObject, m_enemys);
            break;
        case GameObjectType::enemyPlayers:
            RemoveGameObjectFromList(gameObject, m_enemyPlayers);
            break;
        case GameObjectType::Floor:
            RemoveGameObjectFromList(gameObject, m_floors);
            break;
        case GameObjectType::WalkThrough:
            RemoveGameObjectFromList(gameObject, m_WalkThrough);
            break;
        case GameObjectType::Rock:
            RemoveGameObjectFromList(gameObject, m_rocks);
            break;
        case GameObjectType::Empty:
            RemoveGameObjectFromList(gameObject, m_empty);
            break;
        case GameObjectType::fire:
            RemoveGameObjectFromList(gameObject, m_fire);
            break;
        default:;
        }
    }

    void SceneData::RemoveGameObjectAtPosition(const glm::vec3& position)
    {
        auto& gameObjects = m_WalkThrough; 

        const auto it = std::ranges::remove_if(gameObjects,
                                               [&position](const GameObject* gameObject) {
	                                               const glm::vec3& objPos = gameObject->GetWorldPosition();
	                                               return std::abs(objPos.x - position.x) < 0.1f && std::abs(objPos.y - position.y) < 0.1f;
                                               }).begin();

        gameObjects.erase(it, gameObjects.end());
    }



    template<typename T>
    void SceneData::RemoveGameObjectFromList(GameObject* gameObject, std::vector<T>& list)
    {
        auto it = std::find(list.begin(), list.end(), gameObject);
        if (it != list.end())
        {
            list.erase(it); 
        }
    }

    void SceneData::Update() const
    {
	    if (!m_enemys.empty()|| !m_rocks.empty())
	    {
            for (GameObject* player : GetPlayers()) {
                if (!m_enemys.empty())
                {
                    if (player && isOnEnemy(*player)) 
                    {
                        player->GetComponent<game::Player>()->Die();
                    }
                }
                if (!m_rocks.empty())
                {
                    if (player && IsOnRock(*player))
                    {
                        player->GetComponent<game::Player>()->Die();
                    }
                }
                if (!m_fire.empty())
                {
                    if (player && IsOnFire(*player))
                    {
                        player->GetComponent<game::Player>()->Die();
                    }
                }

            }
	    }

    }

    bool SceneData::IsOnSpecificObjectType(GameObject& object, const std::vector<GameObject*>& objects)
    {
        const auto hitBox = object.GetComponent<HitBox>();
        if (!hitBox || !hitBox->GetEnable()) return false;

        for (const auto& gameObject : objects) {
            if (gameObject == &object) continue;

            const auto otherHitBox = gameObject->GetComponent<HitBox>();
            if (otherHitBox && hitBox->IsColliding(*otherHitBox)) {
                return true;
            }
        }
        return false;
    }

    bool dae::SceneData::CanEntityMove(float moveX, float moveY, dae::GameObject& entity) const
    {
        const glm::vec3 currentPosition = entity.GetWorldPosition();
        const glm::vec3 newPosition = currentPosition + glm::vec3(moveX, moveY, 0.0f);

        if (!IsWithinBounds(newPosition.x, newPosition.y))
        {
            return false;
        }

        return !IsNextObject(newPosition,entity);
    }

    bool dae::SceneData::IsWithinBounds(float x, float y) const
    {
        const float minX = SceneHelpers::GetMinCoordinates().x;
        const float minY = SceneHelpers::GetMinCoordinates().y;
        const float maxX = SceneHelpers::GetMaxCoordinates().x;
        const float maxY = SceneHelpers::GetMaxCoordinates().y;

        return x >= minX && x < maxX && y >= minY && y < maxY;
    }

    bool dae::SceneData::IsNextObject(const glm::vec3& newPosition,GameObject& entity) const
    {
        auto checkCollisionsWithObjects = [&](const std::vector<GameObject*>& objects) {
            for (const auto gameObject : objects) {
                if (const auto hitBox = gameObject->GetComponent<HitBox>())
                {
	                if (hitBox->GetEnable())
	                {
                        const SDL_Rect rect = hitBox->GetRect();

                        // Calculate the bounds based on the center position
                        int left = rect.x;
                        int right = rect.x + rect.w;
                        int top = rect.y;
                        int bottom = rect.y + rect.h;

                        const auto playerHitBox = entity.GetComponent<HitBox>()->GetRect();

                        // Check if the new position will intersect the obstacle
                        if (newPosition.x + playerHitBox.w / 2 > left && newPosition.x - playerHitBox.w / 2 < right &&
                            newPosition.y + playerHitBox.h / 2 > top && newPosition.y - playerHitBox.h / 2 < bottom) {
                            return true;
                        }
	                }
                }
            }
            return false;
            };

        if (checkCollisionsWithObjects(m_rocks))
            return true;
        if (checkCollisionsWithObjects(m_empty))
            return true;

        return false;
    }

    bool SceneData::CanEnemyMove(float moveX, float moveY, const GameObject& entity) const {
        const glm::vec3 currentPosition = entity.GetWorldPosition();
        const glm::vec3 newPosition = currentPosition + glm::vec3(moveX, moveY, 0.0f);

        if (!IsWithinBounds(newPosition.x, newPosition.y))
        {
            return false;
        }

        return IsNextwalkthrough(newPosition.x, newPosition.y);
    }

    bool SceneData::IsNextwalkthrough(float x, float y) const {
        auto checkCollisionsWithObjects = [&](const std::vector<GameObject*>& objects) {
            for (const auto gameObject : objects) 
            {
	            if (gameObject)
	            {
                    if (const auto hitBox = gameObject->GetComponent<HitBox>()) {
                        const SDL_Rect rect = hitBox->GetRect();
                        if (x >= rect.x && x < rect.x + rect.w &&
                            y >= rect.y && y < rect.y + rect.h) {
                            return true;
                        }
                    }
	            }
            }
            return false;
            };

        if (checkCollisionsWithObjects(m_WalkThrough))
            return true;

        return false;
    }

    GameObject* SceneData::GetPlayer() const {
        return !m_players.empty() ? m_players.front() : nullptr;
    }

    bool SceneData::IsOnFloor(GameObject& player) const {
        return IsOnSpecificObjectType(player, m_floors);
    }

    bool SceneData::IsOnwalkthrough(GameObject& player) const {
        return IsOnSpecificObjectType(player, m_WalkThrough);
    }

    bool SceneData::isOnEnemy(GameObject& object) const
    {
        return IsOnSpecificObjectType(object, m_enemys);
    }

    bool SceneData::IsOnRock(GameObject& object) const
    {
        return IsOnSpecificObjectType(object, m_rocks);
    }

    bool SceneData::IsOnFire(GameObject& object) const
    {
        return IsOnSpecificObjectType(object, m_fire);
    }
}
