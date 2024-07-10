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
    static void CreateFloor(dae::Scene* scene, float x, float y, glm::vec2 scale , const std::string& textureName);
    static void CreateWalkThough(dae::Scene* scene, float x, float y, glm::vec2 scale , const std::string& textureName);

};
