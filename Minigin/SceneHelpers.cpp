#include "SceneHelpers.h"
#include "BurgerComponent.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "SceneData.h"
#include "SpriteRendererComponent.h"
#include "../BugerTime/EnemyComponent.h"
#include "../BugerTime/GameData.h"
#include "../BugerTime/Player.h"
#include "../BugerTime/MrHotDogAIComponent.h"
#include "../BugerTime/MrPickleAIComponent.h"
#include "../BugerTime/Pepper.h"

glm::vec2 s_MinCoordinates;
glm::vec2 s_MaxCoordinates; 
glm::vec2 s_CellSize;
int playerCount;
std::vector<std::vector<char>> s_loadMap;

void SceneHelpers::CreateFloor(dae::Scene* scene, float x, float y, glm::vec2 scale) {
    auto floorObject = std::make_unique<dae::GameObject>();
    auto spriteRenderer = std::make_unique<dae::SpriteRendererComponent>(floorObject.get(), dae::ResourceManager::GetSprite("floor"));
    spriteRenderer->SetDimensions(scale.x, scale.y);
    floorObject->AddComponent(std::move(spriteRenderer));
    floorObject->SetLocalPosition(glm::vec3(x, y, 0.0f));

    auto hitBox = std::make_unique<HitBox>(scale);
    hitBox->SetGameObject(floorObject.get());
    floorObject->AddComponent(std::move(hitBox));

    dae::SceneData::GetInstance().AddGameObject(floorObject.get(), dae::GameObjectType::Floor);


    scene->Add(std::move(floorObject));
}

void SceneHelpers::CreateLadderUp(dae::Scene* scene, float x, float y, glm::vec2 scale) {
    auto ladderObject = std::make_unique<dae::GameObject>();
    auto spriteRenderer = std::make_unique<dae::SpriteRendererComponent>(ladderObject.get(), dae::ResourceManager::GetSprite("ladder_up"));
    spriteRenderer->SetDimensions(scale.x, scale.y);
    ladderObject->AddComponent(std::move(spriteRenderer));
    ladderObject->SetLocalPosition(glm::vec3(x, y, 0.0f));

    auto hitBox = std::make_unique<HitBox>(scale);
    hitBox->SetGameObject(ladderObject.get());
    ladderObject->AddComponent(std::move(hitBox));

    dae::SceneData::GetInstance().AddGameObject(ladderObject.get(), dae::GameObjectType::LadderUp);

    scene->Add(std::move(ladderObject));
}

void SceneHelpers::CreateLadderUpDown(dae::Scene* scene, float x, float y, glm::vec2 scale) {
    auto ladderObject = std::make_unique<dae::GameObject>();
    auto spriteRenderer = std::make_unique<dae::SpriteRendererComponent>(ladderObject.get(), dae::ResourceManager::GetSprite("ladder_up"));
    spriteRenderer->SetDimensions(scale.x, scale.y);
    ladderObject->AddComponent(std::move(spriteRenderer));
    ladderObject->SetLocalPosition(glm::vec3(x, y, 0.0f));

    auto hitBox = std::make_unique<HitBox>(scale);
    hitBox->SetGameObject(ladderObject.get());
    ladderObject->AddComponent(std::move(hitBox));

    dae::SceneData::GetInstance().AddGameObject(ladderObject.get(), dae::GameObjectType::LadderUpDown);

    scene->Add(std::move(ladderObject));
}


void SceneHelpers::CreateLadderDown(dae::Scene* scene, float x, float y, glm::vec2 scale) {
    auto ladderObject = std::make_unique<dae::GameObject>();
    auto spriteRenderer = std::make_unique<dae::SpriteRendererComponent>(ladderObject.get(), dae::ResourceManager::GetSprite("ladder_down"));
    spriteRenderer->SetDimensions(scale.x, scale.y);
    ladderObject->AddComponent(std::move(spriteRenderer));
    ladderObject->SetLocalPosition(glm::vec3(x, y, 0.0f));

    auto hitBox = std::make_unique<HitBox>(scale);
    hitBox->SetGameObject(ladderObject.get());
    ladderObject->AddComponent(std::move(hitBox));

    dae::SceneData::GetInstance().AddGameObject(ladderObject.get(), dae::GameObjectType::LadderDown);

    scene->Add(std::move(ladderObject));
}

void SceneHelpers::CreateSolidLadder(dae::Scene* scene, float x, float y, glm::vec2 scale) {
    auto solidLadderObject = std::make_unique<dae::GameObject>();
    auto spriteRenderer = std::make_unique<dae::SpriteRendererComponent>(solidLadderObject.get(), dae::ResourceManager::GetSprite("stairs"));
    spriteRenderer->SetDimensions(scale.x, scale.y);
    solidLadderObject->AddComponent(std::move(spriteRenderer));
    solidLadderObject->SetLocalPosition(glm::vec3(x, y, 0.0f));

    auto hitBox = std::make_unique<HitBox>(scale);
    hitBox->SetGameObject(solidLadderObject.get());
    solidLadderObject->AddComponent(std::move(hitBox));

    dae::SceneData::GetInstance().AddGameObject(solidLadderObject.get(), dae::GameObjectType::SolidLadder);

    scene->Add(std::move(solidLadderObject));
}



void SceneHelpers::CreatePlatformLeft(dae::Scene* scene, float x, float y, glm::vec2 scale) {
    // Create the left part of the platform
    auto leftObject = std::make_unique<dae::GameObject>();
    auto leftSpriteRenderer = std::make_unique<dae::SpriteRendererComponent>(leftObject.get(), dae::ResourceManager::GetSprite("left_basket"));
    leftSpriteRenderer->SetDimensions(scale.x, scale.y);
    leftObject->AddComponent(std::move(leftSpriteRenderer));
    leftObject->SetLocalPosition(glm::vec3(x, y, 0.0f));

    auto hitBox = std::make_unique<HitBox>(scale);
    hitBox->SetGameObject(leftObject.get());
    leftObject->AddComponent(std::move(hitBox));

    dae::SceneData::GetInstance().AddGameObject(leftObject.get(), dae::GameObjectType::Basket);

    scene->Add(std::move(leftObject));
}

void SceneHelpers::CreatePlatformCombined(dae::Scene* scene, float x, float y, glm::vec2 scale) {
    // Create the left part of the platform
    auto combinedObject = std::make_unique<dae::GameObject>();
    auto leftSpriteRenderer = std::make_unique<dae::SpriteRendererComponent>(combinedObject.get(), dae::ResourceManager::GetSprite("combined_basket"));
    leftSpriteRenderer->SetDimensions(scale.x, scale.y);
    combinedObject->AddComponent(std::move(leftSpriteRenderer));
    combinedObject->SetLocalPosition(glm::vec3(x, y, 0.0f));

    auto hitBox = std::make_unique<HitBox>(scale);
    hitBox->SetGameObject(combinedObject.get());
    combinedObject->AddComponent(std::move(hitBox));

    dae::SceneData::GetInstance().AddGameObject(combinedObject.get(), dae::GameObjectType::Basket);

    scene->Add(std::move(combinedObject));
}

void SceneHelpers::CreatePlatformMiddle(dae::Scene* scene, float x, float y, glm::vec2 scale) {

    // Create the middle part of the platform
    auto midObject = std::make_unique<dae::GameObject>();
    auto midSpriteRenderer = std::make_unique<dae::SpriteRendererComponent>(midObject.get(), dae::ResourceManager::GetSprite("mid_basket"));
    midSpriteRenderer->SetDimensions(scale.x, scale.y);
    midObject->AddComponent(std::move(midSpriteRenderer));
    midObject->SetLocalPosition(glm::vec3(x, y, 0.0f));

    auto hitBox = std::make_unique<HitBox>(scale);
    hitBox->SetGameObject(midObject.get());
    midObject->AddComponent(std::move(hitBox));

    dae::SceneData::GetInstance().AddGameObject(midObject.get(), dae::GameObjectType::Basket);

    scene->Add(std::move(midObject));
}

void SceneHelpers::CreatePlatformRight(dae::Scene* scene, float x, float y, glm::vec2 scale) {

    // Create the right part of the platform
    auto rightObject = std::make_unique<dae::GameObject>();
    auto rightSpriteRenderer = std::make_unique<dae::SpriteRendererComponent>(rightObject.get(), dae::ResourceManager::GetSprite("right_basket"));
    rightSpriteRenderer->SetDimensions(scale.x, scale.y);
    rightObject->AddComponent(std::move(rightSpriteRenderer));
    rightObject->SetLocalPosition(glm::vec3(x, y, 0.0f));

    auto hitBox = std::make_unique<HitBox>(scale);
    hitBox->SetGameObject(rightObject.get());
    rightObject->AddComponent(std::move(hitBox));

    dae::SceneData::GetInstance().AddGameObject(rightObject.get(), dae::GameObjectType::Basket);

    scene->Add(std::move(rightObject));
}


void SceneHelpers::CreateBurgerTop(dae::Scene* scene, float x, float y, glm::vec2 scale)
{
    auto BurgerTopObject = std::make_unique<dae::GameObject>();
    auto spriteRenderer = std::make_unique<dae::SpriteRendererComponent>(BurgerTopObject.get(), dae::ResourceManager::GetSprite("Bugertop"));
	
	spriteRenderer->SetDimensions(scale.x, scale.y);
    BurgerTopObject->AddComponent(std::move(spriteRenderer));
    BurgerTopObject->SetLocalPosition(glm::vec3(x, y, 0.0f));

    // Create and set up the BurgerComponent
    auto burgerComponent = std::make_unique<BurgerComponent>(scale.x, 50.f);
    burgerComponent->SetGameObject(BurgerTopObject.get());
    BurgerTopObject->AddComponent(std::move(burgerComponent));

    auto hitBox = std::make_unique<HitBox>(glm::vec2(scale.x - 10, scale.y));
    hitBox->SetGameObject(BurgerTopObject.get());
    BurgerTopObject->AddComponent(std::move(hitBox));

    dae::SceneData::GetInstance().AddGameObject(BurgerTopObject.get(), dae::GameObjectType::BurgerPart);

    scene->Add(std::move(BurgerTopObject));
}

void SceneHelpers::CreateCheese(dae::Scene* scene, float x, float y, glm::vec2 scale)
{
    auto CheeseObject = std::make_unique<dae::GameObject>();
    auto spriteRenderer = std::make_unique<dae::SpriteRendererComponent>(CheeseObject.get(), dae::ResourceManager::GetSprite("cheese"));
    spriteRenderer->SetDimensions(scale.x, scale.y);
    CheeseObject->AddComponent(std::move(spriteRenderer));
    CheeseObject->SetLocalPosition(glm::vec3(x, y, 0.0f));

    // Create and set up the BurgerComponent
    auto burgerComponent = std::make_unique<BurgerComponent>(scale.x, 50.f);
    burgerComponent->SetGameObject(CheeseObject.get());
    CheeseObject->AddComponent(std::move(burgerComponent));

    auto hitBox = std::make_unique<HitBox>(glm::vec2(scale.x - 10, scale.y));
    hitBox->SetGameObject(CheeseObject.get());
    CheeseObject->AddComponent(std::move(hitBox));

    dae::SceneData::GetInstance().AddGameObject(CheeseObject.get(), dae::GameObjectType::BurgerPart);

    scene->Add(std::move(CheeseObject));
}

void SceneHelpers::CreateMeat(dae::Scene* scene, float x, float y, glm::vec2 scale)
{
    auto MeatObject = std::make_unique<dae::GameObject>();
    auto spriteRenderer = std::make_unique<dae::SpriteRendererComponent>(MeatObject.get(), dae::ResourceManager::GetSprite("Meat"));
    spriteRenderer->SetDimensions(scale.x, scale.y);
    MeatObject->AddComponent(std::move(spriteRenderer));
    MeatObject->SetLocalPosition(glm::vec3(x, y, 0.0f));


    // Create and set up the BurgerComponent
    auto burgerComponent = std::make_unique<BurgerComponent>(scale.x, 50.f);
    burgerComponent->SetGameObject(MeatObject.get());
    MeatObject->AddComponent(std::move(burgerComponent));


    auto hitBox = std::make_unique<HitBox>(glm::vec2(scale.x - 10, scale.y));
    hitBox->SetGameObject(MeatObject.get());
    MeatObject->AddComponent(std::move(hitBox));

    dae::SceneData::GetInstance().AddGameObject(MeatObject.get(), dae::GameObjectType::BurgerPart);

    scene->Add(std::move(MeatObject));
}

void SceneHelpers::CreateTomato(dae::Scene* scene, float x, float y, glm::vec2 scale)
{
    auto TomatoObject = std::make_unique<dae::GameObject>();
    auto spriteRenderer = std::make_unique<dae::SpriteRendererComponent>(TomatoObject.get(), dae::ResourceManager::GetSprite("tomato"));
    spriteRenderer->SetDimensions(scale.x, scale.y);
    TomatoObject->AddComponent(std::move(spriteRenderer));
    TomatoObject->SetLocalPosition(glm::vec3(x, y, 0.0f));

    // Create and set up the BurgerComponent
    auto burgerComponent = std::make_unique<BurgerComponent>(scale.x, 50.f);
    burgerComponent->SetGameObject(TomatoObject.get());
    TomatoObject->AddComponent(std::move(burgerComponent));

    auto hitBox = std::make_unique<HitBox>(glm::vec2(scale.x - 10, scale.y));
    hitBox->SetGameObject(TomatoObject.get());
    TomatoObject->AddComponent(std::move(hitBox));

    dae::SceneData::GetInstance().AddGameObject(TomatoObject.get(), dae::GameObjectType::BurgerPart);

    scene->Add(std::move(TomatoObject));
}

void SceneHelpers::CreateLettuce(dae::Scene* scene, float x, float y, glm::vec2 scale)
{
    auto LettuceObject = std::make_unique<dae::GameObject>();
    auto spriteRenderer = std::make_unique<dae::SpriteRendererComponent>(LettuceObject.get(), dae::ResourceManager::GetSprite("lettuce"));
    spriteRenderer->SetDimensions(scale.x, scale.y);
    LettuceObject->AddComponent(std::move(spriteRenderer));
    LettuceObject->SetLocalPosition(glm::vec3(x, y, 0.0f));

    // Create and set up the BurgerComponent
    auto burgerComponent = std::make_unique<BurgerComponent>(scale.x, 50.f);
    burgerComponent->SetGameObject(LettuceObject.get());
    LettuceObject->AddComponent(std::move(burgerComponent));

    auto hitBox = std::make_unique<HitBox>(glm::vec2(scale.x - 10, scale.y));
    hitBox->SetGameObject(LettuceObject.get());
    LettuceObject->AddComponent(std::move(hitBox));

    dae::SceneData::GetInstance().AddGameObject(LettuceObject.get(), dae::GameObjectType::BurgerPart);

    scene->Add(std::move(LettuceObject));
}

void SceneHelpers::CreateBurgerBottom(dae::Scene* scene, float x, float y, glm::vec2 scale)
{
    // Create the GameObject for the burger bottom
    auto BurgerBottomObject = std::make_unique<dae::GameObject>();

    // Create and set up the SpriteRendererComponent
    auto spriteRenderer = std::make_unique<dae::SpriteRendererComponent>(BurgerBottomObject.get(), dae::ResourceManager::GetSprite("Buger_bottom"));
    spriteRenderer->SetDimensions(scale.x, scale.y);
    BurgerBottomObject->AddComponent(std::move(spriteRenderer));

    // Set the local position of the GameObject
    BurgerBottomObject->SetLocalPosition(glm::vec3(x, y, 0.0f));

    // Create and set up the BurgerComponent
     auto burgerComponent = std::make_unique<BurgerComponent>(scale.x, 50.f);
     burgerComponent->SetGameObject(BurgerBottomObject.get());
     BurgerBottomObject->AddComponent(std::move(burgerComponent));


    // Create and set up the HitBox
    auto hitBox = std::make_unique<HitBox>(glm::vec2(scale.x - 10, scale.y));
    hitBox->SetGameObject(BurgerBottomObject.get());
    BurgerBottomObject->AddComponent(std::move(hitBox));

    // Add the GameObject to the SceneData
    dae::SceneData::GetInstance().AddGameObject(BurgerBottomObject.get(), dae::GameObjectType::BurgerPart);

    // Add the GameObject to the scene
    scene->Add(std::move(BurgerBottomObject));
}

void SceneHelpers::SpawnSauge(dae::Scene* scene, float x, float y, glm::vec2 scale)
{
    auto SaugeObject = std::make_unique<dae::GameObject>();

    auto spriterenderComponent = std::make_unique<dae::SpriteRendererComponent>(SaugeObject.get(), dae::ResourceManager::GetSprite("sausage"));
    spriterenderComponent->SetDimensions(40, 40);
    SaugeObject->AddComponent(std::move(spriterenderComponent));

    auto animationComponent = std::make_unique<dae::AnimationComponent>(SaugeObject.get(), SaugeObject->GetComponent<dae::SpriteRendererComponent>(), "Idle");
    animationComponent->Play("Stunned", true);
	SaugeObject->AddComponent(std::move(animationComponent));
    SaugeObject->SetLocalPosition(glm::vec3(x, y, 0.0f));

    auto hitBox = std::make_unique<HitBox>(scale);
    hitBox->SetGameObject(SaugeObject.get());
    SaugeObject->AddComponent(std::move(hitBox));

    dae::SceneData::GetInstance().AddGameObject(SaugeObject.get(), dae::GameObjectType::enemy);


    auto aiComponent = std::make_unique<MrHotDogAIComponent>(SaugeObject.get(), s_loadMap);
    SaugeObject->AddComponent(std::move(aiComponent));

    auto enemyComponent = std::make_unique<game::EnemyComponent>(SaugeObject.get(),scene);
    SaugeObject->AddComponent(std::move(enemyComponent));


    scene->Add(std::move(SaugeObject));
}

void SceneHelpers::SpawnEgg(dae::Scene* scene, float x, float y, glm::vec2 scale)
{
    auto EggObject = std::make_unique<dae::GameObject>();

    auto spriterenderComponent = std::make_unique<dae::SpriteRendererComponent>(EggObject.get(), dae::ResourceManager::GetSprite("egg"));
    spriterenderComponent->SetDimensions(40, 40);
    EggObject->AddComponent(std::move(spriterenderComponent));

    auto animationComponent = std::make_unique<dae::AnimationComponent>(EggObject.get(), EggObject->GetComponent<dae::SpriteRendererComponent>(), "Idle");
    animationComponent->Play("Stunned", true);
    EggObject->AddComponent(std::move(animationComponent));
    EggObject->SetLocalPosition(glm::vec3(x, y, 0.0f));

    auto hitBox = std::make_unique<HitBox>(scale);
    hitBox->SetGameObject(EggObject.get());
    EggObject->AddComponent(std::move(hitBox));

    dae::SceneData::GetInstance().AddGameObject(EggObject.get(), dae::GameObjectType::enemy);

    auto aiComponent = std::make_unique<MrHotDogAIComponent>(EggObject.get(), s_loadMap);
    EggObject->AddComponent(std::move(aiComponent));

    auto enemyComponent = std::make_unique<game::EnemyComponent>(EggObject.get(), scene);
    EggObject->AddComponent(std::move(enemyComponent));


    scene->Add(std::move(EggObject));
}

void SceneHelpers::SpawnPickle(dae::Scene* scene, float x, float y, glm::vec2 scale)
{
    auto PickleObject = std::make_unique<dae::GameObject>();

    auto spriterenderComponent = std::make_unique<dae::SpriteRendererComponent>(PickleObject.get(), dae::ResourceManager::GetSprite("Pickle"));
    spriterenderComponent->SetDimensions(40, 40);
    PickleObject->AddComponent(std::move(spriterenderComponent));

    auto animationComponent = std::make_unique<dae::AnimationComponent>(PickleObject.get(), PickleObject->GetComponent<dae::SpriteRendererComponent>(), "Idle");
    animationComponent->Play("Stunned", true);
    PickleObject->AddComponent(std::move(animationComponent));
    PickleObject->SetLocalPosition(glm::vec3(x, y, 0.0f));

    auto hitBox = std::make_unique<HitBox>(scale);
    hitBox->SetGameObject(PickleObject.get());
    PickleObject->AddComponent(std::move(hitBox));

    dae::SceneData::GetInstance().AddGameObject(PickleObject.get(), dae::GameObjectType::enemy);

    auto aiComponent = std::make_unique<MrPickleAIComponent>(PickleObject.get(), s_loadMap);
    PickleObject->AddComponent(std::move(aiComponent));

    auto enemyComponent = std::make_unique<game::EnemyComponent>(PickleObject.get(), scene);
    PickleObject->AddComponent(std::move(enemyComponent));


    scene->Add(std::move(PickleObject));
}


void SceneHelpers::SpawnPlayer(dae::Scene* scene, float x, float y, glm::vec2 scale)
{
    int score;
    int lives;
    auto PlayerObject = std::make_unique<dae::GameObject>();
    if (GameData::GetInstance().GetPlayerData(playerCount).score != 0)
        score = GameData::GetInstance().GetPlayerData(playerCount).score;
    else
        score = 0;

    auto Character1points = std::make_unique<dae::PointComponent>(score);
    PlayerObject->AddComponent(std::move(Character1points));

    if (GameData::GetInstance().GetPlayerData(playerCount).lives != 3)
        lives = GameData::GetInstance().GetPlayerData(playerCount).lives;
    else
        lives = 3;

    auto Character1Health = std::make_unique<dae::HealthComponent>(100, lives);
    PlayerObject->AddComponent(std::move(Character1Health));

    auto spriteRenderComponent = std::make_unique<dae::SpriteRendererComponent>(PlayerObject.get(), dae::ResourceManager::GetSprite("chef"));
    spriteRenderComponent->SetDimensions(40, 40);
    PlayerObject->AddComponent(std::move(spriteRenderComponent));

    auto animationComponent = std::make_unique<dae::AnimationComponent>(PlayerObject.get(), PlayerObject->GetComponent<dae::SpriteRendererComponent>(), "Idle");
    animationComponent->Play("Idle", true);
    PlayerObject->AddComponent(std::move(animationComponent));
    PlayerObject->SetLocalPosition(glm::vec3(x, y, 0.0f));

    auto hitBox = std::make_unique<HitBox>(scale);
    hitBox->SetGameObject(PlayerObject.get());
    PlayerObject->AddComponent(std::move(hitBox));

    auto PlayerComponent = std::make_unique<game::Player>(PlayerObject.get());
    PlayerObject->AddComponent(std::move(PlayerComponent));

    auto pepercomponent = std::make_unique<game::Pepper>(PlayerObject.get(), glm::vec3(1.f,0.f,0.f),10.f,12.f);
    PlayerObject->AddComponent(std::move(pepercomponent));

    dae::SceneData::GetInstance().AddGameObject(PlayerObject.get(), dae::GameObjectType::Player);

    scene->Add(std::move(PlayerObject));

}

void SceneHelpers::SpawnPlayerEnemy(dae::Scene* scene, float x, float y, glm::vec2 scale)
{
    auto PlayerObject = std::make_unique<dae::GameObject>();

    auto Character1Health = std::make_unique<dae::HealthComponent>(100, 3);
    PlayerObject->AddComponent(std::move(Character1Health));

    auto Character1points = std::make_unique<dae::PointComponent>(0);
    PlayerObject->AddComponent(std::move(Character1points));

    auto spriterenderComponent = std::make_unique<dae::SpriteRendererComponent>(PlayerObject.get(), dae::ResourceManager::GetSprite("sausage"));
    spriterenderComponent->SetDimensions(40, 40);
    PlayerObject->AddComponent(std::move(spriterenderComponent));

    auto animationComponent = std::make_unique<dae::AnimationComponent>(PlayerObject.get(), PlayerObject->GetComponent<dae::SpriteRendererComponent>(), "Idle");
    animationComponent->Play("Idle", true);
    PlayerObject->AddComponent(std::move(animationComponent));
    PlayerObject->SetLocalPosition(glm::vec3(x, y, 0.0f));

    auto hitBox = std::make_unique<HitBox>(scale);
    hitBox->SetGameObject(PlayerObject.get());
    PlayerObject->AddComponent(std::move(hitBox));

    auto PlayerComponent = std::make_unique<game::Player>(PlayerObject.get());
    PlayerObject->AddComponent(std::move(PlayerComponent));

    dae::SceneData::GetInstance().AddGameObject(PlayerObject.get(), dae::GameObjectType::enemyPlayers);


    auto tempobject = std::make_unique<dae::GameObject>();

    auto tempobjecthitBox = std::make_unique<HitBox>(scale);
    tempobjecthitBox->SetGameObject(tempobject.get());
    tempobject->AddComponent(std::move(tempobjecthitBox));

    tempobject->SetParent(PlayerObject.get());
    dae::SceneData::GetInstance().AddGameObject(tempobject.get(), dae::GameObjectType::enemy);

    scene->Add(std::move(tempobject));
    scene->Add(std::move(PlayerObject));
}


void SceneHelpers::LoadMapIntoScene(const LoadMap& loadMap, dae::Scene* scene, const glm::vec3& startPos, glm::vec2 scale) {
    const auto& map = loadMap.getMap();
    s_loadMap = map;

    // Initialize variables to store the minimum and maximum X and Y coordinates
    s_MinCoordinates.x = startPos.x;
    s_MinCoordinates.y = startPos.y;
    s_MaxCoordinates.x = startPos.x;
    s_MaxCoordinates.y = startPos.y;

    s_CellSize = scale;

    for (int y = 0; y < static_cast<int>(map.size()); ++y) {
        for (int x = 0; x < static_cast<int>(map[y].size()); ++x) {
            const char tile = map[y][x];
            const float posX = startPos.x + (x * scale.x);
            const float posY = startPos.y + (y * scale.y);

            // Update maximum coordinates
            if (posX > s_MaxCoordinates.x) 
                s_MaxCoordinates.x = posX;
            if (posY > s_MaxCoordinates.y) s_MaxCoordinates.y = posY;

            switch (tile) {
            case 'v':
                CreateLadderDown(scene, posX, posY, scale);
                break;
            case '#':
                CreateSolidLadder(scene, posX, posY, scale);
                break;
            case '^':
                CreateLadderUp(scene, posX, posY, scale);
                break;
            case '|':
                CreateLadderUpDown(scene, posX, posY, scale);
                break;
            case '_':
                CreateFloor(scene, posX, posY, scale);
                break;
            case 'u':
                CreatePlatformMiddle(scene, posX, posY, scale);
                break;
            case 'l':
                CreatePlatformLeft(scene, posX, posY, scale);
                break;
            case 'r':
                CreatePlatformRight(scene, posX, posY, scale);
                break;
            case 'c':
                CreatePlatformCombined(scene, posX, posY, scale);
                break;
            default:
                break;
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
                CreateBurgerTop(scene, posX, posY, glm::vec2(scale.x *3, scale.y));
                break;
            case 'L':
                CreateLettuce(scene, posX, posY, glm::vec2(scale.x * 3, scale.y));
                break;
            case 'M':
                CreateMeat(scene, posX, posY, glm::vec2(scale.x * 3, scale.y));
                break;
            case 'v':
                CreateBurgerBottom(scene, posX, posY, glm::vec2(scale.x * 3, scale.y));
                break;
            case 'C':
                CreateCheese(scene, posX, posY, glm::vec2(scale.x * 3, scale.y));
                break;
            case 'T':
                CreateTomato(scene, posX, posY, glm::vec2(scale.x * 3, scale.y));
                break;
            case 'S':
				SpawnSauge(scene, posX, posY, glm::vec2(scale.x, scale.y + 10));
                break;
            case 'E':
                SpawnEgg(scene, posX, posY, glm::vec2(scale.x, scale.y + 10));
                break;
            case 'D':
                SpawnPickle(scene, posX, posY, scale);
                break;
            case '0':
                SpawnPlayer(scene, posX, posY, scale);
                playerCount += 1;
                break;
            case '1':
                SpawnPlayerEnemy(scene, posX, posY, scale);
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





