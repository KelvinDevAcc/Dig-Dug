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

        void Update() const;

        GameObject* GetPlayer() const;
        std::vector<GameObject*> GetPlayers() const {return m_players;}
        std::vector<GameObject*> GetEnemies() const {return m_enemys;}
        std::vector<GameObject*> GetenemyPlayers() const {return m_enemyPlayers;}
        GameObject* GetFloorAt(const glm::vec3& position) const;

        bool IsOnFloor(GameObject& player) const;
        bool isOnEnemy(GameObject& burgerPart) const;

        bool CanEntityMove(float moveX, float moveY, GameObject& entity) const;

        static bool IsWithinBounds(float x, float y);

        bool IsNextObject(float x, float y) const;

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

    };
}
