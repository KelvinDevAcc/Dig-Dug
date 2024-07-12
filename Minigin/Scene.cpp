#include "Scene.h"
#include <iostream>
#include <string>
#include "HitBox.h"
#include "servicelocator.h"

namespace dae
{
    Scene::Scene(std::string name) : m_name(std::move(name)) {}

    void Scene::Add(std::unique_ptr<GameObject> object)
    {
        m_objects.push_back(std::move(object));
    }

    void Scene::Remove(GameObject* object)
    {
        m_objects.erase(std::ranges::remove_if(m_objects,
                                               [object](const std::unique_ptr<GameObject>& ptr)
                                               {
	                                               if (ptr.get() == object)
	                                               {
		                                               ptr->RemoveAllComponents();
		                                               return true;
	                                               }
	                                               return false;
                                               }).begin(), m_objects.end());
    }

    void Scene::RemoveAll()
    {
        for (const auto& object : m_objects)
        {
            object->RemoveAllComponents();
        }
        m_objects.clear();
    }

    void Scene::Update() const
    {
        for (const auto& object : m_objects)
        {
            if (object)
            {
                object->Update();
            }
            else
            {
                std::cerr << "Null object found in Scene::Update\n";
            }
        }
    }

    void Scene::Render() const
    {
        for (const auto& object : m_objects)
        {
            object->Render();
        }
    }

    const std::vector<std::unique_ptr<GameObject>>& Scene::GetObjects() const
    {
        return m_objects;
    }

    void Scene::SetBackgroundMusic(const std::string& musicFilePath)
    {
        auto& soundSystem = servicelocator::get_sound_system();
        m_backgroundMusicID = soundSystem.get_sound_id_for_file_path(musicFilePath);
    }

    void Scene::SetBackgroundMusic(int musicId)
    {
        m_backgroundMusicID = static_cast<sound_id>(musicId);
    }

    void Scene::PlayBackgroundMusic() const
    {
        if (m_backgroundMusicID != 0)
        {
            auto& soundSystem = servicelocator::get_sound_system();
            soundSystem.play(m_backgroundMusicID);
        }
    }

    void Scene::StopBackgroundMusic() const
    {
        if (m_backgroundMusicID)
        {
            //auto& soundSystem = servicelocator::get_sound_system();
            //soundSystem.StopPlaying(m_backgroundMusicID);
            //m_backgroundMusicID = 0; // Clear the stored background music ID
        }
    }
}
