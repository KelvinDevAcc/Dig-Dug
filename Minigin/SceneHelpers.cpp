#include "SceneHelpers.h"
#include <iostream>

#include "AnimationComponent.h"
#include "GameData.h"
#include "HealthComponent.h"
#include "PointComponent.h"
#include "ResourceManager.h"
#include "SceneData.h"
#include "SpriteRendererComponent.h"
#include "TileComponent.h"
#include "../DigDug2/EnemyPlayer.h"
#include "../DigDug2/Player.h"
#include "../DigDug2/EnemyComponent.h"
#include "../DigDug2/Rock.h"
#include "../DigDug2/FygarComponent.h"

LoadMap* SceneHelpers::s_loadMap = nullptr;
std::vector<std::vector<char>> SceneHelpers::s_tunnelMap;
glm::vec2 SceneHelpers::s_MinCoordinates;
glm::vec2 SceneHelpers::s_MaxCoordinates;
glm::vec2 SceneHelpers::s_CellSize;
int SceneHelpers::playerCount = 0;
dae::Scene* SceneHelpers::m_scene = nullptr;

void CreateGameObject(dae::Scene* scene, const std::string& textureName, float x, float y, glm::vec2 scale, dae::GameObjectType type) {
    auto gameObject = std::make_unique<dae::GameObject>();
    auto spriteRenderer = std::make_unique<dae::SpriteRendererComponent>(gameObject.get(), dae::ResourceManager::GetTexture(dae::HashString(textureName)));
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
    auto spriteRenderer = std::make_unique<dae::SpriteRendererComponent>(gameObject.get(), dae::ResourceManager::GetTexture(dae::HashString(textureName)));
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

void SceneHelpers::SpawnPooka(dae::Scene* scene, float x, float y, glm::vec2 scale)
{
       auto PookaObject = std::make_unique<dae::GameObject>();
       auto spriterenderComponent2 = std::make_unique<dae::SpriteRendererComponent>(PookaObject.get(), dae::ResourceManager::GetSprite(dae::HashString("enemy")));
       spriterenderComponent2->SetDimensions(40, 40);
       PookaObject->AddComponent(std::move(spriterenderComponent2));

       auto animationComponent2 = std::make_unique<dae::AnimationComponent>(PookaObject.get(), PookaObject->GetComponent<dae::SpriteRendererComponent>(), "Normal");
       animationComponent2->Play("Walk_Down", true);
       PookaObject->AddComponent(std::move(animationComponent2));

       PookaObject->SetLocalPosition(glm::vec3(x, y, 1.0f));

       auto hitBox2 = std::make_unique<HitBox>(glm::vec2(scale));
       hitBox2->SetGameObject(PookaObject.get());
       PookaObject->AddComponent(std::move(hitBox2));

       auto pookaComponent = std::make_unique<EnemyComponent>(PookaObject.get());
       PookaObject->AddComponent(std::move(pookaComponent));

       dae::SceneData::GetInstance().AddGameObject(PookaObject.get(), dae::GameObjectType::enemy);
       scene->Add(std::move(PookaObject));
}

void SceneHelpers::SpawnFygar(dae::Scene* scene, float x, float y, glm::vec2 scale)
{
       auto fygarObject = std::make_unique<dae::GameObject>();
      auto spriterenderComponent3 = std::make_unique<dae::SpriteRendererComponent>(fygarObject.get(), dae::ResourceManager::GetSprite(dae::HashString("Fygar")));
      spriterenderComponent3->SetDimensions(40, 40);
      fygarObject->AddComponent(std::move(spriterenderComponent3));

      auto animationComponent3 = std::make_unique<dae::AnimationComponent>(fygarObject.get(), fygarObject->GetComponent<dae::SpriteRendererComponent>(), "Normal");
      animationComponent3->Play("Walk_Down", true);
      fygarObject->AddComponent(std::move(animationComponent3));

      fygarObject->SetLocalPosition(glm::vec3(x, y, 1.0f));

      auto hitBox3 = std::make_unique<HitBox>(glm::vec2(scale));
      hitBox3->SetGameObject(fygarObject.get());
      fygarObject->AddComponent(std::move(hitBox3));

      auto pookaComponent2 = std::make_unique<FygarComponent>(fygarObject.get());
      fygarObject->AddComponent(std::move(pookaComponent2));

      dae::SceneData::GetInstance().AddGameObject(fygarObject.get(), dae::GameObjectType::enemy);
      scene->Add(std::move(fygarObject));
}

void SceneHelpers::SpawnStone(dae::Scene* scene, float x, float y, glm::vec2 scale)
{
    auto RockObject = std::make_unique<dae::GameObject>();

    auto spriterenderComponent4 = std::make_unique<dae::SpriteRendererComponent>(RockObject.get(), dae::ResourceManager::GetSprite(dae::HashString("Rock")));
    spriterenderComponent4->SetDimensions(40, 40);
    RockObject->AddComponent(std::move(spriterenderComponent4));

    auto RockObjectanimationComponent = std::make_unique<dae::AnimationComponent>(RockObject.get(), RockObject->GetComponent<dae::SpriteRendererComponent>(), "Digging");
    RockObjectanimationComponent->Play("Idle", true);
    RockObject->AddComponent(std::move(RockObjectanimationComponent));
    RockObject->SetLocalPosition(glm::vec3(x, y, 1.0f));

    auto RockObjecthitBox = std::make_unique<HitBox>(glm::vec2(scale));
    RockObjecthitBox->SetGameObject(RockObject.get());
    RockObject->AddComponent(std::move(RockObjecthitBox));

    auto RockComponnet = std::make_unique<Rock>(RockObject.get());
    RockObject->AddComponent(std::move(RockComponnet));
    dae::SceneData::GetInstance().AddGameObject(RockObject.get(), dae::GameObjectType::Rock);

    scene->Add(std::move(RockObject));
}


void SceneHelpers::SpawnPlayer(dae::Scene* scene, float x, float y, glm::vec2 scale)
{
    int score = 0;
    int lives = 3;

    // Check if the player already exists in GameData and retrieve existing data
    PlayerData existingData = GameData::GetInstance().GetPlayerData(playerCount);

    if (existingData.score != 0)
        score = existingData.score;

    if (existingData.lives != 3)
        lives = existingData.lives;

    // Create the Player GameObject
    auto PlayerObject = std::make_unique<dae::GameObject>();

    auto Character1points = std::make_unique<dae::PointComponent>(score);
    PlayerObject->AddComponent(std::move(Character1points));

    auto Character1Health = std::make_unique<dae::HealthComponent>(100, lives);
    PlayerObject->AddComponent(std::move(Character1Health));

    auto spriteRenderComponent = std::make_unique<dae::SpriteRendererComponent>(PlayerObject.get(), dae::ResourceManager::GetSprite(dae::HashString("Player")));
    spriteRenderComponent->SetDimensions(40, 40);
    PlayerObject->AddComponent(std::move(spriteRenderComponent));

    auto animationComponent = std::make_unique<dae::AnimationComponent>(PlayerObject.get(), PlayerObject->GetComponent<dae::SpriteRendererComponent>(), "Idle");
    PlayerObject->AddComponent(std::move(animationComponent));
    PlayerObject->SetLocalPosition(glm::vec3(x, y, 1.0f));

    auto hitBox = std::make_unique<HitBox>(glm::vec2(scale));
    hitBox->SetGameObject(PlayerObject.get());
    PlayerObject->AddComponent(std::move(hitBox));

    auto PlayerComponent = std::make_unique<game::Player>(PlayerObject.get());
    PlayerComponent->SetPlayerID(playerCount);
    PlayerObject->AddComponent(std::move(PlayerComponent));


    // Add the GameObject to the scene
    dae::SceneData::GetInstance().AddGameObject(PlayerObject.get(), dae::GameObjectType::Player);
    scene->Add(std::move(PlayerObject));

    // Create and add PlayerData to GameData
    PlayerData playerData;
    playerData.score = score;
    playerData.lives = lives;
    GameData::GetInstance().AddPlayer(playerCount, playerData);

    // Increment playerCount if necessary
    playerCount++;
}

void SceneHelpers::SpawnPlayerEnemy(dae::Scene* scene, float x, float y, glm::vec2 scale)
{
    auto PlayerObject = std::make_unique<dae::GameObject>();

    auto Character1Health = std::make_unique<dae::HealthComponent>(100, 3);
    PlayerObject->AddComponent(std::move(Character1Health));

    auto Character1points = std::make_unique<dae::PointComponent>(0);
    PlayerObject->AddComponent(std::move(Character1points));

    auto spriterenderComponent = std::make_unique<dae::SpriteRendererComponent>(PlayerObject.get(), dae::ResourceManager::GetSprite(dae::HashString("Fygar")));
    spriterenderComponent->SetDimensions(40, 40);
    PlayerObject->AddComponent(std::move(spriterenderComponent));

    auto animationComponent = std::make_unique<dae::AnimationComponent>(PlayerObject.get(), PlayerObject->GetComponent<dae::SpriteRendererComponent>(), "Normal");
    animationComponent->Play("Walk_Right", true);
    PlayerObject->AddComponent(std::move(animationComponent));
    PlayerObject->SetLocalPosition(glm::vec3(x, y, 1.0f));

    auto hitBox = std::make_unique<HitBox>(scale);
    hitBox->SetGameObject(PlayerObject.get());
    PlayerObject->AddComponent(std::move(hitBox));

    auto PlayerComponent = std::make_unique<game::EnemyPlayer>(PlayerObject.get());
    PlayerObject->AddComponent(std::move(PlayerComponent));

    dae::SceneData::GetInstance().AddGameObject(PlayerObject.get(), dae::GameObjectType::enemyPlayers);
    dae::SceneData::GetInstance().AddGameObject(PlayerObject.get(), dae::GameObjectType::enemy);

    scene->Add(std::move(PlayerObject));
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

        // New mappings
     /*   ,{'i', "2skyFloor"}, {'j', "2floorblock01"}, {'k', "2floorblock02"},
        {'l', "2floorblock021"}, {'m', "2floorblock022"}, {'n', "2floorblock023"},
        {'o', "2floorblock024"}, {'p', "2floorblock025"}, {'q', "2floorblock026"},
        {'r', "2floorblock03"}, {'t', "2floorblock031"}, {'y', "2floorblock032"},
        {'u', "2floorblock033"}, {'v', "2floorblock034"}, {'w', "2floorblock035"},
        {'x', "2floorblock036"}, {'z', "2floorblock04"}, {'A', "2floorblock041"},
        {'B', "2floorblock042"}, {'C', "2floorblock043"}, {'D', "2floorblock044"},
        {'E', "2floorblock045"}, {'F', "2floorblock046"}*/
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
    const auto& map = loadMap.getTunMap();
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
}

void SceneHelpers::LoadEntitysMapIntoScene(const LoadMap& loadMap, dae::Scene* scene, const glm::vec3& startPos, glm::vec2 scale) {
    const auto& map = loadMap.getEntityMap();
    s_tunnelMap = map;
    m_scene = scene;
    for (int y = 0; y < static_cast<int>(map.size()); ++y) {
        for (int x = 0; x < static_cast<int>(map[y].size()); ++x) {
            const char tile = map[y][x];
            const float posX = startPos.x + x * scale.x;
            const float posY = startPos.y + y * scale.y;

            switch (tile) {
            case 'S':
                SpawnStone(scene, posX, posY, glm::vec2(scale.x, scale.y));
                break;
            case 'F':
                SpawnFygar(scene, posX, posY, glm::vec2(scale.x, scale.y));
                break;
            case 'P':
                SpawnPooka(scene, posX, posY, glm::vec2(scale.x, scale.y));
                break;
            case 'C':
                SpawnPlayer(scene, posX, posY, glm::vec2(scale.x, scale.y));
                break;
            case 'E':
                SpawnPlayerEnemy(scene, posX, posY, glm::vec2(scale.x, scale.y));
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

glm::vec3 SceneHelpers::GetCenterOfTile(const glm::vec3& position) {
    // Calculate the grid coordinates based on the position
    const int gridX = static_cast<int>((position.x - GetMinCoordinates().x) / GetCellSize().x);
    const int gridY = static_cast<int>((position.y - GetMinCoordinates().y) / GetCellSize().y);

    // Calculate the tile's top-left corner in world space
    const float tilePosX = GetMinCoordinates().x + gridX * GetCellSize().x;
    const float tilePosY = GetMinCoordinates().y + gridY * GetCellSize().y;

    // Calculate the center of the tile
    //const float centerX = tilePosX + GetCellSize().x / 2.0f;
    //const float centerY = tilePosY + GetCellSize().y / 2.0f;

    // Return the center position in 3D space (z is unchanged)
    return glm::vec3(tilePosX, tilePosY, position.z);
}
