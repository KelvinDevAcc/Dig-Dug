#include "GameObject.h"

#include <algorithm>
#include <iostream>

#include "ResourceManager.h"
#include "Renderer.h"

namespace dae
{
    GameObject::GameObject()
	    : m_components(), m_width(), m_height(), m_originalTexWidth(),
	      m_originalTexHeight(), m_localPosition(), m_worldPosition(), m_positionIsDirty(false)
    {
    }

    void GameObject::Update() const
    {
        for (const auto& component : m_components)
        {
            if (component)  // Null check for smart pointer
            {
                component->Update();
            }
            else
            {
                std::cerr << "Warning: Null component in GameObject::Update\n";
            }
        }
    }

    void GameObject::Render() const
    {
        for (const auto& component : m_components)
        {
            if (component)  // Null check
            {
                component->Render();
            }
            else
            {
                std::cerr << "Warning: Null component in GameObject::Render\n";
            }
        }
    }


    void GameObject::SetDimensions(float width, float height)
    {
        m_width = width;
        m_height = height;
    }

    std::pair<float, float> GameObject::GetDimensions() const
    {
        return { m_width, m_height };
    }

    void GameObject::AddComponent(std::unique_ptr<Component> component)
    {
        m_components.push_back(std::move(component));
    }

    void GameObject::RemoveComponent(Component* component)
    {
        const auto it = std::ranges::find_if(m_components, [component](const auto& ptr) { return ptr.get() == component; });
        if (it != m_components.end())
        {
            m_components.erase(it);
        }
    }

    void GameObject::RemoveAllComponents()
    {
        m_components.clear();
    }

    void GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
    {
        if (IsChild(parent) || parent == this || m_parent == parent)
            return;
        if (parent == nullptr)
            SetLocalPosition(GetWorldPosition());
        else
        {
            if (keepWorldPosition)
                SetLocalPosition(GetWorldPosition() - parent->GetWorldPosition());
            SetPositionDirty();
        }
        if (m_parent) m_parent->RemoveChild(this);
        m_parent = parent;
        if (m_parent) m_parent->AddChild(this);

    }


    void GameObject::SetLocalPosition(const glm::vec3& pos)
    {
        m_localPosition.SetPosition(pos);
        SetPositionDirty();
    }

    const glm::vec3& GameObject::GetWorldPosition() const
    {
        if (m_positionIsDirty)
            const_cast<GameObject*>(this)->UpdateWorldPosition();
        return m_worldPosition.GetPosition();
    }

    glm::vec3 GameObject::GetLocalPosition() const
    {
        return m_localPosition.GetPosition();
    }

    void GameObject::UpdateWorldPosition() 
    {
        if (m_positionIsDirty)
        {
            if (m_parent == nullptr) {
                m_worldPosition = m_localPosition;
            }
            else
                m_worldPosition.SetPosition(m_parent->GetWorldPosition() + m_localPosition.GetPosition());
        }
        m_positionIsDirty = false;
    }

    void GameObject::RemoveChild(GameObject* child) {
        std::erase(m_children, child);
    }


    void GameObject::AddChild(GameObject* child)
    {
	    m_children.emplace_back(child);
    }

    bool GameObject::IsChild(GameObject* potentialChild) const
    {
        return std::ranges::any_of(m_children, [potentialChild](const GameObject* child) {
            return child == potentialChild || child->IsChild(potentialChild);
            });
    }

    void GameObject::SetPositionDirty() {
        m_positionIsDirty = true;
        for (const auto child : m_children)
        {
            child->SetPositionDirty();
        }

    }

    float GameObject::GetRotation() const
    {
        return m_worldPosition.GetRotation();
    }
}