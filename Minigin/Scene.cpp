#include "Scene.h"
#include <iostream>
#include <string>
#include "HitBox.h"
#include "servicelocator.h"
#include "SpriteRendererComponent.h"
#include "TweenManager.h"

namespace dae
{
    Scene::Scene(std::string name) : m_name(std::move(name)) {}

    void Scene::Add(std::unique_ptr<GameObject> object) const
    {
        m_objects.push_back(std::move(object));
    }

    void Scene::Remove(GameObject* object) const
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
        m_needsSorting = true;
    }

    void Scene::RemoveAll() const
    {
        m_objects.clear();
        m_sortedObjects.clear();
        TweenManager::GetInstance().ClearAllTweens();
    }

    void Scene::Update() const
    {
        if (m_objects.empty())
            return;

        for (const auto& object : m_objects)
        {
            if (object)
            {
                object->Update();
            }
        }

        if (m_sortedObjects.size() < m_objects.size() )
        {
            m_needsSorting = true;
        }
    }

    void Scene::Render() const
    {
        if (m_objects.empty())
            return;

        if (m_needsSorting)
        {
            SortObjects();
            m_needsSorting = false;
        }

        for (const auto& object : m_sortedObjects)
        {
	        if (object)
	        {
                object->Render();
	        }
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

    void Scene::SortObjects() const
    {
        // Extract raw pointers from unique_ptrs
        m_sortedObjects.clear();
        m_sortedObjects.reserve(m_objects.size());
        for (const auto& obj : m_objects)
        {
            m_sortedObjects.push_back(obj.get());
        }

        // Sort raw pointers by their zIndex (z-coordinate in this case)
        std::ranges::sort(m_sortedObjects,
            [](const GameObject* a, const GameObject* b) {
                return a->GetWorldPosition().z < b->GetWorldPosition().z;
            });
    }
}
