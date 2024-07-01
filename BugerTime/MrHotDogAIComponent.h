#pragma once
#include <vector>
#include <unordered_map>
#include <SDL.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "EnemyAIComponent.h"
#include "GameTime.h"


struct PairHash {
    template <typename T, typename U>
    std::size_t operator()(const std::pair<T, U>& pair) const {
        return std::hash<T>()(pair.first) ^ std::hash<U>()(pair.second);
    }
};

class MrHotDogAIComponent final : public dae::Component
{
public:
    MrHotDogAIComponent(dae::GameObject* owner, std::vector<std::vector<char>>& loadMap);
    ~MrHotDogAIComponent() override = default;

    MrHotDogAIComponent(const MrHotDogAIComponent&) = delete;
    MrHotDogAIComponent& operator=(const MrHotDogAIComponent&) = delete;
    MrHotDogAIComponent(MrHotDogAIComponent&&) noexcept = default;
    MrHotDogAIComponent& operator=(MrHotDogAIComponent&&) noexcept = default;

    void Update() override;

    glm::vec3 GetMoveDirection() const;

    std::type_info const& GetComponentType() const override { return typeid(MrHotDogAIComponent); }

private:
    std::vector<SDL_Rect*> CalculatePathToPeter(const glm::vec3& position);
    std::vector<std::pair<int, int>> GetNeighbors(const std::pair<int, int>& node) const;
    std::vector<SDL_Rect*> ReconstructPath(const std::unordered_map<std::pair<int, int>, std::pair<int, int>, PairHash>& cameFrom, const std::pair<int, int>& current) const;

    bool IsCellWalkable(int x, int y) const;
    float GetCost(const std::pair<int, int>& next) const;
    static float GetHeuristicCost(const std::pair<int, int>& pNodeA, const std::pair<int, int>& pNodeB);

    std::vector<std::unique_ptr<SDL_Rect>> m_path;
    std::vector<std::unique_ptr<SDL_Rect>> m_gridCells;

    float m_movementSpeed = 20.f;
    dae::GameObject* m_gameObject;
    glm::vec2 m_cellSize;
    glm::vec2 m_minCoords;
    glm::vec2 m_maxCoords;
    glm::vec3 m_moveDirection;
    int m_numCellsX;
    int m_numCellsY;
    SDL_Rect* m_startNode;
    SDL_Rect* m_goalNode;
    std::vector<std::vector<char>> m_map;
};



