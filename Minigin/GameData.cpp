#include "GameData.h"

#include <vector>

#include "SceneData.h"
#include "SceneManager.h"


GameData::GameData() : m_gameState(GameState::SINGLE_PLAYER) {}

void GameData::AddPlayer(int playerID, const PlayerData& playerData)
{
    m_playerData[playerID] = playerData;
}

void GameData::UpdatePlayerData(int playerID, const PlayerData& newData)
{
    if (m_playerData.contains(playerID)) {
        m_playerData[playerID] = newData;
    }
}

int GameData::GetNumberOfPlayers() const
{
    return static_cast<int>(m_playerData.size());
}

PlayerData GameData::GetPlayerData(int playerID) const
{
    if (m_playerData.contains(playerID)) {
        return m_playerData.at(playerID);
    }
    else {
        return {};
    }
}

void GameData::ResetPlayerData(int playerID)
{
    if (m_playerData.contains(playerID)) {
        m_playerData[playerID] = PlayerData();
    }
}

void GameData::CheckGameState()
{
    int playersAlive = 0;
    int lastPlayerID = -1;

    for (const auto& [playerID, data] : m_playerData)
    {
        if (data.lives >= 0)
        {
            playersAlive++;
            lastPlayerID = playerID;
        }
    }

    if (playersAlive == 0)
    {
        // Broadcast message and transition to SaveScoreScene
        BroadcastEndGame();
        dae::SceneManager::GetInstance().SetActiveScene("SaveScoreScene");
        return;
    }

    // Transition to the next level or scene based on round and game state
    if (dae::SceneData::GetInstance().GetEnemies().empty())
    {
        BroadcastLevelCompleted();
        IncrementRound();
        LoadNextScene();
    }
}

void GameData::LoadNextScene()
{
    const int currentRound = GetCurrentRound();

    if (currentRound % 3 == 1)
    {
        dae::SceneManager::GetInstance().SetActiveScene("Game");
    }
    else if (currentRound % 3 == 2)
    {
        dae::SceneManager::GetInstance().SetActiveScene("Game2");
    }
    else if (currentRound % 3 == 0)
    {
        dae::SceneManager::GetInstance().SetActiveScene("Game3");
    }
}

void GameData::BroadcastEndGame()
{
    dae::Message message;
    message.type = dae::PlaySoundMessageType::Sound;
    message.arguments.emplace_back(static_cast<sound_id>(5));
    dae::EventQueue::Broadcast(message);
}

void GameData::BroadcastLevelCompleted()
{
    dae::Message message;
    message.type = dae::PlaySoundMessageType::Sound;
    message.arguments.emplace_back(static_cast<sound_id>(12));
    dae::EventQueue::Broadcast(message);
}
