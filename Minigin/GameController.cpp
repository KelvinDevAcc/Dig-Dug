#include "GameController.h"
#include "XInputController.h" 

class GameControllerImpl
{
public:
    GameControllerImpl(int controllerIndex = 0) : m_xinputController(controllerIndex) {}

    bool IsConnected() const { return m_xinputController.IsConnected(); }
    void UpdateState() { m_xinputController.UpdateState(); }
    bool IsButtonDown(unsigned int button) const { return m_xinputController.IsButtonDown(button); }
    bool IsButtonUp(unsigned int button) const { return m_xinputController.IsButtonUp(button); }
    bool IsButtonPressed(unsigned int button) const { return m_xinputController.IsButtonPressed(button); }

private:
    XInputController m_xinputController;
};


const std::map<GameController::Button, unsigned int> GameController::ButtonMappings = {
     {Button::A, XINPUT_GAMEPAD_A},
    {Button::B, XINPUT_GAMEPAD_B},
    {Button::X, XINPUT_GAMEPAD_X},
    {Button::Y, XINPUT_GAMEPAD_Y},
    {Button::LeftBumper, XINPUT_GAMEPAD_LEFT_SHOULDER},
    {Button::RightBumper, XINPUT_GAMEPAD_RIGHT_SHOULDER},
    {Button::Back, XINPUT_GAMEPAD_BACK},
    {Button::Start, XINPUT_GAMEPAD_START},
    {Button::LeftThumb, XINPUT_GAMEPAD_LEFT_THUMB},
    {Button::RightThumb, XINPUT_GAMEPAD_RIGHT_THUMB},
    {Button::DPadUp, XINPUT_GAMEPAD_DPAD_UP},
    {Button::DPadDown, XINPUT_GAMEPAD_DPAD_DOWN},
    {Button::DPadLeft, XINPUT_GAMEPAD_DPAD_LEFT},
    {Button::DPadRight, XINPUT_GAMEPAD_DPAD_RIGHT}
};

unsigned int GameController::GetButtonMapping(Button button)
{
    return ButtonMappings.at(button);
}

GameController::GameController(int controllerIndex) : m_pImpl(std::make_unique<GameControllerImpl>(controllerIndex)) {}
 
GameController::~GameController() = default;

bool GameController::IsConnected() const { return m_pImpl->IsConnected(); }

void GameController::UpdateState() const { m_pImpl->UpdateState(); }

bool GameController::IsButtonDown(unsigned int button) const { return m_pImpl->IsButtonDown(button); }

bool GameController::IsButtonUp(unsigned int button) const { return m_pImpl->IsButtonUp(button); }

bool GameController::IsButtonPressed(unsigned int button) const { return m_pImpl->IsButtonPressed(button); }
