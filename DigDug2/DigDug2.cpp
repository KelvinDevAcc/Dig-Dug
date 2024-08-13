#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <random>

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
#include "EnemyComponent.h"
#include "ResourceManager.h"
#include "SceneData.h"
#include "SceneHelpers.h"
#include "SceneManager.h"
#include "sdl_sound_system.h"
#include "servicelocator.h"
#include "TextComponent.h"
#include "TweenManager.h"

void loadResources()
{
#if _DEBUG
    servicelocator::register_sound_system(std::make_unique<logging_sound_system>(std::make_unique<sdl_sound_system>()));
#else
    servicelocator::register_sound_system(std::make_unique<sdl_sound_system>());
#endif
    auto& ss = servicelocator::get_sound_system();

    ss.setVolume(1.f);
    ss.register_sound_file("../Data/Sounds/02-GameStartMusic.mp3");
    ss.register_sound_file("../Data/Sounds/12-HighScoreMusic.mp3");
    ss.register_sound_file("../Data/Sounds/13-NameEntryMusic.mp3");
    ss.register_sound_file("../Data/Sounds/18_menu_select.mp3");
    ss.register_sound_file("../Data/Sounds/20_menu_move.mp3");
    ss.register_sound_file("../Data/Sounds/DeathAnimation.mp3");
    ss.register_sound_file("../Data/Sounds/EnemyDeath.mp3");
    ss.register_sound_file("../Data/Sounds/GameOver.mp3");
    ss.register_sound_file("../Data/Sounds/PlayerHit.mp3");
    ss.register_sound_file("../Data/Sounds/PumpShoot.mp3");
    ss.register_sound_file("../Data/Sounds/PumpToEnemy.wav");
    ss.register_sound_file("../Data/Sounds/RockBroken.mp3");
    ss.register_sound_file("../Data/Sounds/RockHit.wav");
    ss.register_sound_file("../Data/Sounds/RockDropping.mp3");
    ss.register_sound_file("../Data/Sounds/Victory.wav");
    ss.register_sound_file("../Data/Sounds/walkmusic.wav");
    ss.register_sound_file("../Data/Sounds/12FygarFlame.mp3");

    // Load each sound file once
    ss.load_sound(20, "../Data/Sounds/02-GameStartMusic.mp3",true);
    ss.load_sound(1, "../Data/Sounds/12-HighScoreMusic.mp3", true);
    ss.load_sound(2, "../Data/Sounds/13-NameEntryMusic.mp3", true);
    ss.load_sound(3, "../Data/Sounds/DeathAnimation.mp3");
    ss.load_sound(4, "../Data/Sounds/EnemyDeath.mp3");
    ss.load_sound(5, "../Data/Sounds/GameOver.mp3");
    ss.load_sound(6, "../Data/Sounds/PlayerHit.mp3");
    ss.load_sound(7, "../Data/Sounds/PumpShoot.mp3");
    ss.load_sound(8, "../Data/Sounds/PumpToEnemy.wav");
    ss.load_sound(9, "../Data/Sounds/RockBroken.mp3");
    ss.load_sound(10, "../Data/Sounds/RockHit.wav");
    ss.load_sound(11, "../Data/Sounds/RockDropping.mp3");
    ss.load_sound(12, "../Data/Sounds/Victory.wav");
    ss.load_sound(13, "../Data/Sounds/walkmusic.wav" , true);
    ss.load_sound(15, "../Data/Sounds/18_menu_select.mp3");
    ss.load_sound(14, "../Data/Sounds/20_menu_move.mp3");
    ss.load_sound(15, "../Data/Sounds/12FygarFlame.mp3");

    dae::ResourceManager::LoadFont("arcadeclassic", "ARCADECLASSIC.TTF", 48);
    dae::ResourceManager::LoadFont("digdug", "DIG DUG.ttf", 48);
    dae::ResourceManager::LoadFont("font", "font.ttf", 28);
    dae::ResourceManager::LoadFont("TITLE", "font.ttf", 136);
    dae::ResourceManager::LoadFont("TITLEBig", "font.ttf", 144);
    dae::ResourceManager::LoadFont("arcadeBig", "arcade-legacy.ttf", 28);
    dae::ResourceManager::LoadFont("arcade", "arcade-legacy.ttf", 24);

    dae::ResourceManager::LoadTexture("lives","Lives.png");
    dae::ResourceManager::LoadTexture("sky", "spritesheetMap.png", SDL_Rect(64, 0, 8, 8));
    dae::ResourceManager::LoadTexture("skyFloor", "spritesheetMap.png", SDL_Rect(64, 8, 8, 8));
	dae::ResourceManager::LoadTexture("floorblock01","spritesheetMap.png",SDL_Rect(176,0,16,16));

	dae::ResourceManager::LoadTexture("floorblock02","spritesheetMap.png",SDL_Rect(56,0,8,8));
	dae::ResourceManager::LoadTexture("floorblock021","spritesheetMap.png",SDL_Rect(0,0,8,8));
	dae::ResourceManager::LoadTexture("floorblock022","spritesheetMap.png",SDL_Rect(8,0,8,8));
	dae::ResourceManager::LoadTexture("floorblock023","spritesheetMap.png",SDL_Rect(16,0,8,8));
	dae::ResourceManager::LoadTexture("floorblock024","spritesheetMap.png",SDL_Rect(24,0,8,8));
	dae::ResourceManager::LoadTexture("floorblock025","spritesheetMap.png",SDL_Rect(32,0,8,8));
	dae::ResourceManager::LoadTexture("floorblock026","spritesheetMap.png",SDL_Rect(48,0,8,8));

	dae::ResourceManager::LoadTexture("floorblock03","spritesheetMap.png",SDL_Rect(56,8,8,8));
    dae::ResourceManager::LoadTexture("floorblock031", "spritesheetMap.png", SDL_Rect(0, 8, 8, 8));
    dae::ResourceManager::LoadTexture("floorblock032", "spritesheetMap.png", SDL_Rect(8, 8, 8, 8));
    dae::ResourceManager::LoadTexture("floorblock033", "spritesheetMap.png", SDL_Rect(16, 8, 8, 8));
    dae::ResourceManager::LoadTexture("floorblock034", "spritesheetMap.png", SDL_Rect(24, 8, 8, 8));
    dae::ResourceManager::LoadTexture("floorblock035", "spritesheetMap.png", SDL_Rect(32, 8, 8, 8));
    dae::ResourceManager::LoadTexture("floorblock036", "spritesheetMap.png", SDL_Rect(48, 8, 8, 8));

	dae::ResourceManager::LoadTexture("floorblock04","spritesheetMap.png",SDL_Rect(56,16,8,8));
	dae::ResourceManager::LoadTexture("floorblock041","spritesheetMap.png",SDL_Rect(0,16,8,8));
	dae::ResourceManager::LoadTexture("floorblock042","spritesheetMap.png",SDL_Rect(8,16,8,8));
	dae::ResourceManager::LoadTexture("floorblock043","spritesheetMap.png",SDL_Rect(16,16,8,8));
	dae::ResourceManager::LoadTexture("floorblock044","spritesheetMap.png",SDL_Rect(24,16,8,8));
	dae::ResourceManager::LoadTexture("floorblock045","spritesheetMap.png",SDL_Rect(32,16,8,8));
	dae::ResourceManager::LoadTexture("floorblock046","spritesheetMap.png",SDL_Rect(48,16,8,8));

    dae::ResourceManager::LoadTexture("topEnd", "spritesheetDigging.png", SDL_Rect(0, 0, 16, 16));
    dae::ResourceManager::LoadTexture("bottomEnd", "spritesheetDigging.png", SDL_Rect(16, 0, 16, 16));
    dae::ResourceManager::LoadTexture("leftEnd", "spritesheetDigging.png", SDL_Rect(32, 0, 16, 16));
    dae::ResourceManager::LoadTexture("rightEnd", "spritesheetDigging.png", SDL_Rect(48, 0, 16, 16));
    dae::ResourceManager::LoadTexture("walkTroughUp", "spritesheetDigging.png", SDL_Rect(64, 0, 16, 16));
    dae::ResourceManager::LoadTexture("walkTroughLeft", "spritesheetDigging.png", SDL_Rect(80, 0, 16, 16));
    dae::ResourceManager::LoadTexture("UpRight", "spritesheetDigging.png", SDL_Rect(96, 0, 16, 16));
    dae::ResourceManager::LoadTexture("UpLeft", "spritesheetDigging.png", SDL_Rect(112, 0, 16, 16));
    dae::ResourceManager::LoadTexture("BottemRight", "spritesheetDigging.png", SDL_Rect(128, 0, 16, 16));
    dae::ResourceManager::LoadTexture("BottomLeft", "spritesheetDigging.png", SDL_Rect(144, 0, 16, 16));
    dae::ResourceManager::LoadTexture("MiddleBlock", "spritesheetDigging.png", SDL_Rect(160, 0, 16, 16));


    dae::ResourceManager::LoadTexture("PumpLine", "SpriteSheetFinal.png", SDL_Rect(384, 144, 32, 16));
    dae::ResourceManager::LoadTexture("PumpEnd", "SpriteSheetFinal.png", SDL_Rect(352, 144, 32, 16));

    dae::ResourceManager::LoadTexture("BreathFireStage1", "SpriteSheetFinal.png", SDL_Rect(224, 96, 32, 32));
    dae::ResourceManager::LoadTexture("BreathFireStage2", "SpriteSheetFinal.png", SDL_Rect(256, 96, 64, 32)); // Adjust X to the right by 32 pixels
    dae::ResourceManager::LoadTexture("BreathFireStage3", "SpriteSheetFinal.png", SDL_Rect(320, 96, 96, 32)); // Adjust X to the right by 64 pixels


    dae::ResourceManager::LoadSprite("Player",
        "spritesheetemty.png",
        26,  // rowCount
        16,  // colCount
        {
            { "Idle", { { { 1, 0 }}, 1 } },
            { "IdleUp", { { { 3, 0 }}, 1 } },
            { "Walk_Right", { {  { 0, 0 }, { 1, 0 } }, 4 } },
            { "Walk_Left", { { { 4, 0 }, { 5, 0 } }, 4 } },
            { "Walk_Up", { { { 2, 0 }, { 3, 0 } }, 4 } },
            { "Walk_Down", { { { 6, 0 }, { 7,0 } }, 4 } },
            { "Dying", { { { 0, 7 }, { 1, 7 }, { 2, 7 }, { 3, 7 } }, 2 } },
            { "Attacking", { { { 0, 5 } }, 1 } },
            { "AttackingUp", { { { 2, 5 } }, 1 } },
            { "Victory", { { { 3, 1 }, { 1, 0 }}, 3 } },
            { "Digging", { { { 0, 1 }, { 1, 1 }}, 3 } }

        });



    dae::ResourceManager::LoadSprite("Rock",
        "Extra.png",
        3,  // rowCount
        16,  // colCount
        {
            { "Idle", { { { 0, 1 }}, 1 } },
            { "Falling", { { { 0, 1 },{ 1, 1 } }, 2 } },
            { "Dying", { {  { 0, 1 } ,{ 1, 1 }, { 2, 1 }, { 3,1}}, 1 } },
            { "Digging", { { { 0, 1 }, { 1, 1 }}, 4 } }

        });

    dae::ResourceManager::LoadSprite("enemy",
        "SpriteSheetFinal.png",
        7,  // rowCount
        14,   // colCount
        {
            { "Normal", { { { 0, 5 }}, 1 } },
            { "Walk_Right", { {  { 0, 5 }, { 1, 5 } }, 1 } },
            { "Walk_Left", { {  { 0, 5 }, { 1, 5 } }, 1 } },
            { "Walk_Up", { { { 0, 5 }, { 1, 5 } }, 1 } },
            { "Walk_Down", { { { 0, 5 }, { 1, 5 } }, 1 } },
            { "Pumped1", { {{ 4, 6 } }, 1 } },
            { "Pumped2", { {{ 5, 6 } }, 1 } },
            { "Pumped3", { {{ 6, 6 } }, 1 } },
            { "Pumped4", { { { 7, 6 } }, 1 } },
            { "Dying", { { { 4, 6 }, { 5, 6 }, { 6, 6 }, { 7, 6 } }, 1 } },
            { "Ghost", { { { 2, 6 }, { 3, 6 }}, 1 } },
            { "Crushed", { { { 8, 6 }}, 1 } }
        });

    dae::ResourceManager::LoadSprite("Fygar",
        "SpriteSheetFinal.png",
        7,  // rowCount
        14,   // colCount
        {
            { "Normal", { { { 0, 4 }}, 1 } },
            { "Walk_Right", { {  { 0, 4 }, { 1, 4 } }, 1 } },
            { "Walk_Left", { {  { 0, 4 }, { 1, 4 } }, 1 } },
            { "Walk_Up", { { { 0, 4 }, { 1, 4 } }, 1 } },
            { "Walk_Down", { { { 0, 4 }, { 1, 4 } }, 1 } },
            { "Pumped1", { {{ 4, 5 } }, 1 } },
            { "Pumped2", { {{ 5, 5 } }, 1 } },
            { "Pumped3", { {{ 6, 5 } }, 1 } },
            { "Pumped4", { { { 7, 5 } }, 1 } },
            { "Dying", { { { 4, 6 }, { 5, 6 }, { 6, 6 }, { 7, 6 } }, 1 } },
            { "Ghost", { { { 0, 6 }, { 1, 6 }}, 1 } },
            { "Crushed", { { { 8, 5 }}, 1 } },
            { "Attacking", { { { 5, 3 },{2,4}, { 5, 3 },{2,4}}, 1 } }

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
    inputManager.BindCommand(SDL_SCANCODE_D, KeyState::Pressed, std::make_unique<MoveCommand>(playerId, 1.5f, 0.0f), InputType::Keyboard); inputManager.BindCommand(SDL_SCANCODE_X, KeyState::Up, std::make_unique<SchootCommand>(playerId), InputType::Keyboard);
    inputManager.BindCommand(SDL_SCANCODE_Z, KeyState::Up, std::make_unique<SchootCommand>(playerId), InputType::Keyboard);
    //inputManager.BindCommand(SDL_SCANCODE_C, KeyState::Up, std::make_unique<DamageCommand>(playerId), InputType::Keyboard);
}

void BindControllerCommands(dae::InputManager& inputManager, int playerId, int controllerIndex) {
    inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::DPadUp), KeyState::Pressed, std::make_unique<MoveCommand>(playerId, 0.0f, -1.5f), InputType::Controller, controllerIndex);
    inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::DPadDown), KeyState::Pressed, std::make_unique<MoveCommand>(playerId, 0.0f, 1.5f), InputType::Controller, controllerIndex);
    inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::DPadLeft), KeyState::Pressed, std::make_unique<MoveCommand>(playerId, -1.5f, 0.0f), InputType::Controller, controllerIndex);
    inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::DPadRight), KeyState::Pressed, std::make_unique<MoveCommand>(playerId, 1.5f, 0.0f), InputType::Controller, controllerIndex);
    inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::A), KeyState::Up, std::make_unique<SchootCommand>(playerId), InputType::Controller, controllerIndex);
   // inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::X), KeyState::Up, std::make_unique<DamageCommand>(playerId), InputType::Controller, controllerIndex);
}

void HandlePlayerInput(dae::InputManager& inputManager, int playerId)
{
    UnBindMenuCommands(inputManager);
    UnBindNameCommands(inputManager);
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

void HandleEnemyPLayerCommands(dae::InputManager& inputManager, int playerId)
{
    const int numControllers = inputManager.GetConnectedControllerCount();
    if (numControllers == 1)
    {
        inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::DPadUp), KeyState::Pressed, std::make_unique<MoveEnemyCommand>(playerId, 0.0f, -1.5f), InputType::Controller, 0);
        inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::DPadDown), KeyState::Pressed, std::make_unique<MoveEnemyCommand>(playerId, 0.0f, 1.5f), InputType::Controller, 0);
        inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::DPadLeft), KeyState::Pressed, std::make_unique<MoveEnemyCommand>(playerId, -1.5f, 0.0f), InputType::Controller, 0);
        inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::DPadRight), KeyState::Pressed, std::make_unique<MoveEnemyCommand>(playerId, 1.5f, 0.0f), InputType::Controller, 0);
        inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::A), KeyState::Up, std::make_unique<AttackEnemyCommand>(playerId), InputType::Controller, 0);
        inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::B), KeyState::Up, std::make_unique<GhostEnemyplayercommand>(playerId), InputType::Controller, 0);

    }
    else if (numControllers >= 2)
    {
        inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::DPadUp), KeyState::Pressed, std::make_unique<MoveCommand>(playerId, 0.0f, -1.5f), InputType::Controller, 1);
        inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::DPadDown), KeyState::Pressed, std::make_unique<MoveCommand>(playerId, 0.0f, 1.5f), InputType::Controller, 1);
        inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::DPadLeft), KeyState::Pressed, std::make_unique<MoveCommand>(playerId, -1.5f, 0.0f), InputType::Controller, 1);
        inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::DPadRight), KeyState::Pressed, std::make_unique<MoveCommand>(playerId, 1.5f, 0.0f), InputType::Controller, 1);
        inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::A), KeyState::Up, std::make_unique<SchootCommand>(playerId), InputType::Controller, 1);

    }
}


void BindExtraControlls(dae::InputManager& inputManager)
{
    // Additional keyboard commands
    inputManager.BindCommand(SDL_SCANCODE_F1, KeyState::Up, std::make_unique<GoToNextSceneCommand>(), InputType::Keyboard);
    inputManager.BindCommand(SDL_SCANCODE_F2, KeyState::Up, std::make_unique<ReloadSceneCommand>(), InputType::Keyboard);
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
    inputManager.BindCommand(GameController::GetButtonMapping(GameController::Button::A), KeyState::Up, std::make_unique<saveScoreCommand>(playerId), inputType, controllerId);
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
    GameData::GetInstance().ResetRound();

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


    if (numControllers > 0) {
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

            GameData::GetInstance().SetGameState(GameData::GameState::SINGLE_PLAYER);
            dae::SceneManager::GetInstance().SetActiveScene("Game");
        },
        []() {
            GameData::GetInstance().SetGameState(GameData::GameState::MULTIPLAYER);
            dae::SceneManager::GetInstance().SetActiveScene("Game");
        },
        []() {
            GameData::GetInstance().SetGameState(GameData::GameState::VERSUS);
            dae::SceneManager::GetInstance().SetActiveScene("Game");
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

    startMenuScene->SetBackgroundMusic(20);
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
    auto moveTweeTitleObject02 = std::make_unique<Tween>(TitleObject02.get(), finalPositioTitleObject02, 6.0f, Easing::Bounce);
    TweenManager::GetInstance().AddTween(std::move(moveTweeTitleObject02));
    TitleObject02->AddComponent(std::move(titleTextComponent02));
    ScoreBoardScene->Add(std::move(TitleObject02));

    // Create GameObject for Title
    auto TitleObject = std::make_unique<dae::GameObject>();
    auto titleTextComponent = std::make_unique<dae::TextComponent>("DIG DUG", dae::ResourceManager::GetFont("TITLE"), SDL_Color{ 247, 190, 57, 255 }, *TitleObject);
    TitleObject->SetLocalPosition(glm::vec3(635, 810, 0.f));
    glm::vec3 finalPositioTitleObject(635.f, 20, 0.f);
    auto moveTweenmenuTitleObject = std::make_unique<Tween>(TitleObject.get(), finalPositioTitleObject, 6.0f, Easing::Bounce);
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

    ScoreBoardScene->SetBackgroundMusic(1);
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
    HighScoretextObject->SetLocalPosition(glm::vec3(1090, 50, 0.f));
    HighScoretextObject->AddComponent(std::move(titleTextComponent02));

    scene->Add(std::move(HighScoretextObject));

    auto HighScoreObject = std::make_unique<dae::GameObject>();
    // Retrieve the top score and convert it to a string
    uint32_t topScore = highScoresInstance.getTopScore();
    std::string topScoreStr = std::to_string(topScore);

    // Create the high score value text component using the string
    auto scoreTextComponent = std::make_unique<dae::TextComponent>(topScoreStr, dae::ResourceManager::GetFont("arcade"), SDL_Color{ 255, 255, 255, 255 }, *HighScoreObject);
    HighScoreObject->SetLocalPosition(glm::vec3(1090, 70, 0.f));
    HighScoreObject->AddComponent(std::move(scoreTextComponent));

    // Add HighScoreObject to the scene (assuming 'this' is a scene class)
    scene->Add(std::move(HighScoreObject));


    glm::vec3 initialPositionPoints(1090, 235, 0.f);

    for (size_t i = 0; i < players.size(); ++i) {
        const auto player = players[i];
        if (auto PointComponent = player->GetComponent<dae::PointComponent>())
        {
            constexpr float distanceBetweenPoints = 70.0f;
            auto pointsDisplayObject = std::make_unique<dae::GameObject>();
            glm::vec3 position = initialPositionPoints + glm::vec3(0.0f, i * distanceBetweenPoints, 0.0f);
            pointsDisplayObject->SetLocalPosition(position);
            auto pointsDisplayComponent = std::make_unique<dae::PointsDisplayComponent>(dae::ResourceManager::GetFont("arcade"),*pointsDisplayObject);
            pointsDisplayComponent->SetPointComponent(PointComponent);
        	pointsDisplayObject->AddComponent(std::move(pointsDisplayComponent));
            scene->Add(std::move(pointsDisplayObject));

            auto ScoretextObject = std::make_unique<dae::GameObject>();
            glm::vec3 Scoreposition = initialPositionPoints + glm::vec3(0.0f, i * distanceBetweenPoints - 20.f, 0.0f);
            ScoretextObject->SetLocalPosition(Scoreposition);
            auto ScoretextObjectTextComponent = std::make_unique<dae::TextComponent>(
                "SCORE",
                dae::ResourceManager::GetFont("arcade"),
                SDL_Color{ 255, 0, 0, 255 },
                *ScoretextObject
            );
            ScoretextObject->AddComponent(std::move(ScoretextObjectTextComponent));
            scene->Add(std::move(ScoretextObject));
        }
    }


    glm::vec3 initialPositionHealth(1040, 535, 0.f);

    for (size_t i = 0; i < players.size(); ++i) {
	    const auto player = players[i];
        if (auto healthComponent = player->GetComponent<dae::HealthComponent>()) {
	        float distanceBetweenLives = 50.0f;
	        auto livesDisplayObject = std::make_unique<dae::GameObject>();
			glm::vec3 position = initialPositionHealth + glm::vec3(0.0f, i * distanceBetweenLives, 0.0f);
            livesDisplayObject->SetLocalPosition(position);
            auto livesDisplayComponent = std::make_unique<dae::LivesDisplayComponent>(*livesDisplayObject,50,50,"lives");
            livesDisplayComponent->SetHealthComponent(healthComponent);
        	livesDisplayObject->AddComponent(std::move(livesDisplayComponent));
            scene->Add(std::move(livesDisplayObject));
		}
    }

    auto roundTextObject = std::make_unique<dae::GameObject>();

    // Create the "ROUND:" label
    auto roundLabelComponent = std::make_unique<dae::TextComponent>(
        "ROUND",
        dae::ResourceManager::GetFont("arcade"),
        SDL_Color{ 255, 255, 0, 255 }, // Yellow color
        *roundTextObject
    );
    roundTextObject->SetLocalPosition(glm::vec3(1090, 635, 0.f)); // Adjust position as needed
    roundTextObject->AddComponent(std::move(roundLabelComponent));

    scene->Add(std::move(roundTextObject));

    // Create the round number text component
    auto roundNumberObject = std::make_unique<dae::GameObject>();

    // Assuming you have a method to get the current round, you will replace 1 with that method
    std::string roundNumberStr = std::to_string(GameData::GetInstance().GetCurrentRound());

    auto roundNumberComponent = std::make_unique<dae::TextComponent>(
        roundNumberStr,
        dae::ResourceManager::GetFont("arcade"),
        SDL_Color{ 255, 255, 255, 255 }, // White color
        *roundNumberObject
    );
    roundNumberObject->SetLocalPosition(glm::vec3(1090, 655, 0.f)); // Adjust position as needed
    roundNumberObject->AddComponent(std::move(roundNumberComponent));

    scene->Add(std::move(roundNumberObject));

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
    scene->SetBackgroundMusic(2);
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
    constexpr glm::vec3 startPos(300, 20, 0.0f);
    constexpr glm::vec2 mapScale(40, 40);

    std::string Tunnelmap = "";
    std::string entitymap = "";
    switch (GameData::GetInstance().GetGameState()) {
    case GameData::GameState::SINGLE_PLAYER:
        Tunnelmap = "map1.1.tunmap";
        entitymap = "map1.1.entmap";
        break;
    case GameData::GameState::MULTIPLAYER:
        Tunnelmap = "map1.2.tunmap";
        entitymap = "map1.2.entmap";
        break;
    case GameData::GameState::VERSUS:
        Tunnelmap = "map1.3.tunmap";
        entitymap = "map1.3.entmap";
        break;
    default:
        Tunnelmap = "map1.tunmap";
        break;
    }

    const LoadMap loadMap("../Data/maps/map1.map", "../Data/maps/" + Tunnelmap, "../Data/maps/" + entitymap);
    SceneHelpers::LoadMapIntoScene(loadMap, scene, startPos, mapScale);
    SceneHelpers::LoadTunnelMapIntoScene(loadMap, scene, startPos, mapScale);
    SceneHelpers::LoadEntitysMapIntoScene(loadMap, scene, startPos, mapScale);

    if (GameData::GetInstance().GetGameState() == GameData::GameState::SINGLE_PLAYER)
    {
        HandlePlayerInput(inputManager, 0);
    }
    else if (GameData::GetInstance().GetGameState() == GameData::GameState::VERSUS)
    {
        HandlePlayerInput(inputManager, 0);
        HandleEnemyPLayerCommands(inputManager,0);
    }
    else
    {
        HandlePlayerInput(inputManager, 0);
        HandlePlayerInput(inputManager, 1);
    }
    LoadUi(scene);
    scene->SetBackgroundMusic(13);
}

void GameScene2(dae::Scene* scene)
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


    constexpr glm::vec3 startPos(300, 20, 0.0f);
    constexpr glm::vec2 mapScale(40, 40);

    std::string Tunnelmap = "";
    std::string entitymap = "";
    switch (GameData::GetInstance().GetGameState()) {
    case GameData::GameState::SINGLE_PLAYER:
        Tunnelmap = "map1.1.tunmap";
        entitymap = "map1.1.entmap";
        break;
    case GameData::GameState::MULTIPLAYER:
        Tunnelmap = "map1.2.tunmap";
        entitymap = "map1.2.entmap";
        break;
    case GameData::GameState::VERSUS:
        Tunnelmap = "map1.3.tunmap";
        entitymap = "map1.1.entmap";
        break;
    default:
        Tunnelmap = "map1.tunmap";
        break;
    }

    const LoadMap loadMap("../Data/maps/map1.map", "../Data/maps/" + Tunnelmap, "../Data/maps/" + entitymap);
    SceneHelpers::LoadMapIntoScene(loadMap, scene, startPos, mapScale);
    SceneHelpers::LoadTunnelMapIntoScene(loadMap, scene, startPos, mapScale);
    SceneHelpers::LoadEntitysMapIntoScene(loadMap, scene, startPos, mapScale);


    if (GameData::GetInstance().GetGameState() == GameData::GameState::SINGLE_PLAYER)
    {
        HandlePlayerInput(inputManager, 0);
    }
    else
    {
        HandlePlayerInput(inputManager, 0);
        HandlePlayerInput(inputManager, 1);
    }
    LoadUi(scene);
    scene->SetBackgroundMusic(13);
}

void GameScene3(dae::Scene* scene)
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


    constexpr glm::vec3 startPos(300, 20, 0.0f);
    constexpr glm::vec2 mapScale(40, 40);

    std::string Tunnelmap = "";
    std::string entitymap = "";
    switch (GameData::GetInstance().GetGameState()) {
    case GameData::GameState::SINGLE_PLAYER:
        Tunnelmap = "map1.1.tunmap";
        entitymap = "map1.1.entmap";
        break;
    case GameData::GameState::MULTIPLAYER:
        Tunnelmap = "map1.2.tunmap";
        entitymap = "map1.2.entmap";
        break;
    case GameData::GameState::VERSUS:
        Tunnelmap = "map1.3.tunmap";
        entitymap = "map1.1.entmap";
        break;
    default:
        Tunnelmap = "map1.tunmap";
        break;
    }

    const LoadMap loadMap("../Data/maps/map1.map", "../Data/maps/" + Tunnelmap, "../Data/maps/" + entitymap);
    SceneHelpers::LoadMapIntoScene(loadMap, scene, startPos, mapScale);
    SceneHelpers::LoadTunnelMapIntoScene(loadMap, scene, startPos, mapScale);
    SceneHelpers::LoadEntitysMapIntoScene(loadMap, scene, startPos, mapScale);


    if (GameData::GetInstance().GetGameState() == GameData::GameState::SINGLE_PLAYER)
    {
        HandlePlayerInput(inputManager, 0);
    }
    else
    {
        HandlePlayerInput(inputManager, 0);
        HandlePlayerInput(inputManager, 1);
    }
    LoadUi(scene);
    scene->SetBackgroundMusic(13);
}

void load()
{
    loadResources();
    auto& sceneManager = dae::SceneManager::GetInstance();
    auto& inputmanager = dae::InputManager::GetInstance();
    auto& highscore = HighScores::GetInstance();

    BindExtraControlls(inputmanager);

    highscore.loadScores();

    const auto& startMenuScene = sceneManager.CreateScene("StartMenu");
    const auto& ScoreBoardScene = sceneManager.CreateScene("ScoreboardScene");
    const auto& Game = sceneManager.CreateScene("Game");
    const auto& Game2 = sceneManager.CreateScene("Game2");
    const auto& Game3 = sceneManager.CreateScene("Game3");
    const auto& SaveSoreScene = sceneManager.CreateScene("SaveScoreScene");


    startMenuScene->SetOnActivateCallback([startMenuScene]() { LoadStartMenu(startMenuScene); });
    ScoreBoardScene->SetOnActivateCallback([ScoreBoardScene]() {LoadScoreboard(ScoreBoardScene); });
    Game->SetOnActivateCallback([Game]() { GameScene(Game); });
    Game2->SetOnActivateCallback([Game2]() { GameScene2(Game2); });
    Game3->SetOnActivateCallback([Game3]() { GameScene3(Game3); });
    SaveSoreScene->SetOnActivateCallback([SaveSoreScene]() { loadInputScore(SaveSoreScene); });


    sceneManager.SetActiveScene("StartMenu");
}

int main(int, char* []) {
    dae::minigin engine("../Data/");
    engine.Run(load);
    return 0;
}