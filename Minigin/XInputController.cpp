#include "XInputController.h"

XInputController::XInputController(DWORD controllerIndex)
	: m_controllerIndex(controllerIndex), m_PreviousState(), m_CurrentState(), m_ButtonsPressedThisFrame(0),
	  m_ButtonsReleasedThisFrame(0),
	  m_connected(false)
{
}

bool XInputController::IsConnected() const
{
    return m_connected;
}

void XInputController::UpdateState()
{
    CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
    ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));

    if (XInputGetState(m_controllerIndex, &m_CurrentState) == ERROR_SUCCESS)
    {
        m_connected = true;
    }
    else
    {
        m_connected = false;
    }

    const auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
    m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
    m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
}

bool XInputController::IsButtonDown(unsigned int button) const
{
    return (m_CurrentState.Gamepad.wButtons & button) && !(m_PreviousState.Gamepad.wButtons & button);
}

bool XInputController::IsButtonUp(unsigned int button) const
{
    return !(m_CurrentState.Gamepad.wButtons & button) && (m_PreviousState.Gamepad.wButtons & button);
}

bool XInputController::IsButtonPressed(unsigned int button) const
{
    return m_CurrentState.Gamepad.wButtons & button;
}