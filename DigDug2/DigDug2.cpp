#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <iostream>
#include <random>

#include "AnimationComponent.h"
#include "FPSCounterComponent.h"
#include "GameData.h"
#include "HighScores.h"
#include "InputManager.h"
#include "LivesDisplayComponent.h"
#include "MenuComponent.h"
#include "Minigin.h"
#include "Player.h"
#include "PointComponent.h"
#include "PointsDisplayComponent.h"
#include "ResourceManager.h"
#include "SceneData.h"
#include "SceneHelpers.h"
#include "SceneManager.h"
#include "servicelocator.h"
#include "SpriteRendererComponent.h"
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

    dae::ResourceManager::LoadTexture("Lives.png");
    dae::ResourceManager::LoadSprite("Player",
        "spritesheetemty.png",
        26,  // rowCount
        16,  // colCount
        {
            { "Idle", { { { 1, 0 }}, 1 } },
            { "Walk_Right", { {  { 4, 0 }, { 5, 0 } }, 4 } },
            { "Walk_Left", { { { 4, 0 }, { 5, 0 } }, 4 } },
            { "Walk_Up", { { { 2, 0 }, { 3, 0 } }, 4 } },
            { "Walk_Down", { { { 6, 0 }, { 7,0 } }, 4 } },
            { "Dying", { { { 3, 1 }, { 4, 1 }, { 5, 1 }, { 6, 1 }, { 7, 1 }, { 8, 1 } }, 2 } },
            { "Attacking", { { { 1, 1 } }, 1 } },
            { "Victory", { { { 3, 1 }, { 1, 0 }}, 3 } }

        });

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

void UnBindNameCommands(dae::InputManager& inputManager)
{
    // Unbind keyboard commands
    inputManager.UnbindCommand(SDL_SCANCODE_W, KeyState::Up, InputType::Keyboard);
    inputManager.UnbindCommand(SDL_SCANCODE_S, KeyState::Up, InputType::Keyboard);
    inputManager.UnbindCommand(SDL_SCANCODE_D, KeyState::Up, InputType::Keyboard);
    inputManager.UnbindCommand(SDL_SCANCODE_RETURN, KeyState::Up, InputType::Keyboard);

    // Unbind controller commands
    inputManager.UnbindCommand(GameController::GetButtonMapping(GameController::Button::DPadUp), KeyState::Up, InputType::Controller);
    inputManager.UnbindCommand(GameController::GetButtonMapping(GameController::Button::DPadDown), KeyState::Up, InputType::Controller);
    inputManager.UnbindCommand(GameController::GetButtonMapping(GameController::Button::X), KeyState::Up, InputType::Controller);
    inputManager.UnbindCommand(GameController::GetButtonMapping(GameController::Button::Y), KeyState::Up, InputType::Controller);

}

void BindMenuCommands(dae::InputManager& inputManager)
{
    UnBindPlayerCommands(inputManager);
    UnBindMenuCommands(inputManager);
    UnBindNameCommands(inputManager);
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

void BindKeyBordNameCommands(dae::InputManager& inputManager, int playerId, InputType inputType) {
    inputManager.BindCommand(SDL_SCANCODE_W, KeyState::Up, std::make_unique<NavigateUpLetterCommand>(playerId), inputType);
    inputManager.BindCommand(SDL_SCANCODE_S, KeyState::Up, std::make_unique<NavigateDownLetterCommand>(playerId), inputType);
    inputManager.BindCommand(SDL_SCANCODE_D, KeyState::Up, std::make_unique<SelectOptionLetterCommand>(playerId), inputType);
    inputManager.BindCommand(SDL_SCANCODE_RETURN, KeyState::Up, std::make_unique<saveScoreCommand>(playerId), inputType);
}

void BindControllerNameCommands(dae::InputManager& inputManager, int playerId, InputType inputType, int controllerId = 0) {
    inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::DPadUp), KeyState::Up, std::make_unique<NavigateUpLetterCommand>(playerId), inputType, controllerId);
    inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::DPadDown), KeyState::Up, std::make_unique<NavigateDownLetterCommand>(playerId), inputType, controllerId);
    inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::X), KeyState::Up, std::make_unique<SelectOptionLetterCommand>(playerId), inputType, controllerId);
    inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::Y), KeyState::Up, std::make_unique<saveScoreCommand>(playerId), inputType, controllerId);
}

void BindNameCommands(dae::InputManager& inputManager)
{
    UnBindPlayerCommands(inputManager);
    UnBindMenuCommands(inputManager);
    const int numControllers = inputManager.GetConnectedControllerCount();
    const int numPlayers = GameData::GetInstance().GetNumberOfPlayers();

    BindKeyBordNameCommands(inputManager, 0, InputType::Keyboard);

    for (int playerId = 0; playerId < numPlayers; ++playerId) {
        // Bind controller commands
        if (numControllers > 1) {
            BindControllerNameCommands(inputManager, playerId, InputType::Controller, playerId);
        }
        else {
            BindControllerNameCommands(inputManager, playerId, InputType::Controller);
        }
    }
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
    auto character1textcomponent02 = std::make_unique<dae::TextComponent>("© 1982 1985 NAMCO LTD.", dae::ResourceManager::GetFont("arcade"), SDL_Color{ 255, 255, 255, 255 }, *Infocharacter1Txt02); 
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
    HighScoretextObject->SetLocalPosition(glm::vec3(1040, 50, 0.f));
    HighScoretextObject->AddComponent(std::move(titleTextComponent02));

    scene->Add(std::move(HighScoretextObject));

    auto HighScoreObject = std::make_unique<dae::GameObject>();
    // Retrieve the top score and convert it to a string
    uint32_t topScore = highScoresInstance.getTopScore();
    std::string topScoreStr = std::to_string(topScore);

    // Create the high score value text component using the string
    auto scoreTextComponent = std::make_unique<dae::TextComponent>(topScoreStr, dae::ResourceManager::GetFont("arcade"), SDL_Color{ 255, 255, 255, 255 }, *HighScoreObject);
    HighScoreObject->SetLocalPosition(glm::vec3(1040, 70, 0.f));
    HighScoreObject->AddComponent(std::move(scoreTextComponent));

    // Add HighScoreObject to the scene (assuming 'this' is a scene class)
    scene->Add(std::move(HighScoreObject));

    glm::vec3 initialPositionpoints(1040, 235, 0.f);

    for (size_t i = 0; i < players.size(); ++i) {
        auto player = players[i];
        if (auto scoreComponent = player->GetComponent<dae::PointComponent>()) {
	        float distanceBetweenPoints = 50.0f;
	        auto pointsDisplayObject = std::make_unique<dae::GameObject>();
            auto pointsDisplayComponent = std::make_unique<dae::PointsDisplayComponent>(dae::ResourceManager::GetFont("arcade"), *pointsDisplayObject);

            // Attach the points display component to the player's point component
            pointsDisplayComponent->AttachToPointComponent(scoreComponent);
            pointsDisplayObject->AddComponent(std::move(pointsDisplayComponent));

            // Calculate the position of the points display object based on the index and distance
            glm::vec3 position = initialPositionpoints + glm::vec3(0.0f, i * distanceBetweenPoints, 0.0f);
            pointsDisplayObject->SetLocalPosition(position);

            scene->Add(std::move(pointsDisplayObject));
        }
    }


    glm::vec3 initialPositionHealth(1040, 535, 0.f);

    for (size_t i = 0; i < players.size(); ++i) {
        auto player = players[i];
        if (auto healthComponent = player->GetComponent<dae::HealthComponent>()) {
	        float distanceBetweenLives = 50.0f;
	        auto livesDisplayObject = std::make_unique<dae::GameObject>();
            glm::vec3 position = initialPositionHealth + glm::vec3(0.0f, i * distanceBetweenLives, 0.0f);
            livesDisplayObject->SetLocalPosition(position);

            auto livesDisplayComponent = std::make_unique<dae::LivesDisplayComponent>(*livesDisplayObject,50,50,"Lives.png");
            // Attach the lives display component to the player's health component
            livesDisplayComponent->AttachToHealthComponent(healthComponent);
            // Calculate the position of the lives display object based on the index and distance
            livesDisplayObject->AddComponent(std::move(livesDisplayComponent));
            // Add the lives display object to the scene
            scene->Add(std::move(livesDisplayObject));
        }
    }
}

void loadInputScore(dae::Scene* scene)
{
    auto& inputManager = dae::InputManager::GetInstance();

    constexpr glm::vec3 initialPositionNameInput(630, 400, 0.f);
    constexpr glm::vec3 initialPositionScore(635, 350, 0.f);

    for (int i = 0; i < GameData::GetInstance().GetNumberOfPlayers(); ++i)
    {
	    constexpr float distanceBetweenScore = 150.0f;
	    constexpr float distanceBetweenInput = 150.0f;
	    auto nameInputObject = std::make_unique<dae::GameObject>();
        glm::vec3 positionInput = initialPositionNameInput + glm::vec3(0.0f, i * distanceBetweenInput, 0.0f);
        nameInputObject->SetLocalPosition(positionInput);

        std::unique_ptr<SelectNameComponent> nameInputComponent = std::make_unique<SelectNameComponent>(nameInputObject.get(), 6, dae::ResourceManager::GetFont("arcade"), SDL_Color{ 255, 255, 255, 255 }, i);
        nameInputObject->AddComponent(std::move(nameInputComponent));
        scene->Add(std::move(nameInputObject));

        auto ScoreObject = std::make_unique<dae::GameObject>();
        auto ScoreTextComponent = std::make_unique<dae::TextComponent>(std::to_string(GameData::GetInstance().GetPlayerData(i).score), dae::ResourceManager::GetFont("arcade"), SDL_Color{ 255, 0, 0, 255 }, *ScoreObject); // Specify color here
        ScoreTextComponent->SetColor(SDL_Color{ 255, 255, 0, 255 });
        ScoreObject->AddComponent(std::move(ScoreTextComponent));
        glm::vec3 position = initialPositionScore + glm::vec3(0.0f, i * distanceBetweenScore, 0.0f);
        ScoreObject->SetLocalPosition(position);
        scene->Add(std::move(ScoreObject));
    }

    // Create GameObject for Title
    auto TitleObject02 = std::make_unique<dae::GameObject>();
    auto titleTextComponent02 = std::make_unique<dae::TextComponent>("DIG DUG", dae::ResourceManager::GetFont("TITLEBig"), SDL_Color{ 0, 0, 255, 255 }, *TitleObject02);
    TitleObject02->SetLocalPosition(glm::vec3(635, 70, 0.f));
    TitleObject02->AddComponent(std::move(titleTextComponent02));
    scene->Add(std::move(TitleObject02));

    // Create GameObject for Title
    auto TitleObject = std::make_unique<dae::GameObject>();
    auto titleTextComponent = std::make_unique<dae::TextComponent>("DIG DUG", dae::ResourceManager::GetFont("TITLE"), SDL_Color{ 247, 190, 57, 255 }, *TitleObject);
    TitleObject->SetLocalPosition(glm::vec3(635, 70, 0.f));
    TitleObject->AddComponent(std::move(titleTextComponent));
    scene->Add(std::move(TitleObject));

    BindNameCommands(inputManager);
}

void GameScene(dae::Scene* scene)
{
    auto& inputManager = dae::InputManager::GetInstance();

    // Create GameObject for FPS counter
    auto fpsCounterObject = std::make_unique<dae::GameObject>();
    auto fpsTextComponent = std::make_unique<dae::TextComponent>("FPS: ", dae::ResourceManager::GetFont("font"), SDL_Color{ 252, 157, 3, 255 }, *fpsCounterObject);
    fpsCounterObject->AddComponent(std::move(fpsTextComponent));
    auto fpsTextComponentPtr = fpsCounterObject->GetComponent<dae::TextComponent>();
    auto fpsCounterComponent = std::make_unique<dae::FPSCounterComponent>(fpsTextComponentPtr);
    fpsCounterObject->AddComponent(std::move(fpsCounterComponent));
    fpsCounterObject->SetLocalPosition(glm::vec3(100.f, 20.f, 0.0f));
    scene->Add(std::move(fpsCounterObject));

    //constexpr glm::vec3 startPos(335, 70, 0.0f);
    //constexpr glm::vec2 mapScale(40, 26.f);

    //const LoadMap loadMap("../Data/maps/map1.map", "../Data/maps/" + Ingrediantmap);
    //SceneHelpers::LoadMapIntoScene(loadMap, scene, startPos, mapScale);

   // SceneHelpers::LoadIngMapIntoScene(loadMap, scene, startPos, mapScale);


    int score;
    int lives;
    auto PlayerObject = std::make_unique<dae::GameObject>();
    if (GameData::GetInstance().GetPlayerData(0).score != 0)
        score = GameData::GetInstance().GetPlayerData(0).score;
    else
        score = 0;

    auto Character1points = std::make_unique<dae::PointComponent>(score);
    PlayerObject->AddComponent(std::move(Character1points));

    if (GameData::GetInstance().GetPlayerData(0).lives != 3)
        lives = GameData::GetInstance().GetPlayerData(0).lives;
    else
        lives = 3;

    auto Character1Health = std::make_unique<dae::HealthComponent>(100, lives);
    PlayerObject->AddComponent(std::move(Character1Health));

    auto spriteRenderComponent = std::make_unique<dae::SpriteRendererComponent>(PlayerObject.get(), dae::ResourceManager::GetSprite("Player"));
    spriteRenderComponent->SetDimensions(40, 40);
    PlayerObject->AddComponent(std::move(spriteRenderComponent));

    auto animationComponent = std::make_unique<dae::AnimationComponent>(PlayerObject.get(), PlayerObject->GetComponent<dae::SpriteRendererComponent>(), "Idle");
    animationComponent->Play("Walk_Right", true);
    PlayerObject->AddComponent(std::move(animationComponent));
    PlayerObject->SetLocalPosition(glm::vec3(100, 100, 0.0f));

    auto hitBox = std::make_unique<HitBox>(glm::vec2(40, 40));
    hitBox->SetGameObject(PlayerObject.get());
    PlayerObject->AddComponent(std::move(hitBox));

    auto PlayerComponent = std::make_unique<game::Player>(PlayerObject.get());
    PlayerObject->AddComponent(std::move(PlayerComponent));


    dae::SceneData::GetInstance().AddGameObject(PlayerObject.get(), dae::GameObjectType::Player);

    scene->Add(std::move(PlayerObject));

    HandlePlayerInput(inputManager, 0);
    LoadUi(scene);

}

void load()
{
    BindExtraControlls(dae::InputManager::GetInstance());
    loadResources();
    auto& sceneManager = dae::SceneManager::GetInstance();

    const auto& startMenuScene = sceneManager.CreateScene("StartMenu");
    const auto& ScoreBoardScene = sceneManager.CreateScene("ScoreboardScene");
    const auto& Game = sceneManager.CreateScene("Game");
    const auto& SaveSoreScene = sceneManager.CreateScene("SaveScoreScene");


    startMenuScene->SetOnActivateCallback([startMenuScene]() { LoadStartMenu(startMenuScene); });
    ScoreBoardScene->SetOnActivateCallback([ScoreBoardScene]() {LoadScoreboard(ScoreBoardScene); });
    Game->SetOnActivateCallback([Game]() { GameScene(Game); });
    SaveSoreScene->SetOnActivateCallback([SaveSoreScene]() { loadInputScore(SaveSoreScene); });


    sceneManager.SetActiveScene("StartMenu");
}

int main(int, char* []) {
    dae::minigin engine("../Data/");
    engine.Run(load);
    return 0;
}