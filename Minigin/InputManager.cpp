#include "InputManager.h"

#include <SDL.h>

#include "backends/imgui_impl_sdl2.h"

using namespace dae;

std::array<GameController, 4> InputManager::m_gameControllers = { GameController(0), GameController(1), GameController(2), GameController(3) };
std::array<std::unordered_map<std::pair<unsigned int, KeyState>, std::unique_ptr<Command>>, 4> InputManager::m_controllerBindings;
std::unordered_map<std::pair<unsigned int, KeyState>, std::unique_ptr<Command>> InputManager::m_keyboardBindings;

const Uint8* InputManager::m_previousState = SDL_GetKeyboardState(nullptr);
const Uint8* InputManager::m_currentState = SDL_GetKeyboardState(nullptr);

bool InputManager::ProcessInput()
{
    for (auto& controller : m_gameControllers)
    {
        controller.UpdateState();
    }

    m_previousState = m_currentState;
    m_currentState = SDL_GetKeyboardState(nullptr);

    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
        case SDL_QUIT:
            return false;
        case SDL_KEYDOWN:
            HandleKeyInput(e.key.keysym.scancode, KeyState::Down);
            break;
        case SDL_KEYUP:
            HandleKeyInput(e.key.keysym.scancode, KeyState::Up);
            break;
        default:
            break;
        }
        ImGui_ImplSDL2_ProcessEvent(&e);
    }

    HandleKeyInput();
    HandleControllerInput();

    return true;
}



bool InputManager::IsDownThisFrame(SDL_Scancode button)
{
    return m_currentState[button] && !m_previousState[button];
}

bool InputManager::IsUpThisFrame(SDL_Scancode button)
{
    return !m_currentState[button] && m_previousState[button];
}

bool InputManager::IsPressed(SDL_Scancode button) 
{
    return m_currentState[button];
}

void InputManager::BindCommand(unsigned int button, KeyState state, std::unique_ptr<Command> command, InputType type, int controllerIndex)
{
    if (type == InputType::Controller)
    {
        if (controllerIndex < static_cast<int>(m_controllerBindings.size()))
        {
            m_controllerBindings[controllerIndex][{button, state}] = std::move(command);
        }
    }
    else
    {
        m_keyboardBindings[{button, state}] = std::move(command);
    }
}

void InputManager::UnbindCommand(unsigned int button, KeyState state, InputType type)
{
    if (type == InputType::Controller)
    {
        for (auto& bindings : m_controllerBindings)
        {
            if (bindings.empty()) {
                continue; 
            }

            for (auto it = bindings.begin(); it != bindings.end();)
            {
                const auto& binding = *it;

                if (binding.first.first == button && binding.first.second == state)
                {
                    it = bindings.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }
    }
    else
    {
        m_keyboardBindings.erase({ button, state });
    }
}


void InputManager::HandleKeyInput()
{
    for (const auto& binding : m_keyboardBindings)
    {
        const auto keyCode = static_cast<SDL_Scancode>(binding.first.first);

        if (IsPressed(keyCode) && binding.first.second == KeyState::Pressed)
        {
            binding.second->Execute();
        }
    }
}



void InputManager::HandleKeyInput(SDL_Scancode keyCode, KeyState state)
{
    for (const auto& binding : m_keyboardBindings)
    {
        if (static_cast<SDL_Scancode>(binding.first.first) == keyCode && binding.first.second == state)
        {
            binding.second->Execute();
            break;
        }
    }
}

int InputManager::GetConnectedControllerCount()
{
    int connectedCount = 0;
    for (const auto& controller : m_gameControllers)
    {
        if (controller.IsConnected())
        {
            connectedCount++;
        }
    }
    return connectedCount;
}

void InputManager::HandleControllerInput() {
    for (int i = 0; i <static_cast<int>(m_gameControllers.size()); ++i) {
        if (!m_gameControllers[i].IsConnected()) {
            continue; 
        }

        for (const auto& binding : m_controllerBindings[i]) {
            const auto button = binding.first.first;

            switch (const auto state = binding.first.second) {
            case KeyState::Down: 
                if (m_gameControllers[i].IsButtonDown(button)) {
                    binding.second->Execute();
                }
                break;
            case KeyState::Up:
                if (m_gameControllers[i].IsButtonUp(button)) {
                    binding.second->Execute();
                }
                break;
            case KeyState::Pressed: 
                if (m_gameControllers[i].IsButtonPressed(button)) {
                    binding.second->Execute();
                }
                break;
            }
        }
    }
}


