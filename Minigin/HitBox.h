#pragma once
#include <SDL.h>
#include <glm/glm.hpp>
#include "Component.h"
#include "GameObject.h"
#include "Renderer.h"

class HitBox final : public dae::Component {
public:
    HitBox(const glm::vec2& size)
        : m_position(0, 0), m_size(size), m_enabled(true) 
    {
    }

    SDL_Rect GetRect() const {
        if (const auto parent = GetGameObject()) {
            const glm::vec3 parentPosition = parent->GetWorldPosition();
            return SDL_Rect{
                static_cast<int>(parentPosition.x - m_size.x / 2),
                static_cast<int>(parentPosition.y - m_size.y / 2),
                static_cast<int>(m_size.x),
                static_cast<int>(m_size.y)
            };
        }
        return {
                static_cast<int>(m_position.x - m_size.x / 2),
                static_cast<int>(m_position.y - m_size.y / 2),
                static_cast<int>(m_size.x),
                static_cast<int>(m_size.y)
        };
    }

    bool IsColliding(const HitBox& other) const {
        if (!m_enabled || !other.m_enabled) return false; 

        const SDL_Rect rectA = GetRect();
        const SDL_Rect rectB = other.GetRect();
        return SDL_HasIntersection(&rectA, &rectB);
    }

    void SetPosition(const glm::vec2& position) {
        m_position = position;
    }

    void SetSize(const glm::vec2& size) {
        m_size = size;
    }

    void Update() override {
        if (const auto parent = GetGameObject()) {
            m_position = parent->GetWorldPosition();
        }
    }

    const std::type_info& GetComponentType() const override {
        return typeid(HitBox);
    }

    void Render() const override {
        if (!m_enabled) return;
#if _DEBUG
        const SDL_Rect rect = GetRect();
        constexpr SDL_Color color = { 255, 0, 0, 255 }; 
        dae::Renderer::GetInstance().RenderRect(rect, color, false);
#endif
    }
    bool GetEnable() const
    {
        return m_enabled;
    }
    void Enable() {
        m_enabled = true;
    }

    void Disable() {
        m_enabled = false;
    }

private:
    glm::vec2 m_position;
    glm::vec2 m_size;
    bool m_enabled; // Track whether the hitbox is enabled or disabled
};
