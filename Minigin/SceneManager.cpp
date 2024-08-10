#include "SceneManager.h"

#include <iostream>
#include <ostream>
#include <SDL_mixer.h>
#include <stdexcept>

#include "GameData.h"
#include "SceneData.h"
#include "servicelocator.h"
#include "../DigDug2/Player.h"
#include "../DigDug2/PookaComponent.h"




class PookaComponent;

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
                m_activeSceneIterator = it;

                if (m_activeSceneIterator != m_scenes.end()) // Check if there is an active scene
                {
                    auto& soundSystem = servicelocator::get_sound_system();
                    const sound_id previousSoundID = (*m_activeSceneIterator)->GetBackgroundMusicID();
                    std::cout << (*m_activeSceneIterator)->GetBackgroundMusicID();
                    if (previousSoundID != 0)
                    {
                        soundSystem.StopPlay(previousSoundID);  // Stop the specific music for the active scene
                    }
                }

                // Set the new active scene
                GameData::GetInstance().FindAndStorePlayerData();
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
        GameData::GetInstance().FindAndStorePlayerData();
        m_previousActiveSceneIterator = m_activeSceneIterator;
        // Advance to the next scene
        ++m_activeSceneIterator;
        if (m_activeSceneIterator == m_scenes.end())
            m_activeSceneIterator = m_scenes.begin();
        SceneData::GetInstance().RemoveAllGameObjects();
        (*m_activeSceneIterator)->RemoveAll();
        (*m_activeSceneIterator)->Activate();


        auto& soundSystem = servicelocator::get_sound_system();
        const sound_id soundID = (*m_activeSceneIterator)->GetBackgroundMusicID();
        if (soundID != 0)
        {
            soundSystem.play(soundID);
        }
    }

    void SceneManager::RestartCurrentSceneWithPersistentObjects()
    {
        // Store the current state of objects that should persist
        //std::vector<GameObject*> persistentObjects;
        //for (const auto& obj : (*m_activeSceneIterator)->GetObjects())
        //{
        //    // Add logic here to determine if the object should persist
        //    persistentObjects.push_back(obj.get());  // Store the raw pointer
        //}

        const auto players = SceneData::GetInstance().GetPlayers();
        const auto enemies = SceneData::GetInstance().GetEnemies();
        // Re-add persistent objects
      
        for (const auto& obj : enemies)
        {
            if (const auto pooka = obj->GetComponent<PookaComponent>())
            {
                pooka->ReSpawn();
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
