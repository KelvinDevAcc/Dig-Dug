#pragma once
#include <map>
#include "Singleton.h"

struct PlayerData {
    int score = 0;
    int lives = 3;
};

class GameData final: public dae::Singleton<GameData> {
public:
    GameData();

    ~GameData() override = default;

    GameData(const GameData&) = delete;
    GameData& operator=(const GameData&) = delete;
    GameData(GameData&&) noexcept = delete;
    GameData& operator=(GameData&&) noexcept = delete;

    void AddPlayer(int playerID, const PlayerData& playerData);

    PlayerData GetPlayerData(int playerID) const;

    void UpdatePlayerData(int playerID, const PlayerData& newData);

    int GetNumberOfPlayers() const;

    void ResetPlayerData(int playerID);
    void ResetAllPlayerData();
    void CheckGameState();

    void LoadNextScene();
    void BroadcastEndGame();
    void BroadcastLevelCompleted();

    // Round management
    int GetCurrentRound() const { return m_currentRound; }
    void IncrementRound() { ++m_currentRound; }
    void ResetRound() { m_currentRound = 1; }

    enum class GameState { SINGLE_PLAYER, MULTIPLAYER, VERSUS };
    GameState GetGameState() const { return m_gameState; }
    void SetGameState(GameState gameState) { m_gameState = gameState; }

private:
    std::map<int, PlayerData> m_playerData;

    GameState m_gameState;
    int m_currentRound = 1;
};

