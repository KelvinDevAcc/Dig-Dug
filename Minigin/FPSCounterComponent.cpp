#include "FPSCounterComponent.h"
#include "GameTime.h"
#include "TextComponent.h"

dae::FPSCounterComponent::FPSCounterComponent(TextComponent* textComponent)
	:m_TextComponent(textComponent)
{
}

void dae::FPSCounterComponent::Update()
{
    m_frameCount++;
    m_elapsedTime += GameTime::GetDeltaTime();

    if (m_elapsedTime >= 1.0f)
    {
        m_fps = static_cast<float>(m_frameCount) / m_elapsedTime;

        m_frameCount = 0;
        m_elapsedTime = 0.0f;
    }

    if (m_TextComponent)
    {
        // Format FPS value with one decimal place precision
        std::stringstream stream;
        stream << std::fixed << std::setprecision(1) << m_fps;
        const std::string fpsText = "FPS: " + stream.str();

        m_TextComponent->SetText(fpsText);
    }
}

float dae::FPSCounterComponent::GetFPS() const
{
    return m_fps;
}



