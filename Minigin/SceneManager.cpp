#include "SceneManager.h"

#include <stdexcept>

#include "SceneData.h"
#include "servicelocator.h"
#include "../DigDug2/Player.h"
#include "../DigDug2/EnemyComponent.h"
#include "../DigDug2/EnemyPlayer.h"
#include "../DigDug2/FygarComponent.h"


class EnemyComponent;

namespace dae
{
    Scene* SceneManager::CreateScene(const std::string& name)
    {
        auto scene = std::make_unique<Scene>(name);
        m_scenes.push_back(std::move(scene));

        return m_scenes.back().get();
    }

    Scene* SceneManager::GetActiveScene() const
    {
        return m_activeSceneIterator->get();
    }

    void SceneManager::SetActiveScene(const std::string& name)
    {
        for (auto it = m_scenes.begin(); it != m_scenes.end(); ++it)
        {
            if ((*it)->GetName() == name)
            {
	            if (it != m_scenes.begin())
	            {
                    const sound_id previousSoundID = (*m_activeSceneIterator)->GetBackgroundMusicID();
                    if (previousSoundID != 0)
                    {
                        auto& soundSystem = servicelocator::get_sound_system();
                        soundSystem.StopPlay(previousSoundID);  // Stop the specific music for the previously active scene
                    }
	            }

                m_activeSceneIterator = it;

               

                SceneData::GetInstance().RemoveAllGameObjects();
                (*m_activeSceneIterator)->RemoveAll();
                (*m_activeSceneIterator)->Activate();

                // Start the background music for the newly active scene (if any)
                const sound_id newSoundID = (*m_activeSceneIterator)->GetBackgroundMusicID();
                if (newSoundID != 0)
                {
                    auto& soundSystem = servicelocator::get_sound_system();
                    soundSystem.play(newSoundID);
                }

                // Store the previous active scene iterator for future reference
                m_previousActiveSceneIterator = m_activeSceneIterator;

                return;
            }
        }
        // Throw an exception if the scene with the specified name is not found
        throw std::runtime_error("Scene not found: " + name);
    }


    void SceneManager::GoToNextScene()
    {
        m_previousActiveSceneIterator = m_activeSceneIterator;

        // Stop the background music for the currently active scene, if any
        const sound_id previousSoundID = (*m_activeSceneIterator)->GetBackgroundMusicID();
        if (previousSoundID != 0)
        {
            auto& soundSystem = servicelocator::get_sound_system();
            soundSystem.StopPlay(previousSoundID);  // Stop the specific music for the previously active scene
        }

        // Advance to the next scene
        ++m_activeSceneIterator;
        if (m_activeSceneIterator == m_scenes.end())
            m_activeSceneIterator = m_scenes.begin();

        SceneData::GetInstance().RemoveAllGameObjects();
        (*m_activeSceneIterator)->RemoveAll();
        (*m_activeSceneIterator)->Activate();

        auto& soundSystem = servicelocator::get_sound_system();

        // Start the background music for the newly active scene (if any)
        const sound_id newSoundID = (*m_activeSceneIterator)->GetBackgroundMusicID();
        if (newSoundID != 0)
        {
            soundSystem.play(newSoundID);
        }
    }

    void SceneManager::RestartCurrentSceneWithPersistentObjects()
    {

        const auto enemies = SceneData::GetInstance().GetEnemies();
        const auto enemieplayers = SceneData::GetInstance().GetenemyPlayers();
        const auto players = SceneData::GetInstance().GetPlayers();

        for (const auto& obj : enemies)
        {

            if (const auto pooka = obj->GetComponent<EnemyComponent>())
            {
                pooka->ReSpawn();
            }
            
        }

        for (const auto& obj : enemieplayers)
        {

            if (const auto enemyPlayer = obj->GetComponent<game::EnemyPlayer>())
            {
                enemyPlayer->ReSpawn();
            }

        }

        for (const auto& obj : players)
        {
            if (const auto player = obj->GetComponent<game::Player>())
            {
                player->ReSpawn();
            }
           

        }

       

    }

    void SceneManager::Update() const
    {
	    if (m_scenes.empty())
            return;

        if (m_activeSceneIterator != m_scenes.end())
        {
            if (!(*m_activeSceneIterator)->GetObjects().empty())
            {
                (*m_activeSceneIterator)->Update();
            }
            
        }
    }

    void SceneManager::Render() const
    {
        if (m_scenes.empty() || (*m_activeSceneIterator) == nullptr)
            return;

        if (m_activeSceneIterator != m_scenes.end())
        {
            if (!(*m_activeSceneIterator)->GetObjects().empty())
            {
                (*m_activeSceneIterator)->Render();

            }
        }
    }
}
