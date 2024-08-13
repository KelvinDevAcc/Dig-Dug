#pragma once
#include <memory>
#include <string>
#include <vector>
#include "GameObject.h"
#include "sound_system.h"

namespace dae
{
    class Scene final
    {
    public:
        using ActivateCallback = std::function<void()>;

        void Add(std::unique_ptr<GameObject> object) const;
        void Remove(GameObject* object) const;
        void RemoveAll() const;

        void Update() const;
        void Render() const;

        const std::string& GetName() const { return m_name; }
        const std::vector<std::unique_ptr<GameObject>>& GetObjects() const;

        explicit Scene(std::string name);
        ~Scene() = default;
        Scene(const Scene& other) = delete;
        Scene(Scene&& other) = delete;
        Scene& operator=(const Scene& other) = delete;
        Scene& operator=(Scene&& other) = delete;

        void SetBackgroundMusic(const std::string& musicFilePath);
        void SetBackgroundMusic(int musicId);
        void PlayBackgroundMusic() const;
        void StopBackgroundMusic() const;
        sound_id GetBackgroundMusicID() const { return  m_backgroundMusicID; }


        void SetOnActivateCallback(ActivateCallback callback)
        {
            m_onActivateCallback = std::move(callback);
        }

        void Activate() const
        {
            if (m_onActivateCallback)
            {
                m_onActivateCallback();
            }
        }

    private:

        std::string m_name;
        mutable std::vector<std::unique_ptr<GameObject>> m_objects{};
        sound_id m_backgroundMusicID{ 0 };

        ActivateCallback m_onActivateCallback;


        mutable bool m_needsSorting = true; 
        mutable std::vector<GameObject*> m_sortedObjects;

        void SortObjects() const;
    };
}
