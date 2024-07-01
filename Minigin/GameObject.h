#pragma once
#include <memory>
#include <vector>
#include "Component.h"
#include "Transform.h"
#include "Texture2D.h"

namespace dae
{
    class GameObject final
    {
    public:
        GameObject();
        ~GameObject() = default;

        GameObject(const GameObject& other) = delete;
        GameObject(GameObject&& other) noexcept = delete;
        GameObject& operator=(const GameObject& other) = delete;
        GameObject& operator=(GameObject&& other) noexcept = delete;

        void Update() const;
        void Render() const;

        void SetRotation(float rotation) { m_worldPosition.SetRotation(rotation); }

        void SetDimensions(float width, float height);
        std::pair<float, float> GetDimensions() const;
        void AddComponent(std::unique_ptr<Component> component);
        void RemoveComponent(Component* component); 
        void SetParent(GameObject* parent, bool keepWorldPosition = false);

        void SetLocalPosition(const glm::vec3& pos);
        const glm::vec3& GetWorldPosition();
        glm::vec3 GetLocalPosition() const;

        float GetRotation() const;

        template <typename T>
        T* GetComponent() // Return raw pointer instead of shared_ptr
        {
            for (const auto& component : m_components)
            {
                if (auto derived = dynamic_cast<T*>(component.get()))
                    return derived;
            }
            return nullptr;
        }

        template <typename T>
        bool HasComponent()
        {
            return GetComponent<T>() != nullptr;
        }

        const std::vector<std::unique_ptr<Component>>& GetComponents() const { return m_components; }

        bool IsChild(GameObject* potentialChild) const;

    private:
        std::unique_ptr<Texture2D> m_texture;
        std::vector<std::unique_ptr<Component>> m_components;
        std::vector<GameObject*> m_children;
        float m_width;
        float m_height;
        float m_originalTexWidth;
        float m_originalTexHeight;
        GameObject* m_parent = nullptr;
        Transform m_localPosition;
        Transform m_worldPosition;
        bool m_positionIsDirty;

        void UpdateWorldPosition();
        void RemoveChild(GameObject* child);
        void AddChild(GameObject* child);
        void SetPositionDirty();
    };
}