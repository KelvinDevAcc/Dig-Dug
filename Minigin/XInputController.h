#pragma once

#include <windows.h>
#include <Xinput.h>


class XInputController
{
public:
    XInputController(DWORD controllerIndex = 0);
    ~XInputController() = default;

    bool IsConnected() const;
    void UpdateState();

    bool IsButtonDown(unsigned int button) const;
    bool IsButtonUp(unsigned int button) const;
    bool IsButtonPressed(unsigned int button) const;

private:
    DWORD m_controllerIndex;
    XINPUT_STATE m_PreviousState;
    XINPUT_STATE m_CurrentState;
    unsigned int m_ButtonsPressedThisFrame;
    unsigned int m_ButtonsReleasedThisFrame;
    bool m_connected;
};