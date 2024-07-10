#include "SceneHelpers.h"
#include "ResourceManager.h"
#include "SceneData.h"
#include "SpriteRendererComponent.h"

glm::vec2 s_MinCoordinates;
glm::vec2 s_MaxCoordinates; 
glm::vec2 s_CellSize;
int playerCount;
std::vector<std::vector<char>> s_loadMap;

void CreateGameObject(dae::Scene* scene, const std::string& textureName, float x, float y, glm::vec2 scale, dae::GameObjectType type) {
    auto gameObject = std::make_unique<dae::GameObject>();
    auto spriteRenderer = std::make_unique<dae::SpriteRendererComponent>(gameObject.get(), dae::ResourceManager::GetTexture(textureName));
    spriteRenderer->SetDimensions(scale.x, scale.y);
    gameObject->AddComponent(std::move(spriteRenderer));
    gameObject->SetLocalPosition(glm::vec3(x, y, 0.0f));

    auto hitBox = std::make_unique<HitBox>(scale);
    hitBox->SetGameObject(gameObject.get());
    gameObject->AddComponent(std::move(hitBox));

    dae::SceneData::GetInstance().AddGameObject(gameObject.get(), type);
    scene->Add(std::move(gameObject));
}

void SceneHelpers::CreateFloor(dae::Scene* scene, float x, float y, glm::vec2 scale, const std::string& textureName) {
    CreateGameObject(scene, textureName, x, y, scale, dae::GameObjectType::Floor);
}

void SceneHelpers::CreateWalkThough(dae::Scene* scene, float x, float y, glm::vec2 scale, const std::string& textureName)
{
    CreateGameObject(scene, textureName, x, y, scale, dae::GameObjectType::WalkThrough);
}

void SceneHelpers::LoadMapIntoScene(const LoadMap& loadMap, dae::Scene* scene, const glm::vec3& startPos, glm::vec2 scale) {
    const auto& map = loadMap.getMap();
    s_loadMap = map;

    s_MinCoordinates = { startPos.x, startPos.y };
    s_MaxCoordinates = { startPos.x, startPos.y };
    s_CellSize = scale;

    // Define a mapping from tile characters to floor types
    const std::unordered_map<char, std::string> tileToFloorType = {
        {'v', "sky"}, {'#', "skyFloor"}, {'^', "floorblock01"}, {'|', "floorblock02"}, {'a', "floorblock021"},
        {'s', "floorblock022"}, {'d', "floorblock023"}, {'f', "floorblock024"}, {'g', "floorblock025"},
        {'h', "floorblock026"}, {'1', "floorblock031"}, {'2', "floorblock032"}, {'3', "floorblock033"},
        {'4', "floorblock034"}, {'5', "floorblock035"}, {'6', "floorblock036"}, {'_', "floorblock03"},
        {'7', "floorblock041"}, {'8', "floorblock042"}, {'9', "floorblock043"}, {'0', "floorblock044"},
        {'-', "floorblock045"}, {'=', "floorblock046"}, {'u', "floorblock04"}
    };

    // Precompute starting position and scale factors to avoid redundant computations
    const float startX = startPos.x;
    const float startY = startPos.y;
    const float scaleX = scale.x;
    const float scaleY = scale.y;

    for (int y = 0; y < static_cast<int>(map.size()); ++y) {
        const float posY = startY + (y * scaleY);
        for (int x = 0; x < static_cast<int>(map[y].size()); ++x) {
            const char tile = map[y][x];
            const float posX = startX + (x * scaleX);

            // Update maximum coordinates
            if (posX > s_MaxCoordinates.x)
                s_MaxCoordinates.x = posX;
            if (posY > s_MaxCoordinates.y)
                s_MaxCoordinates.y = posY;

            // Find the floor type for the current tile and create the floor
            auto it = tileToFloorType.find(tile);
            if (it != tileToFloorType.end()) {
                CreateFloor(scene, posX, posY, scale, it->second);
            }
        }
    }
}


void SceneHelpers::LoadIngMapIntoScene(const LoadMap& loadMap, dae::Scene* scene, const glm::vec3& startPos,glm::vec2 scale)
{
    const auto& map = loadMap.getIngMap();

    for (int y = 0; y < static_cast<int>(map.size()); ++y) {
        for (int x = 0; x < static_cast<int>(map[y].size()); ++x) {
            const char tile = map[y][x];
            const float posX = startPos.x + x * scale.x;
            const float posY = startPos.y + y * scale.y;

            switch (tile) {
            case '^':
               CreateWalkThough(scene, posX, posY, glm::vec2(scale.x, scale.y), "topEnd");
                break;
            case 'L':
                CreateWalkThough(scene, posX, posY, glm::vec2(scale.x, scale.y), "bottomEnd");
                break;
            case 'M':
                CreateWalkThough(scene, posX, posY, glm::vec2(scale.x, scale.y), "leftEnd");
                break;
            case 'v':
                CreateWalkThough(scene, posX, posY, glm::vec2(scale.x, scale.y), "rightEnd");
                break;
            case 'C':
                CreateWalkThough(scene, posX, posY, glm::vec2(scale.x, scale.y), "walkTroughUp");
                break;
            case 'T':
                CreateWalkThough(scene, posX, posY, glm::vec2(scale.x, scale.y), "walkTroughLeft");
                break;
            case 'S':
				//SpawnSauge(scene, posX, posY, glm::vec2(scale.x, scale.y + 10));
                break;
            case 'E':
               // SpawnEgg(scene, posX, posY, glm::vec2(scale.x, scale.y + 10));
                break;
            case 'D':
               // SpawnPickle(scene, posX, posY, scale);
                break;
            case '0':
               // SpawnPlayer(scene, posX, posY, scale);
                playerCount += 1;
                break;
            case '1':
               // SpawnPlayerEnemy(scene, posX, posY, scale);
                break;
            default:
                break;
            }
        }
    }

    playerCount = 0;

}


glm::vec2 SceneHelpers::GetMaxCoordinates()
{
    return s_MaxCoordinates;
}
glm::vec2 SceneHelpers::GetMinCoordinates()
{
    return s_MinCoordinates;
}

glm::vec2 SceneHelpers::GetCellSize()
{
    return s_CellSize;
}
glm::vec2 SceneHelpers::GetGridSize()
{
    glm::vec2 maxCoord = GetMaxCoordinates();
    glm::vec2 minCoord = GetMinCoordinates();
    return {maxCoord.x - minCoord.x, maxCoord.y - minCoord.y};
}

std::vector<std::vector<char>> SceneHelpers::GetLoadMap()
{
    return s_loadMap;
}





