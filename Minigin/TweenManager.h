#pragma once

#include <vector>
#include "Tween.h"

class TweenManager
{
public:
    static TweenManager& GetInstance();

    void AddTween(std::unique_ptr<Tween> tween);
    void Update();
    void ClearAllTweens() { m_Tweens.clear();}
    const std::vector<std::unique_ptr<Tween>>& GetAllTweens() const{ return m_Tweens;}
private:
    TweenManager() = default;
    ~TweenManager() = default;
    TweenManager(const TweenManager&) = delete;
    TweenManager& operator=(const TweenManager&) = delete;

    std::vector<std::unique_ptr<Tween>> m_Tweens;
};