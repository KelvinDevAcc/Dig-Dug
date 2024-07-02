#pragma once
#include <chrono>

namespace dae
{
    class GameTime final
    {
        // Granting access to Minigin class
        friend class Minigin;

    public:
        // Gets the delta time adjusted by the time scale
        static float GetDeltaTime()
        {
            return m_DeltaTime * m_TimeScale;
        }

        // Sets the time scale for delta time
        static void SetTimeScale(float timeScale)
        {
            m_TimeScale = timeScale;
        }

        // Gets the current time scale
        static float GetTimeScale()
        {
            return m_TimeScale;
        }

        // Initializes the start and last time points
        static void Start()
        {
            m_StartTime = std::chrono::high_resolution_clock::now();
            m_LastTime = m_StartTime;
        }

        // Updates the delta time based on the current time
        static void Update()
        {
            const auto currentTime = std::chrono::high_resolution_clock::now();
            m_DeltaTime = std::chrono::duration<float>(currentTime - m_LastTime).count();
            m_LastTime = currentTime;
        }

        GameTime() = default;
        ~GameTime() = default;

        // Start time point
        inline static std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;
        // Last updated time point
        inline static std::chrono::time_point<std::chrono::high_resolution_clock> m_LastTime;
        // Time difference between the current and last frame
        inline static float m_DeltaTime = 0.0f;
        // Scale factor for time
        inline static float m_TimeScale = 1.0f;
    };
}
