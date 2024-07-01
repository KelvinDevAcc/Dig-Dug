#pragma once
#include <vector>
#include <string>
#include <memory>

#include "Singleton.h"
#include "Scene.h"

namespace dae
{
    class SceneManager final : public Singleton<SceneManager>
    {
    public:
        SceneManager() = default;
        ~SceneManager() override = default;

        SceneManager(SceneManager&&) noexcept = delete;
        SceneManager& operator=(SceneManager&&) noexcept = delete;


        Scene* CreateScene(const std::string& name);
        Scene* GetActiveScene() const;
        void SetActiveScene(const std::string& name);
        void GoToNextScene();

        void Update() const;
        void Render() const;

    private:
        std::vector<std::unique_ptr<Scene>> m_scenes{};
        std::vector<std::unique_ptr<Scene>>::iterator m_activeSceneIterator;
        std::vector<std::unique_ptr<Scene>>::iterator m_previousActiveSceneIterator;
    };
}
