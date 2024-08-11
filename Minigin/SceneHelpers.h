#pragma once
#include "LoadMap.h"
#include "Scene.h"
#include <glm/vec3.hpp>

#include "TunnelType.h"

class SceneHelpers final
{
public:
    static void LoadMapIntoScene(const LoadMap& loadMap, dae::Scene* scene, const glm::vec3& startPos, glm::vec2 scale);

    static void LoadTunnelMapIntoScene(const LoadMap& loadMap, dae::Scene* scene, const glm::vec3& startPos, glm::vec2 scale);
    static void LoadEntitysMapIntoScene(const LoadMap& loadMap, dae::Scene* scene, const glm::vec3& startPos, glm::vec2 scale);

    static glm::vec2 GetMinCoordinates();
    static glm::vec2 GetMaxCoordinates();

    static glm::vec2 GetCellSize();

    static glm::vec2 GetGridSize();

    static TunnelType GetTileTypeAtPosition(const glm::vec3& position);
    static void SetTileTypeAtPosition(const glm::vec3& position, TunnelType newType);
    static glm::vec3 GetCenterOfTile(const glm::vec3& position);
private:
    static void CreateFloor(dae::Scene* scene, float x, float y, glm::vec2 scale, const std::string& textureName);
    static void CreateWalkThough(dae::Scene* scene, float x, float y, glm::vec2 scale, const std::string& textureName, char tileChar);
    static void CreateEmpty(dae::Scene* scene, float x, float y, glm::vec2 scale, const std::string& textureName);
    static void SpawnPooka(dae::Scene* scene, float x, float y, glm::vec2 scale);
    static void SpawnFygar(dae::Scene* scene, float x, float y, glm::vec2 scale);
    static void SpawnStone(dae::Scene* scene, float x, float y, glm::vec2 scale);

    static void SpawnPlayer(dae::Scene* scene, float x, float y, glm::vec2 scale);
    static void SpawnPlayerEnemy(dae::Scene* scene, float x, float y, glm::vec2 scale);

    static LoadMap* s_loadMap;
    static std::vector<std::vector<char>> s_tunnelMap;
    static dae::Scene* m_scene;
    static glm::vec2 s_MinCoordinates;
    static glm::vec2 s_MaxCoordinates;
    static glm::vec2 s_CellSize;
    static int playerCount;
};
