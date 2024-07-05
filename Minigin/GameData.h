#pragma once
#include <map>
#include "Singleton.h"

struct PlayerData {
    int score = 0;
    int lives = 3;
    int pepperCount = 5;
};

class GameData final: public dae::Singleton<GameData> {
public:
    GameData();

    ~GameData() override = default;

    GameData(const GameData&) = delete;
    GameData& operator=(const GameData&) = delete;
    GameData(GameData&&) noexcept = delete;
    GameData& operator=(GameData&&) noexcept = delete;

    void FindAndStorePlayerData();

    PlayerData GetPlayerData(int playerID) const;

    void UpdatePlayerData(int playerID, const PlayerData& newData);

    int GetNumberOfPlayers() const;

    void ResetPlayerData(int playerID);

    enum class GameState { SINGLE_PLAYER, MULTIPLAYER, VERSUS };
    GameState GetGameState() const { return m_gameState; }
    void SetGameState(GameState gameState) { m_gameState = gameState; }

private:
    std::map<int, PlayerData> m_playerData;

    GameState m_gameState;
};

