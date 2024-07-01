#pragma once
#include "LoadMap.h"
#include "Scene.h" 
#include <glm/vec3.hpp>

class SceneHelpers final
{
public:
    static void LoadMapIntoScene(const LoadMap& loadMap, dae::Scene* scene, const glm::vec3& startPos, glm::vec2 scale);

    static void LoadIngMapIntoScene(const LoadMap& loadMap, dae::Scene* scene, const glm::vec3& startPos, glm::vec2 scale);

    static glm::vec2 GetMinCoordinates();
    static glm::vec2 GetMaxCoordinates();

    static glm::vec2 GetCellSize();

    static glm::vec2 GetGridSize();
    static std::vector<std::vector<char>> GetLoadMap();

private:
    static void CreatePlatformLeft(dae::Scene* scene, float x, float y, glm::vec2 scale);
    static void CreatePlatformMiddle(dae::Scene* scene, float x, float y, glm::vec2 scale);
    static void CreatePlatformRight(dae::Scene* scene, float x, float y, glm::vec2 scale);
    static void CreatePlatformCombined(dae::Scene* scene, float x, float y, glm::vec2 scale);
    static void CreateLadderUp(dae::Scene* scene, float x, float y, glm::vec2 scale);
    static void CreateLadderUpDown(dae::Scene* scene, float x, float y, glm::vec2 scale);
    static void CreateLadderDown(dae::Scene* scene, float x, float y, glm::vec2 scale);
    static void CreateSolidLadder(dae::Scene* scene, float x, float y, glm::vec2 scale);
    static void CreateFloor(dae::Scene* scene, float x, float y, glm::vec2 scale);

    static void CreateBurgerTop(dae::Scene* scene, float x, float y, glm::vec2 scale);
    static void CreateCheese(dae::Scene* scene, float x, float y, glm::vec2 scale);
    static void CreateMeat(dae::Scene* scene, float x, float y, glm::vec2 scale);
    static void CreateTomato(dae::Scene* scene, float x, float y, glm::vec2 scale);
    static void CreateLettuce(dae::Scene* scene, float x, float y, glm::vec2 scale);
    static void CreateBurgerBottom(dae::Scene* scene, float x, float y, glm::vec2 scale);

    static void SpawnSauge(dae::Scene* scene, float x, float y, glm::vec2 scale);
    static void SpawnEgg(dae::Scene* scene, float x, float y, glm::vec2 scale);
    static void SpawnPickle(dae::Scene* scene, float x, float y, glm::vec2 scale);

    static void SpawnPlayer(dae::Scene* scene, float x, float y, glm::vec2 scale);
    static void SpawnPlayerEnemy(dae::Scene* scene, float x, float y, glm::vec2 scale);


};
