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
        case GameObjectType::enemyPlayers: m_enemyPlayers.push_back(gameObject);break;
        case GameObjectType::WalkThrough: m_WalkThrough.push_back(gameObject);  break;
        default: ;
        }
    }

    void SceneData::RemoveAllGameObjects()
    {
        m_floors.clear();
        m_WalkThrough.clear();
        m_players.clear();
        m_enemyPlayers.clear();
        m_enemys.clear();
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

        }
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
        for (GameObject* player : GetPlayers()) {
            if (player && isOnEnemy(*player)) {
                player->GetComponent<game::Player>()->Die(); 
            }
        }
    }

    bool SceneData::IsOnSpecificObjectType(GameObject& object, const std::vector<GameObject*>& objects)
    {
        const auto hitBox = object.GetComponent<HitBox>();
        if (!hitBox) return false;

        for (const auto& gameObject : objects) {
            if (gameObject == &object) continue;

            const auto otherHitBox = gameObject->GetComponent<HitBox>();
            if (otherHitBox && hitBox->IsColliding(*otherHitBox)) {

                return true;
            }
        }
        return false;
    }

    bool SceneData::IsWithinBounds(float x, float y)
    {
        const float minX = SceneHelpers::GetMinCoordinates().x;
        const float minY = SceneHelpers::GetMinCoordinates().y;
        const float maxX = SceneHelpers::GetMaxCoordinates().x;
        const float maxY = SceneHelpers::GetMaxCoordinates().y;

        return x >= minX && x < maxX && y >= minY && y < maxY;
    }

    bool SceneData::CanEntityMove(float moveX, float moveY, GameObject& entity) const {
        const glm::vec3 currentPosition = entity.GetWorldPosition();

        const glm::vec3 newPosition = currentPosition + glm::vec3(moveX, moveY, 0.0f);

        if (!IsWithinBounds(newPosition.x, newPosition.y)) 
        {
            return false; 
        }

        return  IsNextObject(newPosition.x, newPosition.y);
    }

    GameObject* SceneData::GetFloorAt(const glm::vec3& position) const
    {
        for (const auto& floor : m_floors) {
	        if (const auto hitBox = floor->GetComponent<HitBox>()) {
                const SDL_Rect rect = hitBox->GetRect();
                if (position.x >= rect.x && position.x < rect.x + rect.w &&
                    position.y >= rect.y && position.y < rect.y + rect.h) {
                    return floor;
                }
            }
        }
        return nullptr;
    }

    bool SceneData::IsNextObject(float x, float y) const {
        auto checkCollisionsWithObjects = [&](const std::vector<GameObject*>& objects) {
            for (const auto gameObject : objects) {
	            if (const auto hitBox = gameObject->GetComponent<HitBox>()) {
                    const SDL_Rect rect = hitBox->GetRect();
                    if (x >= rect.x && x < rect.x + rect.w &&
                        y >= rect.y && y < rect.y + rect.h) {
                        return true; 
                    }
                }
            }
            return false; 
            };

        if (checkCollisionsWithObjects(m_floors)) return true;
        return false; 
    }

    GameObject* SceneData::GetPlayer() const {
        return !m_players.empty() ? m_players.front() : nullptr;
    }

  
    bool SceneData::IsOnFloor(GameObject& player) const {
        return IsOnSpecificObjectType(player, m_floors);
    }

    bool SceneData::isOnEnemy(GameObject& burgerPart) const {
        return IsOnSpecificObjectType(burgerPart, m_enemys);
    }
}
