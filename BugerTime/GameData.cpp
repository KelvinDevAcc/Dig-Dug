#include "GameData.h"

#include "Player.h"
#include "SceneData.h"

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
}

void GameData::ResetPlayerData(int playerID)
{
    if (m_playerData.contains(playerID)) {
        m_playerData[playerID] = PlayerData(); 
    }
}