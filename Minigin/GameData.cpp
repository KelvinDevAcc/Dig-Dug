#include "GameData.h"

#include <vector>

#include "HealthComponent.h"
#include "PointComponent.h"
#include "SceneData.h"
#include "SceneManager.h" 

namespace dae
{
    class HealthComponent;
    class PointComponent;
    class GameObject;
}

GameData::GameData() : m_gameState(GameState::SINGLE_PLAYER) {}

void GameData::FindAndStorePlayerData()
{
    const std::vector<dae::GameObject*> players = dae::SceneData::GetInstance().GetPlayers();

    m_playerData.clear();

    for (int i = 0; i < static_cast<int>(players.size()); i++) {
        if (!players[i]) {
            continue;
        }

        const auto pointComponent = players[i]->GetComponent<dae::PointComponent>();
        const auto healthComponent = players[i]->GetComponent<dae::HealthComponent>();

        if (!pointComponent || !healthComponent) {
            continue;
        }

        PlayerData playerData;
        playerData.score = pointComponent->GetScore();
        playerData.lives = healthComponent->GetLives();

        m_playerData[i] = playerData;
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

void GameData::UpdatePlayerData(int playerID, const PlayerData& newData)
{
    if (m_playerData.contains(playerID)) {
        m_playerData[playerID] = newData;
    }

    // After updating the player data, check the game state
    CheckGameState();
}

void GameData::ResetPlayerData(int playerID)
{
    if (m_playerData.contains(playerID)) {
        m_playerData[playerID] = PlayerData();
    }
}

// New Method to Check Game State and Handle Transitions
void GameData::CheckGameState()
{
    const int numberOfPlayers = GetNumberOfPlayers();
    int playersAlive = 0;
    int lastPlayerID = -1;

    for (const auto& [playerID, data] : m_playerData)
    {
        if (data.lives > 0)
        {
            playersAlive++;
            lastPlayerID = playerID;
        }
    }

    if (playersAlive == 0)
    {
        dae::Message message;
        message.type = dae::PlaySoundMessageType::Sound;
        message.arguments.emplace_back(static_cast<sound_id>(5));
        dae::EventQueue::Broadcast(message);
        // All players are dead, transition to the save name and score scene
        dae::SceneManager::GetInstance().SetActiveScene("SaveScoreScene");
        return;
    }
    else if (numberOfPlayers == 1 && playersAlive == 1 && m_playerData[lastPlayerID].lives <= 0)
    {
        dae::Message message;
        message.type = dae::PlaySoundMessageType::Sound;
        message.arguments.emplace_back(static_cast<sound_id>(5));
        dae::EventQueue::Broadcast(message);
        // Single player and the last player's lives are less than or equal to 0
        dae::SceneManager::GetInstance().SetActiveScene("SaveScoreScene");
        return;
    }
    else if (numberOfPlayers > 1 && playersAlive == 1 && m_playerData[lastPlayerID].lives <= 0)
    {
        dae::Message message;
        message.type = dae::PlaySoundMessageType::Sound;
        message.arguments.emplace_back(static_cast<sound_id>(5));
        dae::EventQueue::Broadcast(message);
        // Two players, and the last remaining player's lives are less than or equal to 0
        dae::SceneManager::GetInstance().SetActiveScene("SaveScoreScene");
        return;
    }

    // Check if all enemies are defeated
    const std::vector<dae::GameObject*> enemies = dae::SceneData::GetInstance().GetEnemies();

    if (enemies.empty())
    {

        dae::Message message;
        message.type = dae::PlaySoundMessageType::Sound;
        message.arguments.emplace_back(static_cast<sound_id>(12));
        dae::EventQueue::Broadcast(message);

        IncrementRound();
        const int currentRound = GetCurrentRound(); // Assumed that there is a function to get the current round

        // Determine which scene to load based on the current round
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
}
