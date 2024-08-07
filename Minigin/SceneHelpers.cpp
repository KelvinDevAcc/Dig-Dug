#include "SceneHelpers.h"
#include <iostream>
#include "ResourceManager.h"
#include "SceneData.h"
#include "SpriteRendererComponent.h"
#include "TileComponent.h"

LoadMap* SceneHelpers::s_loadMap = nullptr;
std::vector<std::vector<char>> SceneHelpers::s_tunnelMap;
glm::vec2 SceneHelpers::s_MinCoordinates;
glm::vec2 SceneHelpers::s_MaxCoordinates;
glm::vec2 SceneHelpers::s_CellSize;
int SceneHelpers::playerCount = 0;
dae::Scene* SceneHelpers::m_scene = nullptr;

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

void CreateTile(dae::Scene* scene, const std::string& textureName, float x, float y, glm::vec2 scale, dae::GameObjectType type, TunnelType tunnelType) {
    auto gameObject = std::make_unique<dae::GameObject>();

    // Add sprite renderer component
    auto spriteRenderer = std::make_unique<dae::SpriteRendererComponent>(gameObject.get(), dae::ResourceManager::GetTexture(textureName));
    spriteRenderer->SetDimensions(scale.x, scale.y);
    gameObject->AddComponent(std::move(spriteRenderer));
    gameObject->SetLocalPosition(glm::vec3(x, y, 0.0f));

    // Add hitbox component
    auto hitBox = std::make_unique<HitBox>(scale);
    hitBox->SetGameObject(gameObject.get());
    gameObject->AddComponent(std::move(hitBox));

    // Add tile component
    auto tile = std::make_unique<TileComponent>(gameObject.get(), tunnelType);
    gameObject->AddComponent(std::move(tile));

    // Add game object to the scene
    dae::SceneData::GetInstance().AddGameObject(gameObject.get(), type);
    scene->Add(std::move(gameObject));
}

void SceneHelpers::CreateFloor(dae::Scene* scene, float x, float y, glm::vec2 scale, const std::string& textureName) {
    CreateGameObject(scene, textureName, x, y, scale, dae::GameObjectType::Floor);
}

void SceneHelpers::CreateWalkThough(dae::Scene* scene, float x, float y, glm::vec2 scale,
	const std::string& textureName, char tileChar)
{
    TunnelType tunnelType;
    switch (tileChar) {
    case '^': tunnelType = TunnelType::topEnd; break;
    case 'v': tunnelType = TunnelType::bottomEnd; break;
    case '<': tunnelType = TunnelType::leftEnd; break;
    case '>': tunnelType = TunnelType::rightEnd; break;
    case ':': tunnelType = TunnelType::walkThroughUp; break;
    case '=': tunnelType = TunnelType::walkThroughLeft; break;
    case 'M': tunnelType = TunnelType::MiddleBlock; break;
    default: tunnelType = TunnelType::Empty; break;
    }
    CreateTile(scene, textureName, x, y, scale, dae::GameObjectType::WalkThrough, tunnelType);
}

void SceneHelpers::CreateEmpty(dae::Scene* scene, float x, float y, glm::vec2 scale, const std::string& textureName) {
    CreateGameObject(scene, textureName, x, y, scale, dae::GameObjectType::Empty);
}

void SceneHelpers::LoadMapIntoScene(const LoadMap& loadMap, dae::Scene* scene, const glm::vec3& startPos, glm::vec2 scale) {
    const auto& map = loadMap.getMap();
    s_loadMap = const_cast<LoadMap*>(&loadMap);

    s_MinCoordinates = { startPos.x, startPos.y };
    s_MaxCoordinates = { startPos.x, startPos.y };
    s_CellSize = scale;

    const std::unordered_map<char, std::string> tileToTextureName = {
        {'v', "sky"}, {'#', "skyFloor"}, {'^', "floorblock01"}, {'|', "floorblock02"},
        {'a', "floorblock021"}, {'s', "floorblock022"}, {'d', "floorblock023"},
        {'f', "floorblock024"}, {'g', "floorblock025"}, {'h', "floorblock026"},
        {'1', "floorblock031"}, {'2', "floorblock032"}, {'3', "floorblock033"},
        {'4', "floorblock034"}, {'5', "floorblock035"}, {'6', "floorblock036"},
        {'_', "floorblock03"}, {'7', "floorblock041"}, {'8', "floorblock042"},
        {'9', "floorblock043"}, {'0', "floorblock044"}, {'-', "floorblock045"},
        {'=', "floorblock046"}, {'u', "floorblock04"}
    };

    const float startX = startPos.x;
    const float startY = startPos.y;
    const float scaleX = scale.x;
    const float scaleY = scale.y;

    for (int y = 0; y < static_cast<int>(map.size()); ++y) {
        const float posY = startY + (y * scaleY);
        for (int x = 0; x < static_cast<int>(map[y].size()); ++x) {
            const char tile = map[y][x];
            const float posX = startX + (x * scaleX);

            if (posX > s_MaxCoordinates.x)
                s_MaxCoordinates.x = posX;
            if (posY > s_MaxCoordinates.y)
                s_MaxCoordinates.y = posY;

            auto it = tileToTextureName.find(tile);
            if (it != tileToTextureName.end()) {
                const std::string& textureName = it->second;

                if (textureName == "sky") {
                    CreateEmpty(scene, posX, posY, scale, textureName);
                    continue;
                }

                CreateFloor(scene, posX, posY, scale, textureName);
            }
        }
    }
}

void SceneHelpers::LoadTunnelMapIntoScene(const LoadMap& loadMap, dae::Scene* scene, const glm::vec3& startPos, glm::vec2 scale) {
    const auto& map = loadMap.getIngMap();
    s_tunnelMap = map;
    m_scene = scene;
    for (int y = 0; y < static_cast<int>(map.size()); ++y) {
        for (int x = 0; x < static_cast<int>(map[y].size()); ++x) {
            const char tile = map[y][x];
            const float posX = startPos.x + x * scale.x;
            const float posY = startPos.y + y * scale.y;

            switch (tile) {
            case '^':
            case 'v':
            case '<':
            case '>':
            case ':':
            case '=':
            case 'M':
                CreateWalkThough(scene, posX, posY, glm::vec2(scale.x, scale.y), "", tile);
                break;
            default:
                break;
            }
        }
    }

    playerCount = 0;
}

glm::vec2 SceneHelpers::GetMaxCoordinates() {
    return s_MaxCoordinates;
}

glm::vec2 SceneHelpers::GetMinCoordinates() {
    return s_MinCoordinates;
}

glm::vec2 SceneHelpers::GetCellSize() {
    return s_CellSize;
}

glm::vec2 SceneHelpers::GetGridSize() {
    glm::vec2 maxCoord = GetMaxCoordinates();
    glm::vec2 minCoord = GetMinCoordinates();
    return { maxCoord.x - minCoord.x, maxCoord.y - minCoord.y };
}

TunnelType SceneHelpers::GetTileTypeAtPosition(const glm::vec3& position) {
    static const float mapOffsetX = GetMinCoordinates().x;
    static const float mapOffsetY = GetMinCoordinates().y;
    static const glm::vec2 cellSize = GetCellSize();

    const int gridX = static_cast<int>((position.x - mapOffsetX) / cellSize.x);
    const int gridY = static_cast<int>((position.y - mapOffsetY) / cellSize.y);

    if (gridY < 0 || gridY >= static_cast<int>(s_tunnelMap.size()) || gridX < 0 || gridX >= static_cast<int>(s_tunnelMap[gridY].size())) {
        return TunnelType::Empty;
    }

    static const std::unordered_map<char, TunnelType> charToTunnelType = {
        {'v', TunnelType::bottomEnd}, {'^', TunnelType::topEnd}, {'<', TunnelType::leftEnd}, {'>', TunnelType::rightEnd},
        {':', TunnelType::walkThroughUp}, {'=', TunnelType::walkThroughLeft}, {'_', TunnelType::Empty}, {'s', TunnelType::sky},
        {'Y', TunnelType::UpRight}, {'P', TunnelType::UpLeft}, {'R', TunnelType::BottomRight}, {'L', TunnelType::BottomLeft},
        {'M', TunnelType::MiddleBlock}
    };

    const char tileChar = s_tunnelMap[gridY][gridX];
    const auto it = charToTunnelType.find(tileChar);
    return it != charToTunnelType.end() ? it->second : TunnelType::Empty;
}

void SceneHelpers::SetTileTypeAtPosition(const glm::vec3& position, TunnelType newType) {
    const int gridX = static_cast<int>((position.x - GetMinCoordinates().x) / GetCellSize().x);
    const int gridY = static_cast<int>((position.y - GetMinCoordinates().y) / GetCellSize().y);

    if (gridY < 0 || gridY >= static_cast<int>(s_tunnelMap.size()) || gridX < 0 || gridX >= static_cast<int>(s_tunnelMap[gridY].size())) {
        return;
    }

    if (!m_scene) {
        return;
    }

    s_tunnelMap[gridY][gridX] = '_';
    dae::SceneData::GetInstance().RemoveGameObjectAtPosition(glm::vec3(300 + gridX * GetCellSize().x, 20 + gridY * GetCellSize().y, 0.0f));

    const std::unordered_map<TunnelType, std::pair<char, std::string>> tunnelTypeToTile = {
        { TunnelType::Empty, {'_', ""} },
        { TunnelType::bottomEnd, {'v', "bottomEnd"} },
        { TunnelType::topEnd, {'^', "topEnd"} },
        { TunnelType::walkThroughUp, {':', "walkTroughUp"} },
        { TunnelType::walkThroughLeft, {'=', "walkTroughLeft"} },
        { TunnelType::leftEnd, {'<', "leftEnd"} },
        { TunnelType::rightEnd, {'>', "rightEnd"} },
        { TunnelType::UpRight, {'Y', "UpRight"} },
        { TunnelType::UpLeft, {'P', "UpLeft"} },
        { TunnelType::BottomRight, {'R', "BottomRight"} },
        { TunnelType::BottomLeft, {'L', "BottomLeft"} },
        { TunnelType::MiddleBlock, {'M', "MiddleBlock"} }
    };

    const auto it = tunnelTypeToTile.find(newType);
    if (it != tunnelTypeToTile.end()) {
        s_tunnelMap[gridY][gridX] = it->second.first;
        if (!it->second.second.empty()) {
            CreateWalkThough(m_scene, 300 + gridX * GetCellSize().x, 20 + gridY * GetCellSize().y, glm::vec2(GetCellSize().x, GetCellSize().y), it->second.second, it->second.first);
        }
    }
}
