#pragma once
#include <vector>
#include "GameObject.h"
#include "HitBox.h"
#include "Singleton.h"

namespace dae {
    enum class GameObjectType {
        Floor,
        LadderUp,
        LadderUpDown,
        LadderDown,
        SolidLadder,
        BurgerPart,
        Player,
        enemyPlayers,
        enemy,
        Basket
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
        std::vector<GameObject*> GetenemyPlayers() const {return m_enemyPlayers;}
        const std::vector<GameObject*>& GetBurgerParts() const;
        const std::vector<GameObject*>& GetBasket()const;
        GameObject* GetFloorAt(const glm::vec3& position) const;

        bool IsOnFloor(GameObject& player) const;
        bool IsOnLadderUp(GameObject& player) const;
        bool IsOnLadderUpDown(GameObject& player) const;
        bool IsOnLadderDown(GameObject& player) const;
        bool IsOnSolidLadder(GameObject& player) const;
        bool IsInBasket(GameObject& burger) const;
        bool IsBurgerPartColliding(GameObject& burgerPart) const;
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
        std::vector<GameObject*> m_ladderUp;
        std::vector<GameObject*> m_ladderUpDown;
        std::vector<GameObject*> m_ladderDown;
        std::vector<GameObject*> m_solidLadders;
        std::vector<GameObject*> m_players;
        std::vector<GameObject*> m_enemyPlayers;
        std::vector<GameObject*> m_enemys;
        std::vector<GameObject*> m_burgerParts;
        std::vector<GameObject*> m_baskets;


    };
}
