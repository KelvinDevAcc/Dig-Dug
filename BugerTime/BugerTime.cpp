#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "AnimationComponent.h"
#include "FPSCounterComponent.h"
#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "GameObject.h"
#include "Command.h"
#include "GameData.h"
#include "InputManager.h"
#include "HealthComponent.h"
#include "HighScores.h"
#include "HitBox.h"
#include "LivesDisplayComponent.h"
#include "LoadMap.h"
#include "Player.h"
#include "PointsDisplayComponent.h"
#include "SceneData.h"
#include "SceneHelpers.h"
#include "sdl_sound_system.h"
#include "SelectNameComponent.h"
#include "servicelocator.h"
#include "../3rdParty/imgui-1.90.4/imgui.h"

void LoadResources()
{
#if _DEBUG
    servicelocator::register_sound_system(
        std::make_unique<logging_sound_system>(std::make_unique<sdl_sound_system>()));
#else
    servicelocator::register_sound_system(std::make_unique<sdl_sound_system>());
#endif

    auto& ss = servicelocator::get_sound_system();

    ss.setVolume(10.f);

     //Register the sound files with their corresponding IDs
    ss.register_sound_file("../Data/Sounds/0_burger_going_down.mp3");
    ss.register_sound_file("../Data/Sounds/1_burger_touching_floor.mp3");
    ss.register_sound_file("../Data/Sounds/2_die.mp3");
    ss.register_sound_file("../Data/Sounds/3_high_scores.mp3");
    ss.register_sound_file("../Data/Sounds/4_entry_selected.mp3");
    ss.register_sound_file("../Data/Sounds/5_coin_inserted.mp3");
    ss.register_sound_file("../Data/Sounds/6_one_up.mp3");
    ss.register_sound_file("../Data/Sounds/7_enemy_crushed.mp3");
    ss.register_sound_file("../Data/Sounds/8_enemy_fall.mp3");
    ss.register_sound_file("../Data/Sounds/9_pepper_up.mp3");
    ss.register_sound_file("../Data/Sounds/10_level_intro.mp3");
    ss.register_sound_file("../Data/Sounds/11_main.mp3");
    ss.register_sound_file("../Data/Sounds/13_new_ingredient.mp3");
    ss.register_sound_file("../Data/Sounds/14_pepper.mp3");
    ss.register_sound_file("../Data/Sounds/15_peppered.mp3");
    ss.register_sound_file("../Data/Sounds/16_stepping_on_burger.mp3");
    ss.register_sound_file("../Data/Sounds/17_win.mp3");
    ss.register_sound_file("../Data/Sounds/18_menu_select.mp3");
    ss.register_sound_file("../Data/Sounds/19_burgermenu.mp3");
    ss.register_sound_file("../Data/Sounds/20_menu_move.mp3");

    // Load each sound file once
    ss.load_sound(0, "../Data/Sounds/0_burger_going_down.mp3");
    ss.load_sound(1, "../Data/Sounds/1_burger_touching_floor.mp3");
    ss.load_sound(2, "../Data/Sounds/2_die.mp3");
    ss.load_sound(3, "../Data/Sounds/3_high_scores.mp3");
    ss.load_sound(4, "../Data/Sounds/4_entry_selected.mp3");
    ss.load_sound(5, "../Data/Sounds/5_coin_inserted.mp3");
    ss.load_sound(6, "../Data/Sounds/6_one_up.mp3");
    ss.load_sound(7, "../Data/Sounds/7_enemy_crushed.mp3");
    ss.load_sound(8, "../Data/Sounds/8_enemy_fall.mp3");
    ss.load_sound(9, "../Data/Sounds/9_pepper_up.mp3");
    ss.load_sound(10, "../Data/Sounds/10_level_intro.mp3");
    ss.load_sound(11, "../Data/Sounds/11_main.mp3", true);
    ss.load_sound(13, "../Data/Sounds/13_new_ingredient.mp3");
    ss.load_sound(14, "../Data/Sounds/14_pepper.mp3");
    ss.load_sound(15, "../Data/Sounds/15_peppered.mp3");
    ss.load_sound(16, "../Data/Sounds/16_stepping_on_burger.mp3");
    ss.load_sound(17, "../Data/Sounds/17_win.mp3");
    ss.load_sound(18, "../Data/Sounds/18_menu_select.mp3");
    ss.load_sound(19, "../Data/Sounds/19_burgermenu.mp3", true);
    ss.load_sound(20, "../Data/Sounds/20_menu_move.mp3");



	dae::ResourceManager::LoadFont("Lingua", "Lingua.otf", 36);
	dae::ResourceManager::LoadFont("Linguasmall", "Lingua.otf", 26);
	dae::ResourceManager::LoadFont("arcade", "arcade-legacy.ttf", 20);
	dae::ResourceManager::LoadFont("arcadeBig", "arcade-legacy.ttf", 40);
	dae::ResourceManager::LoadFont("PressStart", "PressStart2P-vaV7.ttf", 20);

	dae::ResourceManager::LoadSprite("background", "background.tga");
	dae::ResourceManager::LoadSprite("logo", "logo.tga");

	dae::ResourceManager::LoadSprite("Stage01", "Stage01.png");

    dae::ResourceManager::LoadSprite("floor", "floor.png");
    dae::ResourceManager::LoadSprite("ladder_down", "go_down.png");
    dae::ResourceManager::LoadSprite("ladder_up", "go_up.png");
    dae::ResourceManager::LoadSprite("stairs", "stairs.png");
    dae::ResourceManager::LoadSprite("right_basket", "right_basket.png");
    dae::ResourceManager::LoadSprite("mid_basket", "mid_basket.png");
    dae::ResourceManager::LoadSprite("left_basket", "left_basket.png");
    dae::ResourceManager::LoadSprite("combined_basket", "combined_basket.png");


    dae::ResourceManager::LoadSprite("Bugertop", "Bugertop.png");
    dae::ResourceManager::LoadSprite("cheese", "cheese.png");
    dae::ResourceManager::LoadSprite("Meat", "Meat.png");
    dae::ResourceManager::LoadSprite("tomato", "tomato.png");
    dae::ResourceManager::LoadSprite("lettuce", "lettuce.png");
    dae::ResourceManager::LoadSprite("Buger_bottom", "Buger_bottom.png");


    dae::ResourceManager::LoadSprite("chef",
        "Arcade - Burger Time - Characters & Objects-white.png",
        11,  // rowCount
        15,  // colCount
        {
            { "Idle", { { { 1, 0 }}, 1 } },
            { "Walk_Right", { { { 3, 0 }, { 4, 0 }, { 5, 0 } }, 4 } },
            { "Walk_Left", { { { 3, 0 }, { 4, 0 }, { 5, 0 } }, 4 } },
            { "Walk_Up", { { { 6, 0 }, { 7, 0 }, { 8, 0 } }, 4 } },
            { "Walk_Down", { { { 0, 0 }, { 1, 0 }, { 2, 0 } }, 4 } },
            { "Dying", { { { 3, 1 }, { 4, 1 }, { 5, 1 }, { 6, 1 }, { 7, 1 }, { 8, 1 } }, 2 } },
            { "Attacking", { { { 1, 1 } }, 1 } },
            { "Victory", { { { 3, 1 }, { 1, 0 }}, 3 } }

        });
    dae::ResourceManager::LoadSprite("egg",
        "Arcade - Burger Time - Characters & Objects-white.png",
        11,  // rowCount
        15,  // colCount
        {
            { "Idle", { { { 1, 6 },{ 1, 6 }}, 1 } },
            { "Walk_Right", { { { 2, 6 }, { 3, 6 } }, 2 } },
            { "Walk_Left", { { { 2, 6 }, { 3, 6 } }, 2 } },
            { "Walk_Up", { { { 4, 6 }, { 5, 6 } }, 2 } },
            { "Walk_Down", { { { 0, 6 }, { 1, 6 } }, 2 } },
            { "Dying", { { { 0, 7 }, { 1, 7 }, { 2, 7 }, { 3, 7 } }, 1 } },
            { "Stunned", { { { 4, 7 }, { 5, 7 } }, 1 } }
        });
    dae::ResourceManager::LoadSprite("sausage",
        "Arcade - Burger Time - Characters & Objects-white.png",
        11,  // rowCount
        15,  // colCount
        {
            { "Idle", { { { 1, 2 }}, 1 } },
            { "Walk_Right", { { { 2, 2 }, { 3, 2 } }, 2 } },
            { "Walk_Left", { { { 2, 2 }, { 3, 2 } }, 2 } },
            { "Walk_Up", { { { 4, 2 }, { 5, 2 } }, 2 } },
            { "Walk_Down", { { { 0, 2 }, { 1, 2 } }, 2 } },
            { "Dying", { { { 0, 3 }, { 1, 3 }, { 2, 3 }, { 3, 3 } }, 1 } },
            { "Stunned", { { { 4, 3 }, { 5, 3 } }, 1 } }
        });
    dae::ResourceManager::LoadSprite("Pickle",
        "Arcade - Burger Time - Characters & Objects-white.png",
        11,  // rowCount
        15,  // colCount
        {
            { "Idle", { { { 1, 4 }}, 1 } },
            { "Walk_Right", { { { 2, 4 }, { 3, 4 } }, 2 } },
            { "Walk_Left", { { { 2, 4 }, { 3, 4 } }, 2 } },
            { "Walk_Up", { { { 4, 4 }, { 5, 4 } }, 2 } },
            { "Walk_Down", { { { 0, 4 }, { 1, 4 } }, 2 } },
            { "Dying", { { { 0, 5 }, { 1, 5 }, { 2, 5 }, { 3, 5 } }, 1 } },
            { "Stunned", { { { 4, 5 }, { 5, 5 } }, 1 } }
        });

    dae::ResourceManager::LoadSprite("Pepper",
        "Arcade - Burger Time - Characters & Objects-white.png",
        11,  // rowCount
        15,  // colCount
        {
            { "ShakeCloud", { { { 11, 1 }, {12,1}, {13,1}, {14,1}}, 1 } },
            { "ShakeCloud2", { { { 11, 2 }, {12,2}, {13,2}, {14,2}}, 1 } }
        });

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


void BindKeyBordNameCommands(dae::InputManager& inputManager, int playerId, InputType inputType) {
    inputManager.BindCommand(SDL_SCANCODE_W, KeyState::Up, std::make_unique<NavigateUpLetterCommand>(playerId), inputType);
    inputManager.BindCommand(SDL_SCANCODE_S, KeyState::Up, std::make_unique<NavigateDownLetterCommand>(playerId), inputType);
    inputManager.BindCommand(SDL_SCANCODE_D, KeyState::Up, std::make_unique<SelectOptionLetterCommand>(playerId), inputType);
    inputManager.BindCommand(SDL_SCANCODE_RETURN, KeyState::Up, std::make_unique<saveScoreCommand>(playerId), inputType);
}

void BindControllerNameCommands(dae::InputManager& inputManager, int playerId, int controllerId = 0) {
    inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::DPadUp), KeyState::Up, std::make_unique<NavigateUpLetterCommand>(playerId), InputType::Controller, controllerId);
    inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::DPadDown), KeyState::Up, std::make_unique<NavigateDownLetterCommand>(playerId), InputType::Controller, controllerId);
    inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::X), KeyState::Up, std::make_unique<SelectOptionLetterCommand>(playerId), InputType::Controller, controllerId);
    inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::Y), KeyState::Up, std::make_unique<saveScoreCommand>(playerId), InputType::Controller, controllerId);
}

void BindNameCommands(dae::InputManager& inputManager)
{
    const int numControllers = inputManager.GetConnectedControllerCount();
    const int numPlayers = GameData::GetInstance().GetNumberOfPlayers();

    BindKeyBordNameCommands(inputManager, 0, InputType::Keyboard);

	for (int playerId = 0; playerId < numPlayers; ++playerId) {
        // Bind keyboard commands

        // Bind controller commands
        if (numControllers > 1) {
            BindControllerNameCommands(inputManager, playerId, playerId);
        }
        else {
            BindControllerNameCommands(inputManager, playerId);
        }
    }
}

void BindMenuCommands(dae::InputManager& inputManager)
{
    // Bind keyboard commands
    inputManager.BindCommand(SDL_SCANCODE_W, KeyState::Up, std::make_unique<NavigateUpCommand>(), InputType::Keyboard);
    inputManager.BindCommand(SDL_SCANCODE_S, KeyState::Up, std::make_unique<NavigateDownCommand>(), InputType::Keyboard);
    inputManager.BindCommand(SDL_SCANCODE_RETURN, KeyState::Up, std::make_unique<SelectOptionCommand>(), InputType::Keyboard);

    // Bind controller commands
    inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::DPadUp), KeyState::Up, std::make_unique<NavigateUpCommand>(), InputType::Controller);
    inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::DPadDown), KeyState::Up, std::make_unique<NavigateDownCommand>(), InputType::Controller);
    inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::Y), KeyState::Up, std::make_unique<SelectOptionCommand>(), InputType::Controller);
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
void UnBindMenuCommands(dae::InputManager& inputManager)
{
    // Unbind keyboard commands
    inputManager.UnbindCommand(SDL_SCANCODE_W, KeyState::Up, InputType::Keyboard);
    inputManager.UnbindCommand(SDL_SCANCODE_S, KeyState::Up, InputType::Keyboard);
    inputManager.UnbindCommand(SDL_SCANCODE_RETURN, KeyState::Up, InputType::Keyboard);

    // Unbind controller commands
    inputManager.UnbindCommand(GameController::GetButtonMapping(GameController::Button::DPadUp), KeyState::Up, InputType::Controller);
    inputManager.UnbindCommand(GameController::GetButtonMapping(GameController::Button::DPadDown), KeyState::Up, InputType::Controller);
    inputManager.UnbindCommand(GameController::GetButtonMapping(GameController::Button::X), KeyState::Up, InputType::Controller);
    inputManager.UnbindCommand(GameController::GetButtonMapping(GameController::Button::Y), KeyState::Up, InputType::Controller);

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


    glm::vec3 initialPositionPepper(100, 170, 0.f);
    // Define the distance between points display objects
    float distanceBetweenPepper = 150.0f;

    for (size_t i = 0; i < players.size(); ++i) {
        auto player = players[i];
        if (auto scoreComponent = player->GetComponent<dae::PointComponent>()) {
            auto PepertextObject = std::make_unique<dae::GameObject>();

            auto PeperComponent = std::make_unique<dae::TextComponent>(
                "PEPPER",
                dae::ResourceManager::GetFont("arcade"),
                SDL_Color{ 0, 255, 0, 255 },
                *PepertextObject
            );
            glm::vec3 position = initialPositionPepper + glm::vec3(0.0f, i * distanceBetweenPepper, 0.0f);
            PepertextObject->SetLocalPosition(glm::vec3(position));
            PepertextObject->AddComponent(std::move(PeperComponent));

            scene->Add(std::move(PepertextObject));

            auto PeperscoretextObject = std::make_unique<dae::GameObject>();

            auto PeperscoreComponent = std::make_unique<dae::TextComponent>(
                "4",
                dae::ResourceManager::GetFont("arcade"),
                SDL_Color{ 255, 255, 255, 255 },
                *PeperscoretextObject
            );
            glm::vec3 positionscore = initialPositionPepper + glm::vec3(0.0f, i * distanceBetweenPepper + 20, 0.0f);
            PeperscoretextObject->SetLocalPosition(positionscore);
            PeperscoretextObject->AddComponent(std::move(PeperscoreComponent));

            scene->Add(std::move(PeperscoretextObject));
        }
    }

   


    glm::vec3 initialPositionpoints(100, 235, 0.f);
    // Define the distance between points display objects
    float distanceBetweenPoints = 50.0f;

    for (size_t i = 0; i < players.size(); ++i) {
        auto player = players[i];
        if (auto scoreComponent = player->GetComponent<dae::PointComponent>()) {
            auto pointsDisplayObject = std::make_unique<dae::GameObject>();
            auto pointsDisplayComponent = std::make_unique<dae::PointsDisplayComponent>(dae::ResourceManager::GetFont("arcade"), *pointsDisplayObject);

            // Attach the points display component to the player's point component
            pointsDisplayComponent->AttachToPointComponent(scoreComponent);
            pointsDisplayObject->AddComponent(std::move(pointsDisplayComponent));

            // Calculate the position of the points display object based on the index and distance
            glm::vec3 position = initialPositionpoints + glm::vec3(0.0f, i * distanceBetweenPoints, 0.0f);
            pointsDisplayObject->SetLocalPosition(position);

            // Add the points display object to the scene
            scene->Add(std::move(pointsDisplayObject));
        }
    }


    glm::vec3 initialPositionHealth(100, 215, 0.f);
    // Define the distance between lives display objects
    float distanceBetweenLives = 50.0f;

    for (size_t i = 0; i < players.size(); ++i) {
        auto player = players[i];
        if (auto healthComponent = player->GetComponent<dae::HealthComponent>()) {
            auto livesDisplayObject = std::make_unique<dae::GameObject>();
            auto livesDisplayComponent = std::make_unique<dae::LivesDisplayComponent>(dae::ResourceManager::GetFont("arcade"), *livesDisplayObject);

            // Attach the lives display component to the player's health component
            livesDisplayComponent->AttachToHealthComponent(healthComponent);
            livesDisplayObject->AddComponent(std::move(livesDisplayComponent));

            // Calculate the position of the lives display object based on the index and distance
            glm::vec3 position = initialPositionHealth + glm::vec3(0.0f, i * distanceBetweenLives, 0.0f);
            livesDisplayObject->SetLocalPosition(position);

            // Add the lives display object to the scene
            scene->Add(std::move(livesDisplayObject));
        }
    }
}

void LoadStartMenu(dae::Scene* startMenuScene)
{
    auto& inputManager = dae::InputManager::GetInstance();
    UnBindMenuCommands(inputManager);

    std::vector<std::string> options = { "single player", "multiplayer", "versus Mode", "scoreBoard" };
    std::vector<std::function<void()>> callbacks =
    {
        []() {

            GameData::GetInstance().SetGameState(GameData::GameState::SINGLE_PLAYER);
            dae::SceneManager::GetInstance().SetActiveScene("Scene4");
        },
        []() {
            GameData::GetInstance().SetGameState(GameData::GameState::MULTIPLAYER);
            dae::SceneManager::GetInstance().SetActiveScene("Scene4");
        },
        []() {
            GameData::GetInstance().SetGameState(GameData::GameState::VERSUS);
            dae::SceneManager::GetInstance().SetActiveScene("Scene4");
        },
        []() {
            dae::SceneManager::GetInstance().SetActiveScene("ScoreboardScene");
        }
    };


    // Create the GameObject for the menu
    auto menuObject = std::make_unique<dae::GameObject>();
    menuObject->SetLocalPosition(glm::vec3(635, 300, 0.f));
    auto menuComponent = std::make_unique<dae::MenuComponent>(menuObject.get(), options, callbacks, dae::ResourceManager::GetFont("arcadeBig"), 70.0f);
    menuComponent->SetTextColor(SDL_Color{ 220,200,100,255 });
    menuObject->AddComponent(std::move(menuComponent));

    // Add the menu GameObject to the scene
    startMenuScene->Add(std::move(menuObject));


    // Create GameObject for Title
    auto TitleObject02 = std::make_unique<dae::GameObject>();
    auto titleTextComponent02 = std::make_unique<dae::TextComponent>("BURGERTIME", dae::ResourceManager::GetFont("Lingua"), SDL_Color{ 255, 0, 0, 255 }, *TitleObject02); // Pass the GameObject reference here
    TitleObject02->SetLocalPosition(glm::vec3(635, 70, 0.f));
    TitleObject02->AddComponent(std::move(titleTextComponent02));
    startMenuScene->Add(std::move(TitleObject02));

    auto Infocharacter1Txt02 = std::make_unique<dae::GameObject>();
    auto character1textcomponent02 = std::make_unique<dae::TextComponent>("@ CORP 1982 DATA EAST INC", dae::ResourceManager::GetFont("Lingua"), SDL_Color{ 255, 0, 0, 255 }, *Infocharacter1Txt02); // Specify color here
    Infocharacter1Txt02->AddComponent(std::move(character1textcomponent02));
    Infocharacter1Txt02->SetLocalPosition(glm::vec3(635, 140, 0.f));
    startMenuScene->Add(std::move(Infocharacter1Txt02));

    auto Infocharacter2Txt02 = std::make_unique<dae::GameObject>();
    auto character2textcomponent02 = std::make_unique<dae::TextComponent>("MFGD BY BALLY MIDWAY MFG, CO,", dae::ResourceManager::GetFont("Lingua"), SDL_Color{ 255, 0, 0, 255 }, *Infocharacter2Txt02); // Specify color here
    Infocharacter2Txt02->AddComponent(std::move(character2textcomponent02));
    Infocharacter2Txt02->SetLocalPosition(glm::vec3(635, 170, 0.f));
    startMenuScene->Add(std::move(Infocharacter2Txt02));

    startMenuScene->SetBackgroundMusic(11);
    BindMenuCommands(inputManager);

}

void LoadScoreboard(dae::Scene* ScoreBoardScene)
{
    auto& inputManager = dae::InputManager::GetInstance();
    UnBindNameCommands(inputManager);
    UnBindMenuCommands(inputManager);

    auto& highscore = HighScores::GetInstance();

    const auto& scores = highscore.getHighScores();

    for (size_t i = 0; i < scores.size(); ++i) {
        const auto& playerName = scores[i].first;
        const auto score = scores[i].second;

        auto gameObject = std::make_unique<dae::GameObject>();

        std::string text = playerName.data() + std::string(": ") + std::to_string(score);
        auto textComponent = std::make_unique<dae::TextComponent>(text, dae::ResourceManager::GetFont("arcadeBig"), SDL_Color{ 255, 0, 0, 255 }, *gameObject);

        gameObject->SetLocalPosition(glm::vec3(635, 190 + i * 50, 0.f));

        gameObject->AddComponent(std::move(textComponent));

        ScoreBoardScene->Add(std::move(gameObject));
    }


    // Create GameObject for Title
    auto TitleObject02 = std::make_unique<dae::GameObject>();
    auto titleTextComponent02 = std::make_unique<dae::TextComponent>("BURGERTIME", dae::ResourceManager::GetFont("arcadeBig"), SDL_Color{ 255, 0, 0, 255 }, *TitleObject02); // Pass the GameObject reference here
    TitleObject02->SetLocalPosition(glm::vec3(635, 70, 0.f));
    TitleObject02->AddComponent(std::move(titleTextComponent02));
    ScoreBoardScene->Add(std::move(TitleObject02));


    std::vector<std::string> options2 = { "back to menu"};
    std::vector<std::function<void()>> callbacks2 =
    {
        []() { dae::SceneManager::GetInstance().SetActiveScene("StartMenu"); },
    };

    // Create the GameObject for the menu
    auto menuObject = std::make_unique<dae::GameObject>();
    menuObject->SetLocalPosition(glm::vec3(635, 600, 0.f));
    auto menuComponent = std::make_unique<dae::MenuComponent>(menuObject.get(), options2, callbacks2, dae::ResourceManager::GetFont("arcadeBig"), 70.0f);
    menuComponent->SetTextColor(SDL_Color{ 220,200,100,255 });
    menuObject->AddComponent(std::move(menuComponent));
    ScoreBoardScene->Add(std::move(menuObject));

    BindMenuCommands(inputManager);
}


void loadInputScore(dae::Scene* scene)
{
    auto& inputManager = dae::InputManager::GetInstance();
    UnBindPlayerCommands(inputManager);

    const glm::vec3 initialPositionNameInput(630, 300, 0.f);
    const glm::vec3 initialPositionScore(635, 150, 0.f);

	const float distanceBetweenInput = 150.0f;
    const float distanceBetweenScore = 150.0f;

    for (int i = 0; i  < GameData::GetInstance().GetNumberOfPlayers(); ++i)
    {
        auto nameInputObject = std::make_unique<dae::GameObject>();
        glm::vec3 positionInput = initialPositionNameInput + glm::vec3(0.0f, i * distanceBetweenInput, 0.0f);
        nameInputObject->SetLocalPosition(positionInput);

        std::unique_ptr<SelectNameComponent> nameInputComponent = std::make_unique<SelectNameComponent>(nameInputObject.get(), 6, dae::ResourceManager::GetFont("arcadeBig"), SDL_Color{ 255, 255, 255, 255 }, i);
        nameInputObject->AddComponent(std::move(nameInputComponent));
        scene->Add(std::move(nameInputObject));

        auto ScoreObject = std::make_unique<dae::GameObject>();
        auto ScoreTextcomponent = std::make_unique<dae::TextComponent>(std::to_string(GameData::GetInstance().GetPlayerData(i).score), dae::ResourceManager::GetFont("arcadeBig"), SDL_Color{ 255, 0, 0, 255 }, *ScoreObject); // Specify color here
        ScoreTextcomponent->SetColor(SDL_Color{ 255, 255, 0, 255 });
        ScoreObject->AddComponent(std::move(ScoreTextcomponent));
        glm::vec3 position = initialPositionScore + glm::vec3(0.0f, i * distanceBetweenScore, 0.0f);
        ScoreObject->SetLocalPosition(position);
        scene->Add(std::move(ScoreObject));
    }

    auto TitleObject02 = std::make_unique<dae::GameObject>();
    auto titleTextComponent02 = std::make_unique<dae::TextComponent>("BURGERTIME", dae::ResourceManager::GetFont("Lingua"), SDL_Color{ 255, 0, 0, 255 }, *TitleObject02); // Pass the GameObject reference here
    TitleObject02->SetLocalPosition(glm::vec3(635, 70, 0.f));
    TitleObject02->AddComponent(std::move(titleTextComponent02));
    scene->Add(std::move(TitleObject02));

   

    scene->SetBackgroundMusic(11);

    BindNameCommands(inputManager);
}

void Scene4(dae::Scene* scene)
{
    auto& inputManager = dae::InputManager::GetInstance();
    UnBindMenuCommands(inputManager);

    constexpr glm::vec3 startPos(335, 70, 0.0f);
    constexpr glm::vec2 mapScale(40, 26.f);

    std::string Ingrediantmap = "";
    switch (GameData::GetInstance().GetGameState()) {
    case GameData::GameState::SINGLE_PLAYER:
        Ingrediantmap = "map1.ingmap";
        break;
    case GameData::GameState::MULTIPLAYER:
        Ingrediantmap = "map1.2.ingmap";
       
        break;
    case GameData::GameState::VERSUS:
        Ingrediantmap = "map1.3.ingmap";
        break;
    default:
        Ingrediantmap = "map1.ingmap";
        break;
    }
    const LoadMap loadMap("../Data/maps/map1.map", "../Data/maps/" + Ingrediantmap);
    SceneHelpers::LoadMapIntoScene(loadMap, scene, startPos, mapScale);

    SceneHelpers::LoadIngMapIntoScene(loadMap, scene, startPos, mapScale);

    if (GameData::GetInstance().GetGameState() == GameData::GameState::SINGLE_PLAYER)
    {
        HandlePlayerInput(inputManager, 0); 
    }
    else
    {
        HandlePlayerInput(inputManager, 0); 
        HandlePlayerInput(inputManager, 1); 
    }

    scene->SetBackgroundMusic(19);

}

void Scene5(dae::Scene* scene)
{
    auto& inputManager = dae::InputManager::GetInstance();
    UnBindPlayerCommands(inputManager);

    constexpr glm::vec3 startPos(335, 90, 0.0f);
    constexpr glm::vec2 mapScale(40, 26.f);

    std::string Ingrediantmap = "";
    switch (GameData::GetInstance().GetGameState()) {
    case GameData::GameState::SINGLE_PLAYER:
        Ingrediantmap = "map2.ingmap";
        break;
    case GameData::GameState::MULTIPLAYER:
        Ingrediantmap = "map2.2.ingmap";
        
        break;
    case GameData::GameState::VERSUS:
        Ingrediantmap = "map2.3.ingmap";
        break;
    default:
        Ingrediantmap = "map2.ingmap";
        break;
    }

    const LoadMap loadMap("../Data/maps/map2.map", "../Data/maps/" + Ingrediantmap);
    SceneHelpers::LoadMapIntoScene(loadMap, scene, startPos, mapScale);

    SceneHelpers::LoadIngMapIntoScene(loadMap, scene, startPos, mapScale);

    if (GameData::GetInstance().GetGameState() == GameData::GameState::SINGLE_PLAYER)
    {
        HandlePlayerInput(inputManager, 0); 
    }
    else
    {
        HandlePlayerInput(inputManager, 0); 
        HandlePlayerInput(inputManager, 1); 
    }
}

void Scene6(dae::Scene* scene)
{
    auto& inputManager = dae::InputManager::GetInstance();
    UnBindPlayerCommands(inputManager);

    constexpr glm::vec3 startPos(270, 80, 0.0f);
    constexpr glm::vec2 mapScale(40, 26.f);

    std::string Ingrediantmap = "";
    switch (GameData::GetInstance().GetGameState()) {
    case GameData::GameState::SINGLE_PLAYER:
        Ingrediantmap = "map3.ingmap";
        break;
    case GameData::GameState::MULTIPLAYER:
        Ingrediantmap = "map3.2.ingmap";
        break;
    case GameData::GameState::VERSUS:
        Ingrediantmap = "map3.3.ingmap";
        break;
    default:
        Ingrediantmap = "map3.ingmap";
        break;
    }
    const LoadMap loadMap("../Data/maps/map3.map", "../Data/maps/" + Ingrediantmap);
    SceneHelpers::LoadMapIntoScene(loadMap, scene, startPos, mapScale);

    SceneHelpers::LoadIngMapIntoScene(loadMap, scene, startPos, mapScale);

    if (GameData::GetInstance().GetGameState() == GameData::GameState::SINGLE_PLAYER)
    {
        HandlePlayerInput(inputManager, 0); 
    }
    else
    {
        HandlePlayerInput(inputManager, 0); 
        HandlePlayerInput(inputManager, 1); 
    }

}

void load()
{
    LoadResources();
    auto& sceneManager = dae::SceneManager::GetInstance();
    auto& inputmanager = dae::InputManager::GetInstance();
    auto& highscore = HighScores::GetInstance();

    BindExtraControlls(inputmanager);


    highscore.loadScores();

    const auto& startMenuScene = sceneManager.CreateScene("StartMenu");
    const auto& ScoreBoardScene = sceneManager.CreateScene("ScoreboardScene");
    const auto& scene4 = sceneManager.CreateScene("Scene4");
    const auto& scene5 = sceneManager.CreateScene("Scene5");
    const auto& scene6 = sceneManager.CreateScene("Scene6");
    const auto& SaveSoreScene = sceneManager.CreateScene("SaveScoreScene");



    startMenuScene->SetOnActivateCallback([startMenuScene]() { LoadStartMenu(startMenuScene); });
    ScoreBoardScene->SetOnActivateCallback([ScoreBoardScene]() {LoadScoreboard(ScoreBoardScene); });
    scene4->SetOnActivateCallback([scene4]() { Scene4(scene4); LoadUi(scene4); });
    scene5->SetOnActivateCallback([scene5]() { Scene5(scene5); LoadUi(scene5); });
    scene6->SetOnActivateCallback([scene6]() { Scene6(scene6); LoadUi(scene6); });
    SaveSoreScene->SetOnActivateCallback([SaveSoreScene]() { loadInputScore(SaveSoreScene); });


    sceneManager.SetActiveScene("StartMenu");

}

int main(int, char* []) {
    dae::Minigin engine("../Data/");
    engine.Run(load);
    return 0;
}