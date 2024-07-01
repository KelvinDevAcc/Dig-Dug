#pragma once

#include <map>
#include <memory>


class GameControllerImpl; 

class GameController
{
public:

    enum class Button
    {
        A,
        B,
        X,
        Y,
        LeftBumper,
        RightBumper,
        Back,
        Start,
        LeftThumb,
        RightThumb,
        DPadUp,
        DPadDown,
        DPadLeft,
        DPadRight
    };

    GameController(int controllerIndex = 0);
    ~GameController();

    bool IsConnected() const;
    void UpdateState() const;

    bool IsButtonDown(unsigned int button) const;
    bool IsButtonUp(unsigned int button) const;
    bool IsButtonPressed(unsigned int button) const;

    static unsigned int GetButtonMapping(Button button);

private:
    std::unique_ptr<GameControllerImpl> m_pImpl; 

    static const std::map<Button, unsigned int> ButtonMappings;
};




