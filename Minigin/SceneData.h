#pragma once
#include <vector>
#include "GameObject.h"
#include "HitBox.h"
#include "Singleton.h"

namespace dae {
    enum class GameObjectType {
        Floor,
        WalkThrough,
        Player,
        enemyPlayers,
        enemy,
        Rock,
        fire,
        Empty
    };

    class SceneData final : public Singleton<SceneData> {
    public:
        static SceneData& GetInstance();
        SceneData(const SceneData&) = delete;
        SceneData(SceneData&&) noexcept = delete;
        SceneData& operator=(SceneData&&) noexcept = delete;

        void AddGameObject(GameObject* gameObject, GameObjectType type);
        void RemoveAllGameObjects();

        void RemoveGameObject(GameObject* gameObject, GameObjectType type);
        void RemoveGameObjectAtPosition(const glm::vec3& position);

        void Update() const;

        GameObject* GetPlayer() const;
        std::vector<GameObject*> GetPlayers() const {return m_players;}
        std::vector<GameObject*> GetEnemies() const {return m_enemys;}
        std::vector<GameObject*> GetenemyPlayers() const {return m_enemyPlayers;}
        std::vector<GameObject*> GetWalkThrough() const {return m_WalkThrough;}
        std::vector<GameObject*> getFloors() const {return m_floors;}
        std::vector<GameObject*> getRocks() const {return m_rocks;}
        std::vector<GameObject*> GetFire() const {return m_fire;}

        bool IsOnFloor(GameObject& gameObject) const;
        bool isOnEnemy(GameObject& gameObject) const;
        bool IsOnwalkthrough(GameObject& gameObject) const;
        bool IsOnRock(GameObject& gameObject) const;
        bool IsOnFire(GameObject& gameObject) const;

        bool CanEntityMove(float moveX, float moveY, GameObject& entity) const;
        bool CanEnemyMove(float moveX, float moveY, const GameObject& entity) const;

        bool IsWithinBounds(float x, float y)const;

        bool IsNextObject(const glm::vec3& newPosition, GameObject& entity) const;
        bool IsNextwalkthrough(float x, float y) const;

        static bool IsOnSpecificObjectType(GameObject& object, const std::vector<GameObject*>& objects);

    private:
        SceneData() = default;

        template<typename T>
        void RemoveGameObjectFromList(GameObject* gameObject, std::vector<T>& list);

        std::vector<GameObject*> m_floors;
        std::vector<GameObject*> m_WalkThrough;
        std::vector<GameObject*> m_players;
        std::vector<GameObject*> m_enemyPlayers;
        std::vector<GameObject*> m_enemys;
        std::vector<GameObject*> m_rocks;
        std::vector<GameObject*> m_empty;
        std::vector<GameObject*> m_fire;

    };
}
