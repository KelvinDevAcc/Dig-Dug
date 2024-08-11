#pragma once
#include "GameObject.h"
#include "HealthComponent.h"
#include "MenuComponent.h"
#include "SelectNameComponent.h"
#include "sound_system.h"


class Command 
{
public:
	virtual ~Command() = default;
	virtual void Execute() = 0;
};


enum class MovementDirection
{
    None,
    Up,
    Down,
    Left,
    Right
};

class MoveCommand : public Command
{
public:
    MoveCommand(int playerNumber ,float deltaX, float deltaY);
    void Execute() override;

private:
    dae::GameObject* m_gameObject{};
    float m_deltaX;
    float m_deltaY;
    int m_playerNum{};
};



class DieCommand final : public Command {
public:
    DieCommand(int playerNumber);

    virtual void Execute() override;

private:
    dae::GameObject* m_gameObject{};
};



class SchootCommand final : public Command {
public:
    SchootCommand(int playerNumber);

    virtual void Execute() override;

private:
    dae::GameObject* m_gameObject{};
};

class MoveEnemyCommand : public Command
{
public:
    MoveEnemyCommand(int playerNumber, float deltaX, float deltaY);
    void Execute() override;

private:
    dae::GameObject* m_gameObject{};
    float m_deltaX;
    float m_deltaY;
    int m_playerNum{};
};

class AttackEnemyCommand final : public Command {
public:
    AttackEnemyCommand(int playerNumber);

    virtual void Execute() override;

private:
    dae::GameObject* m_gameObject{};
};

class GhostEnemyplayercommand final : public Command {
public:
    GhostEnemyplayercommand(int playerNumber);

    virtual void Execute() override;

private:
    dae::GameObject* m_gameObject{};
};


class GoToNextSceneCommand final : public Command {
public:
    GoToNextSceneCommand();

    virtual void Execute() override;
};

class ReloadSceneCommand final : public Command {
public:
    ReloadSceneCommand();

    virtual void Execute() override;
};



//sound Commands
class PlaySoundCommand : public Command {
public:
    PlaySoundCommand();
    void Execute() override;

};


class MuteCommand : public Command {
public:
    explicit MuteCommand(sound_system* soundSystem);
    void Execute() override;

private:
    sound_system* m_soundSystem;
};

class IncreaseVolumeCommand : public Command {
public:
    explicit IncreaseVolumeCommand(sound_system* soundSystem);
    void Execute() override;

private:
    sound_system* m_soundSystem;
};

class DecreaseVolumeCommand : public Command {
public:
    explicit DecreaseVolumeCommand(sound_system* soundSystem);
    void Execute() override;

private:
    sound_system* m_soundSystem;
};


//Menu Commands 
class NavigateUpCommand : public Command
{
public:
    void Execute() override;

private:
    static dae::MenuComponent* FindMenuComponent();
};

class NavigateDownCommand : public Command
{
public:
    void Execute() override;

private:
    static dae::MenuComponent* FindMenuComponent();
};

class SelectOptionCommand : public Command
{
public:
    void Execute() override;

private:
    static dae::MenuComponent* FindMenuComponent();
};


//select name commands
class NavigateUpLetterCommand : public Command
{
public:
    NavigateUpLetterCommand(int playerId) : m_PlayerId(playerId) {}
    void Execute() override;

private:
    int m_PlayerId;
    SelectNameComponent* FindSelectNameComponent() const;
};

// NavigateDownLetterCommand.h
class NavigateDownLetterCommand : public Command
{
public:
    NavigateDownLetterCommand(int playerId) : m_PlayerId(playerId) {}
    void Execute() override;

private:
    int m_PlayerId;
    SelectNameComponent* FindSelectNameComponent() const;
};

// SelectOptionLetterCommand.h
class SelectOptionLetterCommand : public Command
{
public:
    SelectOptionLetterCommand(int playerId) : m_PlayerId(playerId) {}
    void Execute() override;

private:
    int m_PlayerId;
    SelectNameComponent* FindSelectNameComponent() const;
};

// saveScoreCommand.h
class saveScoreCommand : public Command
{
public:
    saveScoreCommand(int playerId) : m_PlayerId(playerId) {}
    void Execute() override;

private:
    int m_PlayerId;
    std::string m_CurrentName;
    static SelectNameComponent* FindSelectNameComponent(int playerId);
};