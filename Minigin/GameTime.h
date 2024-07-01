#pragma once

#include <chrono>

namespace dae
{
    class GameTime final
    {
        friend class Minigin;

    public:

        static float GetDeltaTime()
        {
            return m_DeltaTime;
        }

    private:
        GameTime() = default;
        ~GameTime() = default;

        static void Start()
        {
            m_StartTime = std::chrono::high_resolution_clock::now();
        }

        static void Update()
        {
            const auto currentTime = std::chrono::high_resolution_clock::now();
            m_DeltaTime = std::chrono::duration<float>(currentTime - m_LastTime).count();
            m_LastTime = currentTime;
        }

        inline static std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;
        inline static std::chrono::time_point<std::chrono::high_resolution_clock> m_LastTime;
        inline static float m_DeltaTime = 0.0f;
    };
}