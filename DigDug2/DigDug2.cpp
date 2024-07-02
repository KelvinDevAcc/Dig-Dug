#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <iostream>

#include "FPSCounterComponent.h"
#include "HighScores.h"
#include "InputManager.h"
#include "MenuComponent.h"
#include "Minigin.h"
#include "ResourceManager.h"
#include "SceneData.h"
#include "SceneManager.h"
#include "TextComponent.h"

void loadResources()
{
    dae::ResourceManager::LoadFont("arcadeclassic", "ARCADECLASSIC.TTF", 48);
    dae::ResourceManager::LoadFont("digdug", "DIG DUG.ttf", 48);
    dae::ResourceManager::LoadFont("font", "font.ttf", 28);
    dae::ResourceManager::LoadFont("TITLE", "font.ttf", 136);
    dae::ResourceManager::LoadFont("TITLEBig", "font.ttf", 144);
    dae::ResourceManager::LoadFont("arcadeBig", "arcade-legacy.ttf", 28);
    dae::ResourceManager::LoadFont("arcade", "arcade-legacy.ttf", 24);

}

//void BindMenuCommands(dae::InputManager& inputManager)
//{
//    // Bind keyboard commands
//    inputManager.BindCommand(SDL_SCANCODE_W, KeyState::Up, std::make_unique<NavigateUpCommand>(), InputType::Keyboard);
//    inputManager.BindCommand(SDL_SCANCODE_S, KeyState::Up, std::make_unique<NavigateDownCommand>(), InputType::Keyboard);
//    inputManager.BindCommand(SDL_SCANCODE_RETURN, KeyState::Up, std::make_unique<SelectOptionCommand>(), InputType::Keyboard);
//
//    // Bind controller commands
//    inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::DPadUp), KeyState::Up, std::make_unique<NavigateUpCommand>(), InputType::Controller);
//    inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::DPadDown), KeyState::Up, std::make_unique<NavigateDownCommand>(), InputType::Controller);
//    inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::Y), KeyState::Up, std::make_unique<SelectOptionCommand>(), InputType::Controller);
//}
//
//void UnBindMenuCommands(dae::InputManager& inputManager)
//{
//    // Unbind keyboard commands
//    inputManager.UnbindCommand(SDL_SCANCODE_W, KeyState::Up, InputType::Keyboard);
//    inputManager.UnbindCommand(SDL_SCANCODE_S, KeyState::Up, InputType::Keyboard);
//    inputManager.UnbindCommand(SDL_SCANCODE_RETURN, KeyState::Up, InputType::Keyboard);
//
//    // Unbind controller commands
//    inputManager.UnbindCommand(GameController::GetButtonMapping(GameController::Button::DPadUp), KeyState::Up, InputType::Controller);
//    inputManager.UnbindCommand(GameController::GetButtonMapping(GameController::Button::DPadDown), KeyState::Up, InputType::Controller);
//    inputManager.UnbindCommand(GameController::GetButtonMapping(GameController::Button::X), KeyState::Up, InputType::Controller);
//    inputManager.UnbindCommand(GameController::GetButtonMapping(GameController::Button::Y), KeyState::Up, InputType::Controller);
//
//}


void LoadStartMenu(dae::Scene* startMenuScene)
{
    auto& inputManager = dae::InputManager::GetInstance();
    const int numControllers = inputManager.GetConnectedControllerCount();
    //BindMenuCommands(inputManager);

    // Create GameObject for FPS counter
    auto fpsCounterObject = std::make_unique<dae::GameObject>();
    auto fpsTextComponent = std::make_unique<dae::TextComponent>("FPS: ", dae::ResourceManager::GetFont("font"), SDL_Color{ 252, 157, 3, 255 }, *fpsCounterObject);
    fpsCounterObject->AddComponent(std::move(fpsTextComponent));
    auto fpsTextComponentPtr = fpsCounterObject->GetComponent<dae::TextComponent>();
    auto fpsCounterComponent = std::make_unique<dae::FPSCounterComponent>(fpsTextComponentPtr);
    fpsCounterObject->AddComponent(std::move(fpsCounterComponent));
    fpsCounterObject->SetLocalPosition(glm::vec3(100.f, 20.f, 0.0f));
    startMenuScene->Add(std::move(fpsCounterObject));

    auto& highScoresInstance = HighScores::GetInstance();

    auto HighScoretextObject = std::make_unique<dae::GameObject>();
    auto HighScoretextComponent02 = std::make_unique<dae::TextComponent>(
        "HI-SCORE",
        dae::ResourceManager::GetFont("arcade"),
        SDL_Color{ 255, 0, 0, 255 },
        *HighScoretextObject
    );
    HighScoretextObject->SetLocalPosition(glm::vec3(630, 50, 0.f));
    HighScoretextObject->AddComponent(std::move(HighScoretextComponent02));
    startMenuScene->Add(std::move(HighScoretextObject));

    auto HighScoreObject = std::make_unique<dae::GameObject>();
    uint32_t topScore = highScoresInstance.getTopScore();
    std::string topScoreStr = std::to_string(topScore);
    auto scoreTextComponent = std::make_unique<dae::TextComponent>(topScoreStr, dae::ResourceManager::GetFont("arcade"), SDL_Color{ 255, 255, 255, 255 }, *HighScoreObject);
    HighScoreObject->SetLocalPosition(glm::vec3(640, 73, 0.f));
    HighScoreObject->AddComponent(std::move(scoreTextComponent));
    startMenuScene->Add(std::move(HighScoreObject));

    auto playerInfoTxt = std::make_unique<dae::GameObject>();
    auto playertextcomponent = std::make_unique<dae::TextComponent>("1UP", dae::ResourceManager::GetFont("arcade"), SDL_Color{ 255, 0, 0, 255 }, *playerInfoTxt);
    playerInfoTxt->AddComponent(std::move(playertextcomponent));
    playerInfoTxt->SetLocalPosition(glm::vec3(430, 50, 0.f));
    startMenuScene->Add(std::move(playerInfoTxt));


    if (numControllers == 1) {
        auto player02InfoTxt = std::make_unique<dae::GameObject>();
        auto player02textcomponent = std::make_unique<dae::TextComponent>("2UP", dae::ResourceManager::GetFont("arcade"), SDL_Color{ 255, 0, 0, 255 }, *player02InfoTxt);
        player02InfoTxt->AddComponent(std::move(player02textcomponent));
        player02InfoTxt->SetLocalPosition(glm::vec3(830, 50, 0.f));
        startMenuScene->Add(std::move(player02InfoTxt));
    }

    // Create GameObject for Title
    auto TitleObject02 = std::make_unique<dae::GameObject>();
    auto titleTextComponent02 = std::make_unique<dae::TextComponent>("DIG DUG", dae::ResourceManager::GetFont("TITLEBig"), SDL_Color{ 0, 0, 255, 255 }, *TitleObject02);
    TitleObject02->SetLocalPosition(glm::vec3(635, 110, 0.f));
    TitleObject02->AddComponent(std::move(titleTextComponent02));
    startMenuScene->Add(std::move(TitleObject02));

    // Create GameObject for Title
    auto TitleObject = std::make_unique<dae::GameObject>();
    auto titleTextComponent = std::make_unique<dae::TextComponent>("DIG DUG", dae::ResourceManager::GetFont("TITLE"), SDL_Color{ 247, 190, 57, 255 }, *TitleObject);
    TitleObject->SetLocalPosition(glm::vec3(635, 110, 0.f));
    TitleObject->AddComponent(std::move(titleTextComponent));
    startMenuScene->Add(std::move(TitleObject));


    std::vector<std::string> options = { "1 PLAYER", "2 PLAYERS", "versus Mode", "scoreBoard" };
    std::vector<std::function<void()>> callbacks =
    {
        []() {
            std::cout << "single player";
        },
        []() {
            std::cout << "multiplayer";

        },
        []() {
            std::cout << "versus Mode";

        },
        []() {
            std::cout << "ScoreBord";
        }
    };


    // Create the GameObject for the menu
    auto menuObject = std::make_unique<dae::GameObject>();
    menuObject->SetLocalPosition(glm::vec3(635, 330, 0.f));
    auto menuComponent = std::make_unique<dae::MenuComponent>(menuObject.get(), options, callbacks, dae::ResourceManager::GetFont("arcade"), 30.0f);
    menuComponent->SetTextColor(SDL_Color{ 255, 255, 255, 255 });
    menuObject->AddComponent(std::move(menuComponent));

    // Add the menu GameObject to the scene
    startMenuScene->Add(std::move(menuObject));



    auto InfoTxt = std::make_unique<dae::GameObject>();
    auto textcomponent = std::make_unique<dae::TextComponent>("NAMCOT", dae::ResourceManager::GetFont("arcade"), SDL_Color{ 255, 0, 0, 255 }, *InfoTxt);
    InfoTxt->AddComponent(std::move(textcomponent));
    InfoTxt->SetLocalPosition(glm::vec3(635, 545, 0.f));
    startMenuScene->Add(std::move(InfoTxt));

    auto Infocharacter1Txt02 = std::make_unique<dae::GameObject>();
    auto character1textcomponent02 = std::make_unique<dae::TextComponent>("© 1982 1985 NAMCO LTD.", dae::ResourceManager::GetFont("arcade"), SDL_Color{ 255, 255, 255, 255 }, *Infocharacter1Txt02); 
    Infocharacter1Txt02->AddComponent(std::move(character1textcomponent02));
    Infocharacter1Txt02->SetLocalPosition(glm::vec3(635, 585, 0.f));
    startMenuScene->Add(std::move(Infocharacter1Txt02));

    auto Infocharacter2Txt02 = std::make_unique<dae::GameObject>();
    auto character2textcomponent02 = std::make_unique<dae::TextComponent>("ALL RIGHTS RESERVED", dae::ResourceManager::GetFont("arcade"), SDL_Color{ 255, 255, 255, 255 }, *Infocharacter2Txt02); 
    Infocharacter2Txt02->AddComponent(std::move(character2textcomponent02));
    Infocharacter2Txt02->SetLocalPosition(glm::vec3(635, 620, 0.f));
    startMenuScene->Add(std::move(Infocharacter2Txt02));


  

}

void load()
{
    loadResources();
    auto& sceneManager = dae::SceneManager::GetInstance();

    const auto& startMenuScene = sceneManager.CreateScene("StartMenu");

    startMenuScene->SetOnActivateCallback([startMenuScene]() { LoadStartMenu(startMenuScene); });


    sceneManager.SetActiveScene("StartMenu");
}

int main(int, char* []) {
    dae::minigin engine("../Data/");
    engine.Run(load);
    return 0;
}