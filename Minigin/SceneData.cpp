#include "SceneData.h"
#include "SceneHelpers.h"
#include "../BugerTime/Player.h"

namespace dae {
    SceneData& SceneData::GetInstance() {
        static SceneData instance;
        return instance;
    }

    void SceneData::AddGameObject(GameObject* gameObject, GameObjectType type) {
        switch (type) {
        case GameObjectType::Floor: m_floors.push_back(gameObject); break;
        case GameObjectType::LadderUp: m_ladderUp.push_back(gameObject); break;
        case GameObjectType::LadderUpDown: m_ladderUpDown.push_back(gameObject); break;
        case GameObjectType::LadderDown: m_ladderDown.push_back(gameObject); break;
        case GameObjectType::SolidLadder: m_solidLadders.push_back(gameObject); break;
        case GameObjectType::Player: m_players.push_back(gameObject); break;
        case GameObjectType::enemy: m_enemys.push_back(gameObject); break;
        case GameObjectType::BurgerPart: m_burgerParts.push_back(gameObject); break;
        case GameObjectType::Basket: m_baskets.push_back(gameObject); break;
        case GameObjectType::enemyPlayers: m_enemyPlayers.push_back(gameObject);break;
        }
    }

    void SceneData::RemoveAllGameObjects()
    {
        m_floors.clear();
        m_ladderUp.clear();
        m_ladderDown.clear();
        m_ladderUpDown.clear();
        m_solidLadders.clear();
        m_players.clear();
        m_enemyPlayers.clear();
        m_enemys.clear();
        m_burgerParts.clear();
        m_baskets.clear();
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
        default:
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

        if (checkCollisionsWithObjects(m_solidLadders)) return true;
        if (checkCollisionsWithObjects(m_floors)) return true;
        if (checkCollisionsWithObjects(m_ladderUp)) return true;
        if (checkCollisionsWithObjects(m_ladderUpDown)) return true;
        if (checkCollisionsWithObjects(m_ladderDown)) return true;

        return false; 
    }

    GameObject* SceneData::GetPlayer() const {
        return !m_players.empty() ? m_players.front() : nullptr;
    }

    const std::vector<GameObject*>& SceneData::GetBurgerParts() const {
        return m_burgerParts;
    }

    const std::vector<GameObject*>& SceneData::GetBasket() const {
        return m_baskets;
    }

    bool SceneData::IsOnFloor(GameObject& player) const {
        return IsOnSpecificObjectType(player, m_floors);
    }

    bool SceneData::IsOnLadderUp(GameObject& player) const {
        return IsOnSpecificObjectType(player, m_ladderUp);
    }

    bool SceneData::IsOnLadderUpDown(GameObject& player) const {
        return IsOnSpecificObjectType(player, m_ladderUpDown);
    }

    bool SceneData::IsOnLadderDown(GameObject& player) const {
        return IsOnSpecificObjectType(player, m_ladderDown);
    }

	bool SceneData::IsOnSolidLadder(GameObject& player) const {
        return IsOnSpecificObjectType(player, m_solidLadders);
    }
    bool SceneData::IsInBasket(GameObject& burger) const {
        return IsOnSpecificObjectType(burger, m_baskets);
    }

    bool SceneData::IsBurgerPartColliding(GameObject& burgerPart) const {
        return IsOnSpecificObjectType(burgerPart, m_burgerParts);
    }

    bool SceneData::isOnEnemy(GameObject& burgerPart) const {
        return IsOnSpecificObjectType(burgerPart, m_enemys);
    }
}
