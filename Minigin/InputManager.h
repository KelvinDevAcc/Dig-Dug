#pragma once
#include <unordered_map>
#include <array>
#include <SDL.h>
#include "Command.h"
#include "Singleton.h"
#include "GameController.h"

enum class KeyState
{
	Down,
	Up,
	Pressed
};
enum class InputType
{
	Controller,
	Keyboard
};

namespace dae {
    class InputManager final : public Singleton<InputManager> {
    public:
        static bool ProcessInput();
        static bool IsDownThisFrame(SDL_Scancode button);
        static bool IsUpThisFrame(SDL_Scancode button);
        static bool IsPressed(SDL_Scancode button);
        static int  GetConnectedControllerCount();

        static void BindCommand(unsigned int button, KeyState state, std::unique_ptr<Command> command, InputType type, int controllerIndex = 0);
        static void UnbindCommand(unsigned int button, KeyState state, InputType type);

    private:
        static void HandleKeyInput();
        static void HandleKeyInput(SDL_Scancode keyCode, KeyState state);
        static void HandleControllerInput();

        static std::array<GameController, 4> m_gameControllers;
        static std::array<std::unordered_map<std::pair<unsigned int, KeyState>, std::unique_ptr<Command>>, 4> m_controllerBindings;
        static std::unordered_map<std::pair<unsigned int, KeyState>, std::unique_ptr<Command>> m_keyboardBindings;

        static const Uint8* m_previousState;
        static const Uint8* m_currentState;
    };
}

template<>
struct std::hash<std::pair<unsigned int, KeyState>> {
    size_t operator()(const std::pair<unsigned int, KeyState>& p) const noexcept {
        return std::hash<unsigned int>()(p.first) ^ std::hash<int>()(static_cast<int>(p.second));
    }
};
