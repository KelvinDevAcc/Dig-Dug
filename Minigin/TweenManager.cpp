#include "TweenManager.h"
#include <algorithm>

#include "GameTime.h"

TweenManager& TweenManager::GetInstance()
{
    static TweenManager instance;
    return instance;
}

void TweenManager::AddTween(std::unique_ptr<Tween> tween)
{
    m_Tweens.push_back(std::move(tween));
}

void TweenManager::Update()
{
    for (const auto& tween : m_Tweens)
    {
        tween->Update();
    }
    std::erase_if(m_Tweens, [](const std::unique_ptr<Tween>& t) { return !t->IsActive(); });
}