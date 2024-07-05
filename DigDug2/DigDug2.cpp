#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <iostream>
#include <random>

#include "FPSCounterComponent.h"
#include "HighScores.h"
#include "InputManager.h"
#include "MenuComponent.h"
#include "Minigin.h"
#include "ResourceManager.h"
#include "SceneData.h"
#include "SceneHelpers.h"
#include "SceneManager.h"
#include "servicelocator.h"
#include "TextComponent.h"
#include "TweenManager.h"

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
void UnBindMenuCommands(dae::InputManager& inputManager)
{
    // Unbind keyboard commands
    inputManager.UnbindCommand(SDL_SCANCODE_W, KeyState::Up, InputType::Keyboard);
    inputManager.UnbindCommand(SDL_SCANCODE_S, KeyState::Up, InputType::Keyboard);
    inputManager.UnbindCommand(SDL_SCANCODE_RETURN, KeyState::Up, InputType::Keyboard);

    // Unbind controller commands
    inputManager.UnbindCommand(GameController::GetButtonMapping(GameController::Button::DPadUp), KeyState::Up, InputType::Controller);
    inputManager.UnbindCommand(GameController::GetButtonMapping(GameController::Button::DPadDown), KeyState::Up, InputType::Controller);
    inputManager.UnbindCommand(GameController::GetButtonMapping(GameController::Button::A), KeyState::Up, InputType::Controller);

}

void UnBindPlayerCommands(dae::InputManager& inputManager)
{
    // Unbind keyboard commands
    inputManager.UnbindCommand(SDL_SCANCODE_W, KeyState::Pressed, InputType::Keyboard);
    inputManager.UnbindCommand(SDL_SCANCODE_S, KeyState::Pressed, InputType::Keyboard);
    inputManager.UnbindCommand(SDL_SCANCODE_A, KeyState::Pressed, InputType::Keyboard);
    inputManager.UnbindCommand(SDL_SCANCODE_D, KeyState::Pressed, InputType::Keyboard);
    inputManager.UnbindCommand(SDL_SCANCODE_X, KeyState::Up, InputType::Keyboard);
    inputManager.UnbindCommand(SDL_SCANCODE_Z, KeyState::Up, InputType::Keyboard);
    inputManager.UnbindCommand(SDL_SCANCODE_C, KeyState::Up, InputType::Keyboard);

    // Unbind controller commands
    inputManager.UnbindCommand(GameController::GetButtonMapping(GameController::Button::DPadUp), KeyState::Pressed, InputType::Controller);
    inputManager.UnbindCommand(GameController::GetButtonMapping(GameController::Button::DPadDown), KeyState::Pressed, InputType::Controller);
    inputManager.UnbindCommand(GameController::GetButtonMapping(GameController::Button::DPadLeft), KeyState::Pressed, InputType::Controller);
    inputManager.UnbindCommand(GameController::GetButtonMapping(GameController::Button::DPadRight), KeyState::Pressed, InputType::Controller);
    inputManager.UnbindCommand(GameController::GetButtonMapping(GameController::Button::A), KeyState::Up, InputType::Controller);
    inputManager.UnbindCommand(GameController::GetButtonMapping(GameController::Button::B), KeyState::Up, InputType::Controller);
    inputManager.UnbindCommand(GameController::GetButtonMapping(GameController::Button::X), KeyState::Up, InputType::Controller);
}

void BindMenuCommands(dae::InputManager& inputManager)
{
    UnBindPlayerCommands(inputManager);
    UnBindMenuCommands(inputManager);
    // Bind keyboard commands
    inputManager.BindCommand(SDL_SCANCODE_W, KeyState::Up, std::make_unique<NavigateUpCommand>(), InputType::Keyboard);
    inputManager.BindCommand(SDL_SCANCODE_S, KeyState::Up, std::make_unique<NavigateDownCommand>(), InputType::Keyboard);
    inputManager.BindCommand(SDL_SCANCODE_RETURN, KeyState::Up, std::make_unique<SelectOptionCommand>(), InputType::Keyboard);

    // Bind controller commands
    inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::DPadUp), KeyState::Up, std::make_unique<NavigateUpCommand>(), InputType::Controller);
    inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::DPadDown), KeyState::Up, std::make_unique<NavigateDownCommand>(), InputType::Controller);
    inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::A), KeyState::Up, std::make_unique<SelectOptionCommand>(), InputType::Controller);
}



void BindKeyboardCommands(dae::InputManager& inputManager, int playerId) {
    inputManager.BindCommand(SDL_SCANCODE_W, KeyState::Pressed, std::make_unique<MoveCommand>(playerId, 0.0f, -1.5f), InputType::Keyboard);
    inputManager.BindCommand(SDL_SCANCODE_S, KeyState::Pressed, std::make_unique<MoveCommand>(playerId, 0.0f, 1.5f), InputType::Keyboard);
    inputManager.BindCommand(SDL_SCANCODE_A, KeyState::Pressed, std::make_unique<MoveCommand>(playerId, -1.5f, 0.0f), InputType::Keyboard);
    inputManager.BindCommand(SDL_SCANCODE_D, KeyState::Pressed, std::make_unique<MoveCommand>(playerId, 1.5f, 0.0f), InputType::Keyboard);
    inputManager.BindCommand(SDL_SCANCODE_X, KeyState::Up, std::make_unique<ScorePointCommand>(playerId), InputType::Keyboard);
    inputManager.BindCommand(SDL_SCANCODE_Z, KeyState::Up, std::make_unique<ScorePointCommand>(playerId), InputType::Keyboard);
    inputManager.BindCommand(SDL_SCANCODE_C, KeyState::Up, std::make_unique<DamageCommand>(playerId), InputType::Keyboard);
}

void BindControllerCommands(dae::InputManager& inputManager, int playerId, int controllerIndex) {
    inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::DPadUp), KeyState::Pressed, std::make_unique<MoveCommand>(playerId, 0.0f, -1.5f), InputType::Controller, controllerIndex);
    inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::DPadDown), KeyState::Pressed, std::make_unique<MoveCommand>(playerId, 0.0f, 1.5f), InputType::Controller, controllerIndex);
    inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::DPadLeft), KeyState::Pressed, std::make_unique<MoveCommand>(playerId, -1.5f, 0.0f), InputType::Controller, controllerIndex);
    inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::DPadRight), KeyState::Pressed, std::make_unique<MoveCommand>(playerId, 1.5f, 0.0f), InputType::Controller, controllerIndex);
    inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::A), KeyState::Up, std::make_unique<ScorePointCommand>(playerId), InputType::Controller, controllerIndex);
    inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::X), KeyState::Up, std::make_unique<DamageCommand>(playerId), InputType::Controller, controllerIndex);
}
void HandlePlayerInput(dae::InputManager& inputManager, int playerId)
{
    UnBindMenuCommands(inputManager);
    const int numControllers = inputManager.GetConnectedControllerCount();

    // Bind keyboard commands for Player 0 (always)
    if (playerId == 0)
    {
        BindKeyboardCommands(inputManager, 0);
    }

    // Handle controller binding based on the number of controllers
    if (numControllers == 1)
    {
        if (playerId == 0)
        {
            // Player 0: Can use both keyboard and controller 0
            BindControllerCommands(inputManager, 0, 0);
        }
        else if (playerId == 1)
        {
            // Player 1: Can use controller 0
            BindControllerCommands(inputManager, 1, 0);
        }
    }
    else if (numControllers >= 2)
    {
        if (playerId == 0)
        {
            // Player 0: Can use keyboard and controller 0
            BindControllerCommands(inputManager, 0, 0);
        }
        else if (playerId == 1)
        {
            // Player 1: Can use controller 1
            BindControllerCommands(inputManager, 1, 1);
        }
    }


}

void BindExtraControlls(dae::InputManager& inputManager)
{
    // Additional keyboard commands
    inputManager.BindCommand(SDL_SCANCODE_F1, KeyState::Up, std::make_unique<GoToNextSceneCommand>(), InputType::Keyboard);
    inputManager.BindCommand(SDL_SCANCODE_M, KeyState::Up, std::make_unique<MuteCommand>(&servicelocator::get_sound_system()), InputType::Keyboard);
    inputManager.BindCommand(SDL_SCANCODE_P, KeyState::Up, std::make_unique<PlaySoundCommand>(), InputType::Keyboard);
    inputManager.BindCommand(SDL_SCANCODE_MINUS, KeyState::Up, std::make_unique<DecreaseVolumeCommand>(&servicelocator::get_sound_system()), InputType::Keyboard);
    inputManager.BindCommand(SDL_SCANCODE_EQUALS, KeyState::Up, std::make_unique<IncreaseVolumeCommand>(&servicelocator::get_sound_system()), InputType::Keyboard);


    inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::Start), KeyState::Up, std::make_unique<GoToNextSceneCommand>(), InputType::Controller);
    inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::Back), KeyState::Up, std::make_unique<MuteCommand>(&servicelocator::get_sound_system()), InputType::Controller);
    inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::LeftBumper), KeyState::Up, std::make_unique<IncreaseVolumeCommand>(&servicelocator::get_sound_system()), InputType::Controller);
    inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::RightBumper), KeyState::Up, std::make_unique<DecreaseVolumeCommand>(&servicelocator::get_sound_system()), InputType::Controller);

}

void LoadStartMenu(dae::Scene* startMenuScene)
{
    auto& inputManager = dae::InputManager::GetInstance();
    const int numControllers = inputManager.GetConnectedControllerCount();
    BindMenuCommands(inputManager);

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
    HighScoretextObject->SetLocalPosition(glm::vec3(640, 850, 0.f));
    glm::vec3 finalPositionHighScoretextObject(640, 50, 0.f);
    auto moveTweenHighScoretextObject = std::make_unique<Tween>(HighScoretextObject.get(), finalPositionHighScoretextObject, 10.0f, Easing::Linear);
    TweenManager::GetInstance().AddTween(std::move(moveTweenHighScoretextObject));
    HighScoretextObject->AddComponent(std::move(HighScoretextComponent02));
    startMenuScene->Add(std::move(HighScoretextObject));

    auto HighScoreObject = std::make_unique<dae::GameObject>();
    uint32_t topScore = highScoresInstance.getTopScore();
    std::string topScoreStr = std::to_string(topScore);
    auto scoreTextComponent = std::make_unique<dae::TextComponent>(topScoreStr, dae::ResourceManager::GetFont("arcade"), SDL_Color{ 255, 255, 255, 255 }, *HighScoreObject);
    HighScoreObject->SetLocalPosition(glm::vec3(640, 873, 0.f));
    glm::vec3 finalPositionHighScoreObject(640, 73, 0.f);
    auto moveTweenHighScoreObject = std::make_unique<Tween>(HighScoreObject.get(), finalPositionHighScoreObject, 10.0f, Easing::Linear);
    TweenManager::GetInstance().AddTween(std::move(moveTweenHighScoreObject));
    HighScoreObject->AddComponent(std::move(scoreTextComponent));
    startMenuScene->Add(std::move(HighScoreObject));

    auto playerInfoTxt = std::make_unique<dae::GameObject>();
    auto playertextcomponent = std::make_unique<dae::TextComponent>("1UP", dae::ResourceManager::GetFont("arcade"), SDL_Color{ 255, 0, 0, 255 }, *playerInfoTxt);
    playerInfoTxt->AddComponent(std::move(playertextcomponent));
    playerInfoTxt->SetLocalPosition(glm::vec3(430, 850, 0.f));
    glm::vec3 finalPositionplayerInfoTxt(430, 50, 0.f);
    auto moveTweenplayerInfoTxt = std::make_unique<Tween>(playerInfoTxt.get(), finalPositionplayerInfoTxt, 10.0f, Easing::Linear);
    TweenManager::GetInstance().AddTween(std::move(moveTweenplayerInfoTxt));
    startMenuScene->Add(std::move(playerInfoTxt));


    if (numControllers == 1) {
        auto player02InfoTxt = std::make_unique<dae::GameObject>();
        auto player02textcomponent = std::make_unique<dae::TextComponent>("2UP", dae::ResourceManager::GetFont("arcade"), SDL_Color{ 255, 0, 0, 255 }, *player02InfoTxt);
        player02InfoTxt->AddComponent(std::move(player02textcomponent));
        player02InfoTxt->SetLocalPosition(glm::vec3(830, 850, 0.f));
        glm::vec3 finalPositioplayer02InfoTxt(830, 50, 0.f);
        auto moveTweenplayer02InfoTxt = std::make_unique<Tween>(player02InfoTxt.get(), finalPositioplayer02InfoTxt, 10.0f, Easing::Linear);
        TweenManager::GetInstance().AddTween(std::move(moveTweenplayer02InfoTxt));
        startMenuScene->Add(std::move(player02InfoTxt));
    }

    // Create GameObject for Title
    auto TitleObject02 = std::make_unique<dae::GameObject>();
    auto titleTextComponent02 = std::make_unique<dae::TextComponent>("DIG DUG", dae::ResourceManager::GetFont("TITLEBig"), SDL_Color{ 0, 0, 255, 255 }, *TitleObject02);
    TitleObject02->SetLocalPosition(glm::vec3(635, 810, 0.f));
    glm::vec3 finalPositioTitleObject02(635.f, 110, 0.f);
    auto moveTweeTitleObject02 = std::make_unique<Tween>(TitleObject02.get(), finalPositioTitleObject02, 10.0f, Easing::Linear);
    TweenManager::GetInstance().AddTween(std::move(moveTweeTitleObject02));
    TitleObject02->AddComponent(std::move(titleTextComponent02));
    startMenuScene->Add(std::move(TitleObject02));

    // Create GameObject for Title
    auto TitleObject = std::make_unique<dae::GameObject>();
    auto titleTextComponent = std::make_unique<dae::TextComponent>("DIG DUG", dae::ResourceManager::GetFont("TITLE"), SDL_Color{ 247, 190, 57, 255 }, *TitleObject);
    TitleObject->SetLocalPosition(glm::vec3(635, 810, 0.f));
    glm::vec3 finalPositioTitleObject(635.f, 110, 0.f);
    auto moveTweenmenuTitleObject = std::make_unique<Tween>(TitleObject.get(), finalPositioTitleObject, 10.0f, Easing::Linear);
    TweenManager::GetInstance().AddTween(std::move(moveTweenmenuTitleObject));
    TitleObject->AddComponent(std::move(titleTextComponent));
    startMenuScene->Add(std::move(TitleObject));


    std::vector<std::string> options = { "1 PLAYER", "2 PLAYERS", "versus Mode", "scoreBoard" };
    std::vector<std::function<void()>> callbacks =
    {
        []() {
            std::cout << "single player" << std::endl;
            dae::SceneManager::GetInstance().SetActiveScene("Game");
        },
        []() {
            std::cout << "multiplayer" << std::endl;
        },
        []() {
            std::cout << "versus Mode" << std::endl;
        },
        []() {
            dae::SceneManager::GetInstance().SetActiveScene("ScoreboardScene");
        }
    };

    // Create the GameObject for the menu
    auto menuObject = std::make_unique<dae::GameObject>();
    menuObject->SetLocalPosition(glm::vec3(635, 30, 0.f));
    auto menuComponent = std::make_unique<dae::MenuComponent>(menuObject.get(), options, callbacks, dae::ResourceManager::GetFont("arcade"), 30.0f);
    menuComponent->SetTextColor(SDL_Color{ 255, 255, 255, 255 });
    menuObject->AddComponent(std::move(menuComponent));

    // Create Tween to move menuObject into final position
    glm::vec3 finalPositionmenuObject(635.f, 330.f, 0.f);
    auto moveTweenmenuObject = std::make_unique<Tween>(menuObject.get(), finalPositionmenuObject, 10.0f, Easing::Linear, [menuObjectPtr = menuObject.get()]() {
        if (menuObjectPtr) {  // Check if the pointer is valid
	        if (const auto menuComponent = menuObjectPtr->GetComponent<dae::MenuComponent>()) {
                menuComponent->SetActive(true);
            }
        }
        });
    TweenManager::GetInstance().AddTween(std::move(moveTweenmenuObject));


    // Add the menu GameObject to the scene
    startMenuScene->Add(std::move(menuObject));



    // Creating the InfoTxt GameObject and adding a TextComponent
    auto InfoTxt = std::make_unique<dae::GameObject>();
    auto textComponent = std::make_unique<dae::TextComponent>("NAMCOT", dae::ResourceManager::GetFont("arcade"), SDL_Color{ 255, 0, 0, 255 }, *InfoTxt);
    InfoTxt->AddComponent(std::move(textComponent));
    InfoTxt->SetLocalPosition(glm::vec3(635.f, 845.f, 0.f)); // Start off-screen to the left
    glm::vec3 finalPosition(635.f, 545.f, 0.f);
    auto moveTween = std::make_unique<Tween>(InfoTxt.get(), finalPosition, 10.0f, Easing::Linear);
    TweenManager::GetInstance().AddTween(std::move(moveTween));
    startMenuScene->Add(std::move(InfoTxt));


    auto Infocharacter1Txt02 = std::make_unique<dae::GameObject>();
    auto character1textcomponent02 = std::make_unique<dae::TextComponent>("� 1982 1985 NAMCO LTD.", dae::ResourceManager::GetFont("arcade"), SDL_Color{ 255, 255, 255, 255 }, *Infocharacter1Txt02); 
    Infocharacter1Txt02->AddComponent(std::move(character1textcomponent02));
    Infocharacter1Txt02->SetLocalPosition(glm::vec3(635, 885, 0.f));
    glm::vec3 finalPositionInfocharacter1Txt02(635.f, 585.f, 0.f);
    auto moveInfocharacter1Txt02 = std::make_unique<Tween>(Infocharacter1Txt02.get(), finalPositionInfocharacter1Txt02, 10.0f, Easing::Linear);
    TweenManager::GetInstance().AddTween(std::move(moveInfocharacter1Txt02));
    startMenuScene->Add(std::move(Infocharacter1Txt02));

    auto Infocharacter2Txt02 = std::make_unique<dae::GameObject>();
    auto character2textcomponent02 = std::make_unique<dae::TextComponent>("ALL RIGHTS RESERVED", dae::ResourceManager::GetFont("arcade"), SDL_Color{ 255, 255, 255, 255 }, *Infocharacter2Txt02); 
    Infocharacter2Txt02->AddComponent(std::move(character2textcomponent02));
    Infocharacter2Txt02->SetLocalPosition(glm::vec3(635, 920, 0.f));
    glm::vec3 finalPositionInfocharacter2Txt02(635.f, 620, 0.f);
    auto moveTweenInfocharacter2Txt02 = std::make_unique<Tween>(Infocharacter2Txt02.get(), finalPositionInfocharacter2Txt02, 10.0f, Easing::Linear);
    TweenManager::GetInstance().AddTween(std::move(moveTweenInfocharacter2Txt02));
    startMenuScene->Add(std::move(Infocharacter2Txt02));


  

}

void LoadScoreboard(dae::Scene* ScoreBoardScene)
{
    auto& inputManager = dae::InputManager::GetInstance();
	const auto& highScore = HighScores::GetInstance();

    const auto& scores = highScore.getHighScores();

    for (size_t i = 0; i < scores.size(); ++i) {
        const auto& playerName = scores[i].first;
        const auto score = scores[i].second;

        auto gameObject = std::make_unique<dae::GameObject>();

        std::string text = playerName.data() + std::string(": ") + std::to_string(score);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 255);

        SDL_Color textColor = {
            static_cast<unsigned char>(dis(gen)),   // Red
            static_cast<unsigned char>(dis(gen)),   // Green
            static_cast<unsigned char>(dis(gen)),   // Blue
            255                                      // Alpha
        };
        auto textComponent = std::make_unique<dae::TextComponent>(text, dae::ResourceManager::GetFont("arcadeBig"), textColor, *gameObject);

        gameObject->SetLocalPosition(glm::vec3(635, 230 + i * 50, 0.f));

        gameObject->AddComponent(std::move(textComponent));

        ScoreBoardScene->Add(std::move(gameObject));
    }


    // Create GameObject for Title
    auto TitleObject02 = std::make_unique<dae::GameObject>();
    auto titleTextComponent02 = std::make_unique<dae::TextComponent>("DIG DUG", dae::ResourceManager::GetFont("TITLEBig"), SDL_Color{ 0, 0, 255, 255 }, *TitleObject02);
    TitleObject02->SetLocalPosition(glm::vec3(635, 810, 0.f));
    glm::vec3 finalPositioTitleObject02(635.f, 20, 0.f);
    auto moveTweeTitleObject02 = std::make_unique<Tween>(TitleObject02.get(), finalPositioTitleObject02, 10.0f, Easing::EaseInOutQuad);
    TweenManager::GetInstance().AddTween(std::move(moveTweeTitleObject02));
    TitleObject02->AddComponent(std::move(titleTextComponent02));
    ScoreBoardScene->Add(std::move(TitleObject02));

    // Create GameObject for Title
    auto TitleObject = std::make_unique<dae::GameObject>();
    auto titleTextComponent = std::make_unique<dae::TextComponent>("DIG DUG", dae::ResourceManager::GetFont("TITLE"), SDL_Color{ 247, 190, 57, 255 }, *TitleObject);
    TitleObject->SetLocalPosition(glm::vec3(635, 810, 0.f));
    glm::vec3 finalPositioTitleObject(635.f, 20, 0.f);
    auto moveTweenmenuTitleObject = std::make_unique<Tween>(TitleObject.get(), finalPositioTitleObject, 10.0f, Easing::EaseInOutQuad);
    TweenManager::GetInstance().AddTween(std::move(moveTweenmenuTitleObject));
    TitleObject->AddComponent(std::move(titleTextComponent));
    ScoreBoardScene->Add(std::move(TitleObject));

    // Create GameObject for FPS counter
    auto fpsCounterObject = std::make_unique<dae::GameObject>();
    auto fpsTextComponent = std::make_unique<dae::TextComponent>("FPS: ", dae::ResourceManager::GetFont("font"), SDL_Color{ 252, 157, 3, 255 }, *fpsCounterObject);
    fpsCounterObject->AddComponent(std::move(fpsTextComponent));
    auto fpsTextComponentPtr = fpsCounterObject->GetComponent<dae::TextComponent>();
    auto fpsCounterComponent = std::make_unique<dae::FPSCounterComponent>(fpsTextComponentPtr);
    fpsCounterObject->AddComponent(std::move(fpsCounterComponent));
    fpsCounterObject->SetLocalPosition(glm::vec3(100.f, 20.f, 0.0f));
    ScoreBoardScene->Add(std::move(fpsCounterObject));


    std::vector<std::string> options2 = { "back to menu" };
    std::vector<std::function<void()>> callbacks2 =
    {
        []() { dae::SceneManager::GetInstance().SetActiveScene("StartMenu"); }
    };

    // Create the GameObject for the menu
    auto menuObject = std::make_unique<dae::GameObject>();
    menuObject->SetLocalPosition(glm::vec3(635, 600, 0.f));
    auto menuComponent = std::make_unique<dae::MenuComponent>(menuObject.get(), options2, callbacks2, dae::ResourceManager::GetFont("arcadeBig"), 70.0f);
    menuComponent->SetTextColor(SDL_Color{ 220,200,100,255 });
    menuComponent->SetActive(true);
    menuObject->AddComponent(std::move(menuComponent));
    ScoreBoardScene->Add(std::move(menuObject));
    BindMenuCommands(inputManager);
}

void LoadUi(dae::Scene* scene)
{
    auto& highScoresInstance = HighScores::GetInstance();
    const std::vector<dae::GameObject*> players = dae::SceneData::GetInstance().GetPlayers();


    auto HighScoretextObject = std::make_unique<dae::GameObject>();

    // Create the "HIGHSCORE:" text component
    auto titleTextComponent02 = std::make_unique<dae::TextComponent>(
        "HI-SCORE",
        dae::ResourceManager::GetFont("arcade"),
        SDL_Color{ 255, 0, 0, 255 },
        *HighScoretextObject
    );
    HighScoretextObject->SetLocalPosition(glm::vec3(630, 5, 0.f));
    HighScoretextObject->AddComponent(std::move(titleTextComponent02));

    scene->Add(std::move(HighScoretextObject));

    auto HighScoreObject = std::make_unique<dae::GameObject>();
    // Retrieve the top score and convert it to a string
    uint32_t topScore = highScoresInstance.getTopScore();
    std::string topScoreStr = std::to_string(topScore);

    // Create the high score value text component using the string
    auto scoreTextComponent = std::make_unique<dae::TextComponent>(topScoreStr, dae::ResourceManager::GetFont("arcade"), SDL_Color{ 255, 255, 255, 255 }, *HighScoreObject);
    HighScoreObject->SetLocalPosition(glm::vec3(640, 33, 0.f));
    HighScoreObject->AddComponent(std::move(scoreTextComponent));

    // Add HighScoreObject to the scene (assuming 'this' is a scene class)
    scene->Add(std::move(HighScoreObject));


    //glm::vec3 initialPositionPepper(100, 170, 0.f);
    //// Define the distance between points display objects
    //float distanceBetweenPepper = 150.0f;

    //for (size_t i = 0; i < players.size(); ++i) {
    //    auto player = players[i];
    //    if (auto scoreComponent = player->GetComponent<dae::PointComponent>()) {
    //        auto PepertextObject = std::make_unique<dae::GameObject>();

    //        auto PeperComponent = std::make_unique<dae::TextComponent>(
    //            "PEPPER",
    //            dae::ResourceManager::GetFont("arcade"),
    //            SDL_Color{ 0, 255, 0, 255 },
    //            *PepertextObject
    //        );
    //        glm::vec3 position = initialPositionPepper + glm::vec3(0.0f, i * distanceBetweenPepper, 0.0f);
    //        PepertextObject->SetLocalPosition(glm::vec3(position));
    //        PepertextObject->AddComponent(std::move(PeperComponent));

    //        scene->Add(std::move(PepertextObject));

    //        auto PeperscoretextObject = std::make_unique<dae::GameObject>();

    //        auto PeperscoreComponent = std::make_unique<dae::TextComponent>(
    //            "4",
    //            dae::ResourceManager::GetFont("arcade"),
    //            SDL_Color{ 255, 255, 255, 255 },
    //            *PeperscoretextObject
    //        );
    //        glm::vec3 positionscore = initialPositionPepper + glm::vec3(0.0f, i * distanceBetweenPepper + 20, 0.0f);
    //        PeperscoretextObject->SetLocalPosition(positionscore);
    //        PeperscoretextObject->AddComponent(std::move(PeperscoreComponent));

    //        scene->Add(std::move(PeperscoretextObject));
    //    }
    //}




    //glm::vec3 initialPositionpoints(100, 235, 0.f);
    //// Define the distance between points display objects
    //float distanceBetweenPoints = 50.0f;

    //for (size_t i = 0; i < players.size(); ++i) {
    //    auto player = players[i];
    //    if (auto scoreComponent = player->GetComponent<dae::PointComponent>()) {
    //        auto pointsDisplayObject = std::make_unique<dae::GameObject>();
    //        auto pointsDisplayComponent = std::make_unique<dae::PointsDisplayComponent>(dae::ResourceManager::GetFont("arcade"), *pointsDisplayObject);

    //        // Attach the points display component to the player's point component
    //        pointsDisplayComponent->AttachToPointComponent(scoreComponent);
    //        pointsDisplayObject->AddComponent(std::move(pointsDisplayComponent));

    //        // Calculate the position of the points display object based on the index and distance
    //        glm::vec3 position = initialPositionpoints + glm::vec3(0.0f, i * distanceBetweenPoints, 0.0f);
    //        pointsDisplayObject->SetLocalPosition(position);

    //        // Add the points display object to the scene
    //        scene->Add(std::move(pointsDisplayObject));
    //    }
    //}


    //glm::vec3 initialPositionHealth(100, 215, 0.f);
    //// Define the distance between lives display objects
    //float distanceBetweenLives = 50.0f;

    //for (size_t i = 0; i < players.size(); ++i) {
    //    auto player = players[i];
    //    if (auto healthComponent = player->GetComponent<dae::HealthComponent>()) {
    //        auto livesDisplayObject = std::make_unique<dae::GameObject>();
    //        auto livesDisplayComponent = std::make_unique<dae::LivesDisplayComponent>(dae::ResourceManager::GetFont("arcade"), *livesDisplayObject);

    //        // Attach the lives display component to the player's health component
    //        livesDisplayComponent->AttachToHealthComponent(healthComponent);
    //        livesDisplayObject->AddComponent(std::move(livesDisplayComponent));

    //        // Calculate the position of the lives display object based on the index and distance
    //        glm::vec3 position = initialPositionHealth + glm::vec3(0.0f, i * distanceBetweenLives, 0.0f);
    //        livesDisplayObject->SetLocalPosition(position);

    //        // Add the lives display object to the scene
    //        scene->Add(std::move(livesDisplayObject));
    //    }
    //}
}

void GameScene(dae::Scene* scene)
{
    auto& inputManager = dae::InputManager::GetInstance();
    HandlePlayerInput(inputManager,0);
    LoadUi(scene);

    // Create GameObject for FPS counter
    auto fpsCounterObject = std::make_unique<dae::GameObject>();
    auto fpsTextComponent = std::make_unique<dae::TextComponent>("FPS: ", dae::ResourceManager::GetFont("font"), SDL_Color{ 252, 157, 3, 255 }, *fpsCounterObject);
    fpsCounterObject->AddComponent(std::move(fpsTextComponent));
    auto fpsTextComponentPtr = fpsCounterObject->GetComponent<dae::TextComponent>();
    auto fpsCounterComponent = std::make_unique<dae::FPSCounterComponent>(fpsTextComponentPtr);
    fpsCounterObject->AddComponent(std::move(fpsCounterComponent));
    fpsCounterObject->SetLocalPosition(glm::vec3(100.f, 20.f, 0.0f));
    scene->Add(std::move(fpsCounterObject));

    constexpr glm::vec3 startPos(335, 70, 0.0f);
    constexpr glm::vec2 mapScale(40, 26.f);

    const LoadMap loadMap("../Data/maps/map1.map", "../Data/maps/" + Ingrediantmap);
    SceneHelpers::LoadMapIntoScene(loadMap, scene, startPos, mapScale);

    SceneHelpers::LoadIngMapIntoScene(loadMap, scene, startPos, mapScale);


}

void load()
{
    BindExtraControlls(dae::InputManager::GetInstance());
    loadResources();
    auto& sceneManager = dae::SceneManager::GetInstance();

    const auto& startMenuScene = sceneManager.CreateScene("StartMenu");
    const auto& ScoreBoardScene = sceneManager.CreateScene("ScoreboardScene");
    const auto& Game = sceneManager.CreateScene("Game");

    startMenuScene->SetOnActivateCallback([startMenuScene]() { LoadStartMenu(startMenuScene); });
    ScoreBoardScene->SetOnActivateCallback([ScoreBoardScene]() {LoadScoreboard(ScoreBoardScene); });
    Game->SetOnActivateCallback([Game]() { GameScene(Game); });



    sceneManager.SetActiveScene("StartMenu");
}

int main(int, char* []) {
    dae::minigin engine("../Data/");
    engine.Run(load);
    return 0;
}