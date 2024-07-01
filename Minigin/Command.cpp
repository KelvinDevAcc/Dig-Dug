#include "Command.h"
#include "EnventQueue.h"
#include "HighScores.h"
#include "SceneData.h"
#include "SceneManager.h"
#include "servicelocator.h"
#include "../BugerTime/GameData.h"
#include "../BugerTime/Player.h"


MoveCommand::MoveCommand(int playerNumber, float deltaX, float deltaY)
	: m_deltaX(deltaX), m_deltaY(deltaY)
{
	const std::vector<dae::GameObject*> players = dae::SceneData::GetInstance().GetPlayers();
    const std::vector<dae::GameObject*> playerEnemys = dae::SceneData::GetInstance().GetenemyPlayers();

    if (playerNumber >= 0 && playerNumber < static_cast<int>(players.size()) + static_cast<int>(playerEnemys.size()))
    {
	    if (playerNumber == 1)
	    {
		    if (!dae::SceneData::GetInstance().GetenemyPlayers().empty())
		    {
                m_gameObject = playerEnemys[0];
		    }
            else
            {
                m_playerNum = playerNumber;
                m_gameObject = players[playerNumber];
            }
	    }
    	else
	    {
            m_playerNum = playerNumber;
            m_gameObject = players[playerNumber];
	    }
        
    }
}

void MoveCommand::Execute()
{
	if (m_gameObject) {
        m_gameObject->GetComponent<game::Player>()->Move(m_deltaX, m_deltaY);
    }
}



DamageCommand::DamageCommand(int playerNumber)
{
    const std::vector<dae::GameObject*> players = dae::SceneData::GetInstance().GetPlayers();

    if (playerNumber >= 0 && playerNumber < static_cast<int>(players.size()))
    {
        m_gameObject = players[playerNumber];
    }
}

void DamageCommand::Execute()
{
    m_gameObject->GetComponent<game::Player>()->Die();
}


ScorePointCommand::ScorePointCommand(int playerNumber)
{
    const std::vector<dae::GameObject*> players = dae::SceneData::GetInstance().GetPlayers();

    if (playerNumber >= 0 && playerNumber < static_cast<int>(players.size()))
    {
        m_gameObject = players[playerNumber];
    }
}

void ScorePointCommand::Execute()
{
    m_gameObject->GetComponent<game::Player>()->Attack();
}



GoToNextSceneCommand::GoToNextSceneCommand() = default;

void GoToNextSceneCommand::Execute()
{
    auto& sceneManager = dae::SceneManager::GetInstance();
    sceneManager.GoToNextScene();
   
}

PlaySoundCommand::PlaySoundCommand() = default;

void PlaySoundCommand::Execute() {

    dae::Message message;

    message.type = dae::PlaySoundMessageType::Sound;

    message.arguments.emplace_back(static_cast<sound_id>(4)); 

    dae::EventQueue::Broadcast(message);
}

MuteCommand::MuteCommand(sound_system* soundSystem)
    : m_soundSystem(soundSystem) {}

void MuteCommand::Execute() {
    if (m_soundSystem) {
        m_soundSystem->mute();
    }
}

IncreaseVolumeCommand::IncreaseVolumeCommand(sound_system* soundSystem)
    : m_soundSystem(soundSystem) {}

void IncreaseVolumeCommand::Execute() {
    if (m_soundSystem) {
        const float currentVolume = m_soundSystem->getVolume();
        m_soundSystem->setVolume(std::min(currentVolume + 10.0f, 128.0f));
    }
}

DecreaseVolumeCommand::DecreaseVolumeCommand(sound_system* soundSystem)
    : m_soundSystem(soundSystem) {}

void DecreaseVolumeCommand::Execute() {
    if (m_soundSystem) {
        const float currentVolume = m_soundSystem->getVolume();
        m_soundSystem->setVolume(std::max(currentVolume - 10.0f, 0.0f));
    }
}



void NavigateUpCommand::Execute() {
	if (const auto menu = FindMenuComponent()) {
        dae::Message message;

        message.type = dae::PlaySoundMessageType::Sound;

        message.arguments.emplace_back(static_cast<sound_id>(20)); 

        dae::EventQueue::Broadcast(message);
        menu->NavigateUp();
    }
}

dae::MenuComponent* NavigateUpCommand::FindMenuComponent() {
	const auto& sceneManager = dae::SceneManager::GetInstance();
    const auto& currentScene = sceneManager.GetActiveScene();
    const auto& objects = currentScene->GetObjects();
    for (const auto& obj : objects) {
	    if (const auto menuComponent = obj->GetComponent<dae::MenuComponent>()) {
            return menuComponent;
        }
    }
    return nullptr;
}

void NavigateDownCommand::Execute() {
	if (const auto menu = FindMenuComponent()) {
        dae::Message message;

        message.type = dae::PlaySoundMessageType::Sound;

        message.arguments.emplace_back(static_cast<sound_id>(20)); 

        dae::EventQueue::Broadcast(message);
        menu->NavigateDown();
    }
}

dae::MenuComponent* NavigateDownCommand::FindMenuComponent() {
	const auto& sceneManager = dae::SceneManager::GetInstance();
    const auto& currentScene = sceneManager.GetActiveScene();
    const auto& objects = currentScene->GetObjects();
    for (const auto& obj : objects) {
	    if (const auto menuComponent = obj->GetComponent<dae::MenuComponent>()) {
            return menuComponent;
        }
    }
    return nullptr;
}

void SelectOptionCommand::Execute() {
	if (const auto menu = FindMenuComponent()) {
        dae::Message message;

        message.type = dae::PlaySoundMessageType::Sound;

        message.arguments.emplace_back(static_cast<sound_id>(18)); 

        dae::EventQueue::Broadcast(message);
        menu->SelectOption();
    }
}

dae::MenuComponent* SelectOptionCommand::FindMenuComponent() {
	const auto& sceneManager = dae::SceneManager::GetInstance();
    const auto& currentScene = sceneManager.GetActiveScene();
    const auto& objects = currentScene->GetObjects();
    for (const auto& obj : objects) {
	    if (const auto menuComponent = obj->GetComponent<dae::MenuComponent>()) {
            return menuComponent;
        }
    }
    return nullptr;
}

void NavigateUpLetterCommand::Execute() {
    if (const auto selectName = FindSelectNameComponent()) {
        dae::Message message;
        message.type = dae::PlaySoundMessageType::Sound;
        message.arguments.emplace_back(static_cast<sound_id>(20)); 
        dae::EventQueue::Broadcast(message);
        selectName->AddLetter();
    }
}

SelectNameComponent* NavigateUpLetterCommand::FindSelectNameComponent() const
{
    const auto& sceneManager = dae::SceneManager::GetInstance();
    const auto& currentScene = sceneManager.GetActiveScene();
    const auto& objects = currentScene->GetObjects();
    for (const auto& obj : objects) {
        if (const auto selectNameComponent = obj->GetComponent<SelectNameComponent>()) {
            if (selectNameComponent->GetPlayerId() == m_PlayerId) {
                return selectNameComponent;
            }
        }
    }
    return nullptr;
}

void NavigateDownLetterCommand::Execute() {
    if (const auto selectName = FindSelectNameComponent()) {
        dae::Message message;
        message.type = dae::PlaySoundMessageType::Sound;
        message.arguments.emplace_back(static_cast<sound_id>(20)); 
        dae::EventQueue::Broadcast(message);
        selectName->SubtractLetter();
    }
}

SelectNameComponent* NavigateDownLetterCommand::FindSelectNameComponent() const
{
    const auto& sceneManager = dae::SceneManager::GetInstance();
    const auto& currentScene = sceneManager.GetActiveScene();
    const auto& objects = currentScene->GetObjects();
    for (const auto& obj : objects) {
        if (const auto selectNameComponent = obj->GetComponent<SelectNameComponent>()) {
            if (selectNameComponent->GetPlayerId() == m_PlayerId) {
                return selectNameComponent;
            }
        }
    }
    return nullptr;
}

void SelectOptionLetterCommand::Execute() {
    if (const auto selectName = FindSelectNameComponent()) {
        dae::Message message;
        message.type = dae::PlaySoundMessageType::Sound;
        message.arguments.emplace_back(static_cast<sound_id>(18)); 
        dae::EventQueue::Broadcast(message);
        selectName->ConfirmLetter();
    }
}

SelectNameComponent* SelectOptionLetterCommand::FindSelectNameComponent() const
{
    const auto& sceneManager = dae::SceneManager::GetInstance();
    const auto& currentScene = sceneManager.GetActiveScene();
    const auto& objects = currentScene->GetObjects();
    for (const auto& obj : objects) {
        if (const auto selectNameComponent = obj->GetComponent<SelectNameComponent>()) {
            if (selectNameComponent->GetPlayerId() == m_PlayerId) {
                return selectNameComponent;
            }
        }
    }
    return nullptr;
}

void saveScoreCommand::Execute() {
    auto& gameData = GameData::GetInstance();
    int numberOfPlayers = gameData.GetNumberOfPlayers();

    for (int i = 0; i < numberOfPlayers; ++i) {
        std::string currentName = "";
        if (const auto selectName = FindSelectNameComponent(i)) {
            currentName = selectName->GetCurrentName();
            HighScores::GetInstance().saveNewScore(currentName, gameData.GetPlayerData(i).score);
        }
        gameData.ResetPlayerData(i);
    }

    dae::SceneManager::GetInstance().SetActiveScene("ScoreboardScene");
}

SelectNameComponent* saveScoreCommand::FindSelectNameComponent(int playerId) {
    const auto& sceneManager = dae::SceneManager::GetInstance();
    const auto& currentScene = sceneManager.GetActiveScene();
    const auto& objects = currentScene->GetObjects();
    for (const auto& obj : objects) {
        if (const auto selectNameComponent = obj->GetComponent<SelectNameComponent>()) {
            if (selectNameComponent->GetPlayerId() == playerId) {
                return selectNameComponent;
            }
        }
    }
    return nullptr;
}